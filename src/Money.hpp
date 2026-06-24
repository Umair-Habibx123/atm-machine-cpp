#pragma once

#include <cstdint>
#include <string>

namespace atm {

// Money is stored as an integer count of paisa (1/100 of a rupee) so that
// arithmetic is always exact -- no silent truncation or floating-point drift,
// which was a real bug in the original program (a `double` deposit assigned
// into an `int balance`).
class Money {
public:
    constexpr Money() = default;
    constexpr explicit Money(std::int64_t paisa) : paisa_(paisa) {}

    static Money fromRupees(double rupees);

    std::int64_t paisa() const { return paisa_; }
    bool isZero() const { return paisa_ == 0; }

    Money operator+(Money other) const { return Money{paisa_ + other.paisa_}; }
    Money operator-(Money other) const { return Money{paisa_ - other.paisa_}; }
    Money& operator+=(Money other) { paisa_ += other.paisa_; return *this; }
    Money& operator-=(Money other) { paisa_ -= other.paisa_; return *this; }

    bool operator<(Money other) const { return paisa_ < other.paisa_; }
    bool operator<=(Money other) const { return paisa_ <= other.paisa_; }
    bool operator>(Money other) const { return paisa_ > other.paisa_; }
    bool operator==(Money other) const { return paisa_ == other.paisa_; }

    // Formatted as "Rs 1,234.50".
    std::string format() const;

private:
    std::int64_t paisa_ = 0;
};

}  // namespace atm
