#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

namespace atm {

// FNV-1a, a small, fast, deterministic hash. Deterministic matters: we persist
// the hash of a PIN to disk and must reproduce it on the next run to log a user
// in (std::hash gives no such cross-run guarantee).
//
// NOTE: this is NOT cryptographically secure and is fine only for a learning
// project. A real bank would use a slow, salted KDF (bcrypt/scrypt/argon2).
inline std::uint64_t fnv1aNumber(const std::string& text) {
    std::uint64_t hash = 1469598103934665603ull;  // FNV offset basis
    for (unsigned char c : text) {
        hash ^= c;
        hash *= 1099511628211ull;  // FNV prime
    }
    return hash;
}

inline std::string fnv1aHex(const std::string& text) {
    char buffer[17];
    std::snprintf(buffer, sizeof(buffer), "%016llx",
                  static_cast<unsigned long long>(fnv1aNumber(text)));
    return std::string(buffer);
}

}  // namespace atm
