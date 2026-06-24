#include "Atm.hpp"
#include "Bank.hpp"

// Build the persistent bank (loads from disk, or seeds demo accounts on first
// run), then run one ATM session against it. Every detail lives in a small,
// single-responsibility unit -- a long way from the one 173-line main() this
// started as. See legacy/ATM.cpp for where it began.
int main() {
    atm::Bank bank{"atm_data.db"};
    atm::Atm atm{bank};
    atm.run();
    return 0;
}
