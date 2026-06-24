#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "Account.hpp"
#include "Money.hpp"

namespace atm {

// The "real database": owns every account and persists them to a flat file on
// disk, so balances, history, and bills survive between runs. Also the single
// place where money moves between two accounts.
class Bank {
public:
    enum class TransferResult { Ok, InsufficientFunds, RecipientNotFound, SameAccount };
    enum class BillResult { Ok, AlreadyPaidThisMonth, InsufficientFunds };

    // Loads accounts from `path`. If the file does not exist yet, seeds a few
    // demo accounts so transfers work out of the box.
    explicit Bank(std::string path);

    // Registration: usernames must be unique. Returns the new account number,
    // or nullopt if the username is taken.
    std::optional<std::int64_t> registerUser(const std::string& username,
                                              const std::string& plainPin);

    // Login by username + PIN. Returns a pointer into the bank's storage (so
    // changes are reflected and persisted), or nullptr on failure.
    Account* login(const std::string& username, const std::string& plainPin);

    Account* findByNumber(std::int64_t number);

    // Moves money between accounts and records it on both sides at once.
    TransferResult transfer(Account& from, std::int64_t toNumber, Money amount);

    // Pays a utility bill, enforcing one payment per calendar month.
    BillResult payBill(Account& account, const std::string& billName);

    // The (deterministic) amount due for a utility in the current month.
    Money billAmountDue(const std::string& billName) const;

    void save() const;

private:
    void seedDemoAccounts();
    bool load();

    std::string path_;
    std::vector<Account> accounts_;
    std::int64_t nextNumber_ = 1001;
};

}  // namespace atm
