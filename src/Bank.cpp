#include "Bank.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "Clock.hpp"
#include "Hash.hpp"

namespace atm {
namespace {

// '|' is our field separator, so it must never appear inside a field.
std::string sanitize(std::string text) {
    std::replace(text.begin(), text.end(), '|', '/');
    return text;
}

std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, delim)) parts.push_back(item);
    return parts;
}

}  // namespace

Bank::Bank(std::string path) : path_(std::move(path)) {
    if (!load()) {
        seedDemoAccounts();
        save();
    }
}

bool Bank::load() {
    std::ifstream in(path_);
    if (!in.is_open()) return false;  // no DB yet -> caller seeds demo accounts

    accounts_.clear();
    Account* current = nullptr;
    std::int64_t maxNumber = 1000;
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto f = split(line, '|');

        if (f[0] == "ACCT" && f.size() == 5) {
            std::int64_t number = std::stoll(f[1]);
            accounts_.push_back(Account::restore(
                number, f[2], f[3], Money{std::stoll(f[4])}));
            current = &accounts_.back();
            maxNumber = std::max(maxNumber, number);
        } else if (f[0] == "TX" && f.size() == 5 && current) {
            TxType type;
            if (!parseTxType(f[2], type)) continue;
            current->loadTransaction(Transaction{
                static_cast<std::time_t>(std::stoll(f[1])), type,
                Money{std::stoll(f[3])}, f[4]});
        } else if (f[0] == "BILL" && f.size() == 3 && current) {
            current->loadBillPaid(f[1], f[2]);
        }
    }

    nextNumber_ = maxNumber + 1;
    return true;
}

void Bank::save() const {
    std::ofstream out(path_, std::ios::trunc);
    if (!out) return;

    for (const auto& acc : accounts_) {
        out << "ACCT|" << acc.number() << '|' << sanitize(acc.username()) << '|'
            << acc.pinHash() << '|' << acc.balance().paisa() << '\n';
        for (const auto& tx : acc.history()) {
            out << "TX|" << static_cast<long long>(tx.timestamp) << '|'
                << toString(tx.type) << '|' << tx.amount.paisa() << '|'
                << sanitize(tx.detail) << '\n';
        }
        for (const auto& [bill, ym] : acc.billsPaid()) {
            out << "BILL|" << sanitize(bill) << '|' << ym << '\n';
        }
    }
}

void Bank::seedDemoAccounts() {
    // Two ready-made recipients so "Send money" works on a fresh install.
    Account bob = Account::create(nextNumber_++, "bob", "0000");
    bob.deposit(Money::fromRupees(50000), "opening balance");
    accounts_.push_back(std::move(bob));

    Account carol = Account::create(nextNumber_++, "carol", "0000");
    carol.deposit(Money::fromRupees(12000), "opening balance");
    accounts_.push_back(std::move(carol));
}

std::optional<std::int64_t> Bank::registerUser(const std::string& username,
                                               const std::string& plainPin) {
    for (const auto& acc : accounts_) {
        if (acc.username() == username) return std::nullopt;  // taken
    }
    std::int64_t number = nextNumber_++;
    accounts_.push_back(Account::create(number, username, plainPin));
    save();
    return number;
}

Account* Bank::login(const std::string& username, const std::string& plainPin) {
    for (auto& acc : accounts_) {
        if (acc.username() == username && acc.checkPin(plainPin)) return &acc;
    }
    return nullptr;
}

Account* Bank::findByNumber(std::int64_t number) {
    for (auto& acc : accounts_) {
        if (acc.number() == number) return &acc;
    }
    return nullptr;
}

Bank::TransferResult Bank::transfer(Account& from, std::int64_t toNumber, Money amount) {
    if (toNumber == from.number()) return TransferResult::SameAccount;
    Account* to = findByNumber(toNumber);
    if (!to) return TransferResult::RecipientNotFound;

    std::ostringstream outDetail, inDetail;
    outDetail << "to " << to->username() << " (#" << to->number() << ")";
    inDetail << "from " << from.username() << " (#" << from.number() << ")";

    if (!from.transferOut(amount, outDetail.str())) {
        return TransferResult::InsufficientFunds;
    }
    to->credit(amount, inDetail.str());  // auto-received on the other side
    save();
    return TransferResult::Ok;
}

Money Bank::billAmountDue(const std::string& billName) const {
    // Deterministic for a given (bill, month): stable until paid, fresh next
    // month. Ranges are roughly realistic per utility.
    int minR = 500, maxR = 2500;
    if (billName == "electricity") { minR = 1500; maxR = 6000; }
    else if (billName == "water")  { minR = 400;  maxR = 1800; }
    else if (billName == "gas")    { minR = 700;  maxR = 3000; }

    std::uint64_t h = fnv1aNumber(billName + clock::currentYearMonth());
    int span = maxR - minR + 1;
    int rupees = minR + static_cast<int>(h % static_cast<std::uint64_t>(span));
    return Money::fromRupees(rupees);
}

Bank::BillResult Bank::payBill(Account& account, const std::string& billName) {
    const std::string ym = clock::currentYearMonth();
    if (account.hasPaidBill(billName, ym)) {
        return BillResult::AlreadyPaidThisMonth;
    }
    Money due = billAmountDue(billName);
    if (!account.payBill(due, billName, ym)) {
        return BillResult::InsufficientFunds;
    }
    save();
    return BillResult::Ok;
}

}  // namespace atm
