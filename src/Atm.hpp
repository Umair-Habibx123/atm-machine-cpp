#pragma once

#include <string>

#include "Bank.hpp"

namespace atm {

// Orchestrates a single ATM session against a persistent Bank: a welcome screen
// (register or log in, with a lockout), then the transaction menu. Presentation
// lives in Console; money rules live in Account/Bank; this class is the glue.
class Atm {
public:
    static constexpr int kMaxLoginAttempts = 3;

    explicit Atm(Bank& bank) : bank_(bank) {}

    void run();

private:
    Account* welcome();           // register or log in; nullptr if the user quits/locks out
    Account* doRegister();
    Account* doLogin();
    void sessionLoop(Account& account);

    void showMenu(const Account& account) const;
    void doCheckBalance(const Account& account) const;
    void doDeposit(Account& account);
    void doWithdraw(Account& account);
    void doSendMoney(Account& account);
    void doHistory(const Account& account) const;
    void doPayBills(Account& account);

    Bank& bank_;
};

}  // namespace atm
