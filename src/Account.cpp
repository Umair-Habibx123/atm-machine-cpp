#include "Account.hpp"

#include "Clock.hpp"
#include "Hash.hpp"

namespace atm {

Account Account::create(std::int64_t number, std::string username,
                        const std::string& plainPin) {
    return Account(number, std::move(username), fnv1aHex(plainPin), Money{});
}

Account Account::restore(std::int64_t number, std::string username,
                         std::string pinHash, Money balance) {
    return Account(number, std::move(username), std::move(pinHash), balance);
}

bool Account::checkPin(const std::string& plainPin) const {
    return fnv1aHex(plainPin) == pinHash_;
}

void Account::record(TxType type, Money amount, const std::string& detail) {
    history_.push_back(Transaction{clock::now(), type, amount, detail});
}

void Account::deposit(Money amount, const std::string& detail) {
    if (amount > Money{}) {
        balance_ += amount;
        record(TxType::Deposit, amount, detail);
    }
}

bool Account::withdraw(Money amount, const std::string& detail) {
    if (amount <= Money{} || amount > balance_) {
        return false;
    }
    balance_ -= amount;
    record(TxType::Withdrawal, amount, detail);
    return true;
}

bool Account::transferOut(Money amount, const std::string& detail) {
    if (amount <= Money{} || amount > balance_) {
        return false;
    }
    balance_ -= amount;
    record(TxType::TransferOut, amount, detail);
    return true;
}

void Account::credit(Money amount, const std::string& detail) {
    if (amount > Money{}) {
        balance_ += amount;
        record(TxType::TransferIn, amount, detail);
    }
}

bool Account::payBill(Money amount, const std::string& billName,
                      const std::string& yearMonth) {
    if (amount <= Money{} || amount > balance_) {
        return false;
    }
    balance_ -= amount;
    record(TxType::BillPayment, amount, billName);
    billsPaid_[billName] = yearMonth;
    return true;
}

bool Account::hasPaidBill(const std::string& billName,
                          const std::string& yearMonth) const {
    auto it = billsPaid_.find(billName);
    return it != billsPaid_.end() && it->second == yearMonth;
}

}  // namespace atm
