#include "Money.hpp"

#include <cmath>
#include <sstream>

namespace atm {

Money Money::fromRupees(double rupees) {
    // Round to the nearest paisa to avoid representation surprises.
    return Money{static_cast<std::int64_t>(std::llround(rupees * 100.0))};
}

std::string Money::format() const {
    std::int64_t value = paisa_;
    const bool negative = value < 0;
    if (negative) value = -value;

    const std::int64_t rupees = value / 100;
    const std::int64_t paise = value % 100;

    // Group the rupee part with thousands separators.
    std::string digits = std::to_string(rupees);
    std::string grouped;
    int count = 0;
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        if (count != 0 && count % 3 == 0) grouped.push_back(',');
        grouped.push_back(*it);
        ++count;
    }
    std::string rupeeStr(grouped.rbegin(), grouped.rend());

    std::ostringstream out;
    if (negative) out << '-';
    out << "Rs " << rupeeStr << '.';
    if (paise < 10) out << '0';
    out << paise;
    return out.str();
}

}  // namespace atm
