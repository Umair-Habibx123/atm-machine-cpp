#pragma once

#include <ctime>
#include <string>

#include "Money.hpp"

namespace atm {

enum class TxType {
    Deposit,
    Withdrawal,
    TransferOut,
    TransferIn,
    BillPayment,
};

// A single line in an account's statement.
struct Transaction {
    std::time_t timestamp{};
    TxType type{};
    Money amount{};
    std::string detail;  // counterparty name/account, or bill name
};

inline const char* toString(TxType type) {
    switch (type) {
        case TxType::Deposit:     return "DEPOSIT";
        case TxType::Withdrawal:  return "WITHDRAWAL";
        case TxType::TransferOut: return "SENT";
        case TxType::TransferIn:  return "RECEIVED";
        case TxType::BillPayment: return "BILL";
    }
    return "UNKNOWN";
}

inline bool parseTxType(const std::string& text, TxType& out) {
    if (text == "DEPOSIT")     { out = TxType::Deposit;     return true; }
    if (text == "WITHDRAWAL")  { out = TxType::Withdrawal;  return true; }
    if (text == "SENT")        { out = TxType::TransferOut; return true; }
    if (text == "RECEIVED")    { out = TxType::TransferIn;  return true; }
    if (text == "BILL")        { out = TxType::BillPayment; return true; }
    return false;
}

// Money flows out of the account for these; in for the rest. Used so the
// statement can show a +/- sign.
inline bool isDebit(TxType type) {
    return type == TxType::Withdrawal || type == TxType::TransferOut ||
           type == TxType::BillPayment;
}

}  // namespace atm
