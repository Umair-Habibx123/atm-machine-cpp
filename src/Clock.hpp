#pragma once

#include <ctime>
#include <string>

// Tiny cross-platform time helpers. Kept header-only and in one place so the
// rest of the code never has to deal with std::tm directly.
namespace atm::clock {

inline std::time_t now() { return std::time(nullptr); }

inline std::tm toLocal(std::time_t t) {
    std::tm out{};
#if defined(_WIN32)
    localtime_s(&out, &t);
#else
    localtime_r(&t, &out);
#endif
    return out;
}

// "YYYY-MM" -- used to enforce one bill payment per calendar month.
inline std::string yearMonth(std::time_t t) {
    std::tm tm = toLocal(t);
    char buf[8];
    std::strftime(buf, sizeof(buf), "%Y-%m", &tm);
    return buf;
}

inline std::string currentYearMonth() { return yearMonth(now()); }

// "YYYY-MM-DD HH:MM" -- used in the printed transaction statement.
inline std::string formatTimestamp(std::time_t t) {
    std::tm tm = toLocal(t);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &tm);
    return buf;
}

}  // namespace atm::clock
