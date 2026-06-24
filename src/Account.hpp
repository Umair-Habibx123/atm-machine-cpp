#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "Money.hpp"
#include "Transaction.hpp"

namespace atm {

// A persisted bank account: identity, a hashed PIN, a balance, a full
// transaction history, and a record of which bills were paid in which month.
//
// The class guards the one invariant that matters -- you can never withdraw
// more than you hold -- and records every balance change as a Transaction so
// the statement is always faithful.
class Account {
public:
    // Create a brand-new account from a plaintext PIN (which is hashed and
    // never stored in the clear).
    static Account create(std::int64_t number, std::string username,
                          const std::string& plainPin);

    // Rebuild an account loaded from disk (PIN is already hashed).
    static Account restore(std::int64_t number, std::string username,
                           std::string pinHash, Money balance);

    std::int64_t number() const { return number_; }
    const std::string& username() const { return username_; }
    const std::string& pinHash() const { return pinHash_; }
    Money balance() const { return balance_; }
    const std::vector<Transaction>& history() const { return history_; }
    const std::map<std::string, std::string>& billsPaid() const { return billsPaid_; }

    bool checkPin(const std::string& plainPin) const;

    // Balance-changing operations. Each records a Transaction. Withdraw/transfer
    // return false (changing nothing) on insufficient or non-positive amounts.
    void deposit(Money amount, const std::string& detail);
    bool withdraw(Money amount, const std::string& detail);
    bool transferOut(Money amount, const std::string& detail);  // outgoing transfer
    void credit(Money amount, const std::string& detail);       // incoming transfer
    bool payBill(Money amount, const std::string& billName,
                 const std::string& yearMonth);

    // True if this bill has already been paid in the given "YYYY-MM".
    bool hasPaidBill(const std::string& billName, const std::string& yearMonth) const;

    // Used by the loader to re-attach persisted state without re-running rules.
    void loadTransaction(const Transaction& tx) { history_.push_back(tx); }
    void loadBillPaid(const std::string& billName, const std::string& yearMonth) {
        billsPaid_[billName] = yearMonth;
    }

private:
    Account(std::int64_t number, std::string username, std::string pinHash, Money balance)
        : number_(number),
          username_(std::move(username)),
          pinHash_(std::move(pinHash)),
          balance_(balance) {}

    void record(TxType type, Money amount, const std::string& detail);

    std::int64_t number_;
    std::string username_;
    std::string pinHash_;
    Money balance_;
    std::vector<Transaction> history_;
    std::map<std::string, std::string> billsPaid_;  // billName -> "YYYY-MM"
};

}  // namespace atm
