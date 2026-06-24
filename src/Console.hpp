#pragma once

#include <string>

#include "Money.hpp"

// Thin, testable wrapper around terminal I/O. Everything the program needs to
// talk to a human lives here, so the rest of the code never touches std::cin
// or std::cout directly.
namespace atm::console {

// Clears the screen in a cross-platform way (the original used system("cls"),
// which is Windows-only and spawns a shell on every call).
void clear();

void pause();

// Prompts until the user enters a valid integer; keeps asking on bad input
// instead of leaving std::cin in a failed state (a classic beginner trap).
int readInt(const std::string& prompt);

// Reads a whole, positive rupee/paisa amount, re-prompting on garbage.
Money readAmount(const std::string& prompt);

std::string readLine(const std::string& prompt);

// Reads a secret without echoing it to the screen (PINs should not be shoulder
// -surfable). Falls back to plain input if the terminal can't hide it.
std::string readSecret(const std::string& prompt);

}  // namespace atm::console
