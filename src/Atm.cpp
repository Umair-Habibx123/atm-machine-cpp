#include "Atm.hpp"

#include <array>
#include <iostream>
#include <string>

#include "Clock.hpp"
#include "Console.hpp"

namespace atm {
namespace {

const char* const kBorder =
    "\t\t*****************************************************\n";

}  // namespace

void Atm::run() {
    Account* account = welcome();
    if (account) {
        sessionLoop(*account);
    }
}

Account* Atm::welcome() {
    console::clear();
    std::cout << kBorder
              << "\t\t*            * WELCOME TO THE ATM *               *\n"
              << "\t\t*                                                 *\n"
              << "\t\t*            1. LOG IN                            *\n"
              << "\t\t*            2. CREATE NEW ACCOUNT                *\n"
              << "\t\t*            3. EXIT                              *\n"
              << kBorder;

    int choice = console::readInt("\n\t\tSelect an option (1-3) : ");
    switch (choice) {
        case 1:  return doLogin();
        case 2:  return doRegister();
        default: return nullptr;
    }
}

Account* Atm::doRegister() {
    console::clear();
    std::cout << kBorder
              << "\t\t*            * CREATE NEW ACCOUNT *               *\n"
              << kBorder << '\n';

    std::string username = console::readLine("\t\tChoose a username : ");
    std::string pin = console::readSecret("\t\tChoose a PIN      : ");

    auto number = bank_.registerUser(username, pin);
    if (!number) {
        std::cout << "\n\t\tThat username is already taken. Try logging in.\n";
        console::pause();
        return welcome();
    }

    std::cout << "\n\t\tAccount created! Your account number is " << *number << ".\n"
              << "\t\tPlease keep it safe -- others use it to send you money.\n";
    console::pause();
    return doLogin();
}

Account* Atm::doLogin() {
    for (int attempt = 1; attempt <= kMaxLoginAttempts; ++attempt) {
        console::clear();
        std::cout << kBorder
                  << "\t\t*                 * LOG IN *                      *\n"
                  << kBorder << '\n';

        std::string username = console::readLine("\t\tUsername : ");
        std::string pin = console::readSecret("\t\tPIN      : ");

        if (Account* account = bank_.login(username, pin)) {
            return account;
        }

        std::cout << "\n\t\tIncorrect credentials -- attempt " << attempt
                  << " of " << kMaxLoginAttempts << ".\n";
        console::pause();
    }
    console::clear();
    std::cout << "\n\t\tLIMIT EXCEEDED -- access locked. Goodbye.\n";
    return nullptr;
}

void Atm::sessionLoop(Account& account) {
    while (true) {
        console::clear();
        showMenu(account);

        int choice = console::readInt("\n\t\tSelect an option (1-7) : ");
        console::clear();

        switch (choice) {
            case 1: doCheckBalance(account); break;
            case 2: doDeposit(account); break;
            case 3: doWithdraw(account); break;
            case 4: doSendMoney(account); break;
            case 5: doHistory(account); break;
            case 6: doPayBills(account); break;
            case 7:
                std::cout << "\n\t\tThanks for banking with us, " << account.username()
                          << ". \"GOOD BYE\"\n";
                return;
            default:
                std::cout << "\n\t\tInvalid entry -- please choose 1 to 7.\n";
        }
        console::pause();
    }
}

void Atm::showMenu(const Account& account) const {
    std::cout << kBorder
              << "\t\t*            * WELCOME TO THE MENU *               *\n"
              << "\t\t*                                                 *\n"
              << "\t\t*            1. CHECK BALANCE                      *\n"
              << "\t\t*            2. DEPOSIT MONEY                      *\n"
              << "\t\t*            3. WITHDRAW MONEY                     *\n"
              << "\t\t*            4. SEND MONEY                         *\n"
              << "\t\t*            5. TRANSACTION HISTORY                *\n"
              << "\t\t*            6. PAY BILLS                          *\n"
              << "\t\t*            7. EXIT                               *\n"
              << kBorder;
    std::cout << "\t\t" << account.username() << "  |  account #" << account.number()
              << "  |  " << account.balance().format() << '\n';
    if (account.balance().isZero()) {
        std::cout << "\t\tYour balance is 0 -- start by depositing money.\n";
    }
}

void Atm::doCheckBalance(const Account& account) const {
    std::cout << "\n\t\tYour balance is " << account.balance().format() << '\n';
}

void Atm::doDeposit(Account& account) {
    Money amount = console::readAmount("\n\t\tDeposit amount : ");
    account.deposit(amount, "cash deposit");
    bank_.save();
    std::cout << "\t\tDeposited " << amount.format() << ". New balance: "
              << account.balance().format() << '\n';
}

void Atm::doWithdraw(Account& account) {
    Money amount = console::readAmount("\n\t\tWithdraw amount : ");
    if (account.withdraw(amount, "cash withdrawal")) {
        bank_.save();
        std::cout << "\t\tWithdrew " << amount.format() << ". New balance: "
                  << account.balance().format() << '\n';
    } else {
        std::cout << "\t\tERROR: amount exceeds your available balance ("
                  << account.balance().format() << ").\n";
    }
}

void Atm::doSendMoney(Account& account) {
    long long recipient = console::readInt("\n\t\tRecipient account number : ");
    Money amount = console::readAmount("\t\tAmount to send           : ");

    switch (bank_.transfer(account, recipient, amount)) {
        case Bank::TransferResult::Ok:
            std::cout << "\n\t\tSent " << amount.format() << " to account #"
                      << recipient << ". It was received instantly.\n"
                      << "\t\tNew balance: " << account.balance().format() << '\n';
            break;
        case Bank::TransferResult::InsufficientFunds:
            std::cout << "\t\tERROR: amount exceeds your available balance ("
                      << account.balance().format() << ").\n";
            break;
        case Bank::TransferResult::RecipientNotFound:
            std::cout << "\t\tERROR: no account found with number " << recipient << ".\n";
            break;
        case Bank::TransferResult::SameAccount:
            std::cout << "\t\tERROR: you cannot send money to yourself.\n";
            break;
    }
}

void Atm::doHistory(const Account& account) const {
    const auto& history = account.history();
    std::cout << "\n\t\t==================== STATEMENT ====================\n";
    if (history.empty()) {
        std::cout << "\t\t(no transactions yet)\n";
    }
    for (const auto& tx : history) {
        const char* sign = isDebit(tx.type) ? "-" : "+";
        std::cout << "\t\t" << clock::formatTimestamp(tx.timestamp) << "  "
                  << sign << tx.amount.format() << "  " << toString(tx.type);
        if (!tx.detail.empty()) std::cout << "  (" << tx.detail << ")";
        std::cout << '\n';
    }
    std::cout << "\t\t--------------------------------------------------\n"
              << "\t\tCurrent balance: " << account.balance().format() << '\n';
}

void Atm::doPayBills(Account& account) {
    if (account.balance().isZero()) {
        std::cout << "\n\t\tYou cannot pay a bill -- your balance is 0.\n";
        return;
    }

    static constexpr std::array<const char*, 3> kBills{"electricity", "water", "gas"};

    std::cout << kBorder;
    for (std::size_t i = 0; i < kBills.size(); ++i) {
        const char* name = kBills[i];
        Money due = bank_.billAmountDue(name);
        bool paid = account.hasPaidBill(name, clock::currentYearMonth());
        std::cout << "\t\t*  " << (i + 1) << ". " << name;
        std::cout << std::string(14 - std::string(name).size(), ' ');
        if (paid) {
            std::cout << "PAID this month";
        } else {
            std::cout << "due " << due.format();
        }
        std::cout << '\n';
    }
    std::cout << kBorder;

    int choice = console::readInt("\n\t\tSelect a bill to pay (1-3) : ");
    if (choice < 1 || choice > 3) {
        std::cout << "\n\t\tInvalid entry.\n";
        return;
    }
    const char* name = kBills[static_cast<std::size_t>(choice - 1)];
    Money due = bank_.billAmountDue(name);

    int confirm = console::readInt(
        "\t\tPay your " + std::string(name) + " bill of " + due.format() +
        "?  (1 = yes, 0 = no) : ");
    if (confirm != 1) {
        std::cout << "\t\tPayment cancelled.\n";
        return;
    }

    switch (bank_.payBill(account, name)) {
        case Bank::BillResult::Ok:
            std::cout << "\t\tPaid your " << name << " bill of " << due.format()
                      << ". New balance: " << account.balance().format() << '\n';
            break;
        case Bank::BillResult::AlreadyPaidThisMonth:
            std::cout << "\t\tYou have already paid your " << name
                      << " bill this month.\n";
            break;
        case Bank::BillResult::InsufficientFunds:
            std::cout << "\t\tInsufficient balance to pay this bill ("
                      << account.balance().format() << ").\n";
            break;
    }
}

}  // namespace atm
