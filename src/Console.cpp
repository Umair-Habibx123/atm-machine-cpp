#include "Console.hpp"

#include <cctype>
#include <iostream>
#include <string>

#if defined(_WIN32)
#include <conio.h>
#include <io.h>
#include <cstdio>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace atm::console {

namespace {
bool stdinIsTerminal() {
#if defined(_WIN32)
    return _isatty(_fileno(stdin)) != 0;
#else
    return isatty(STDIN_FILENO) != 0;
#endif
}
}  // namespace

void clear() {
    // ANSI escape: move cursor home and clear screen. Works on modern Windows
    // terminals (Windows 10+), macOS, and Linux without spawning a subprocess.
    std::cout << "\033[2J\033[H" << std::flush;
}

void pause() {
    std::cout << "\n\t\tPress Enter to continue...";
    std::string discard;
    std::getline(std::cin, discard);
}

int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            return 0;  // EOF: treat as a quiet exit signal.
        }
        try {
            std::size_t pos = 0;
            int value = std::stoi(line, &pos);
            // Reject trailing garbage like "12abc".
            while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
            if (pos == line.size()) return value;
        } catch (...) {
            // fall through to the retry message
        }
        std::cout << "\t\t>> Please enter a valid whole number.\n";
    }
}

Money readAmount(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return Money{};
        try {
            std::size_t pos = 0;
            double rupees = std::stod(line, &pos);
            while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
            if (pos == line.size() && rupees > 0.0) {
                return Money::fromRupees(rupees);
            }
        } catch (...) {
        }
        std::cout << "\t\t>> Please enter a positive amount.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

std::string readSecret(const std::string& prompt) {
    std::cout << prompt;
    std::string secret;

    // When input is piped or redirected (scripts, tests, CI), there's no keyboard to
    // mask, so read a normal line. _getch() would ignore the pipe and wait forever.
    if (!stdinIsTerminal()) {
        std::getline(std::cin, secret);
        std::cout << '\n';
        return secret;
    }

#if defined(_WIN32)
    int ch;
    while ((ch = _getch()) != '\r' && ch != '\n' && ch != EOF) {
        if (ch == '\b') {  // backspace
            if (!secret.empty()) {
                secret.pop_back();
                std::cout << "\b \b";
            }
        } else {
            secret.push_back(static_cast<char>(ch));
            std::cout << '*';
        }
    }
    std::cout << '\n';
#else
    termios oldt{};
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~static_cast<tcflag_t>(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::getline(std::cin, secret);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << '\n';
#endif

    return secret;
}

}  // namespace atm::console
