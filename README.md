<h1 align="center">🏧 ATM Machine — A First-Semester Project, Grown Up</h1>

<p align="center">
  <em>The very first program I was proud of, refactored years later into something I'd actually ship.</em>
</p>

<p align="center">
  <img alt="Language" src="https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white">
  <img alt="Build" src="https://img.shields.io/badge/build-CMake-064F8C?logo=cmake&logoColor=white">
  <img alt="Platform" src="https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-555">
  <img alt="Status" src="https://img.shields.io/badge/status-nostalgic%20%26%20maintained-success">
  <a href="https://github.com/Umair-Habibx123/atm-machine-cpp/actions/workflows/ci.yml"><img alt="CI" src="https://github.com/Umair-Habibx123/atm-machine-cpp/actions/workflows/ci.yml/badge.svg"></a>
  <a href="LICENSE"><img alt="License: MIT" src="https://img.shields.io/badge/license-MIT-blue"></a>
</p>

---

## 📖 The story

This started life as my **1st-semester C++ project** — a console ATM simulator with
big ASCII banners, a global `int balance`, and a `main()` that did absolutely
everything. It worked, it made me grin, and it was held together with `system("cls")`
and good intentions.

I kept the soul and rebuilt the skeleton. The original is preserved untouched in
[`legacy/ATM.cpp`](legacy/ATM.cpp) as a little museum piece. The version you build
today lives in [`src/`](src/) and keeps **every feature** of the original — just
without the footguns.

---

## ✨ Features

- 🗄️ **Persistent accounts** — a real on-disk database (`atm_data.db`); your
  balance, history, and bills survive between runs
- 🔐 **Sign up & log in** with unique usernames, **hashed PINs**, and a
  3-attempt lockout
- 👁️ **Masked PIN entry** — your secret never echoes to the screen
- 💰 **Check balance**, **deposit**, and **withdraw**
- 📤 **Send money** to another account by number — the recipient is
  **credited instantly** and sees it in their own history next login
- 🧾 **Transaction history** — a full, timestamped statement with running
  `+`/`-` entries
- 🏠 **Real monthly bills** — electricity, water, and gas, each with a bill
  amount that's fixed for the month and **payable only once per calendar month**
- 🧮 Exact money math — stored as integer paisa, no floating-point drift, no
  silent truncation

### 🧪 Demo accounts

On first run the database is seeded with two accounts so **Send Money** works
right away (PIN `0000` for both):

| Account # | Username | Opening balance |
|-----------|----------|-----------------|
| `1001`    | `bob`    | Rs 50,000.00    |
| `1002`    | `carol`  | Rs 12,000.00    |

Create your own account, then send some money to `1001` and log in as `bob` to
watch it arrive.

---

## 🚀 How to run

You need a C++ compiler. If you don't have one yet, install it first (one time),
then jump to whichever version you want to run.

### 0. Install a compiler (one time)

**Windows** — install MinGW-w64 (gives you `g++`) via winget:

```powershell
winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT
```

Or, if you prefer the classic toolchain, install **MSYS2** and run
`pacman -S mingw-w64-ucrt-x86_64-gcc` from its shell. Either way, make sure the
compiler's `bin` folder is on your `PATH`, then **open a new terminal** and check:

```powershell
g++ --version
```

**macOS** — `xcode-select --install` (gives you `clang++`).
**Linux** — `sudo apt install g++` (Debian/Ubuntu) or your distro's equivalent.

---

### 1. Run the modern version (`src/`)

**Option A — plain g++ (no CMake needed):**

```powershell
# from the project root
g++ -std=c++17 -Wall -Wextra src/*.cpp -o atm
./atm           # Windows
# ./atm         # macOS / Linux
```

**Option B — CMake** (if you have it; install on Windows with
`winget install -e --id Kitware.CMake`):

```powershell
cmake -S . -B build
cmake --build build
./build/atm                # macOS / Linux
./build/Debug/atm.exe      # Windows (MSVC)
./build/atm.exe            # Windows (MinGW)
```

**Run the tests** (CMake only). An end-to-end test plays three scripted sessions
against a fresh database: create an account, deposit, withdraw, overdraw, send
money, check the recipient's statement, pay a bill twice, and trigger the lockout.
CI runs it on Windows, macOS and Linux.

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

---

### 2. Run the original legacy version (`legacy/`)

The 1st-semester original is a single file, so it compiles directly. It uses
`system("cls")`, so it's Windows-only — exactly as it was back then:

```powershell
g++ legacy/ATM.cpp -o atm_legacy
./atm_legacy
```

> ⚠️ Heads up: the legacy build will emit warnings (and relies on some
> undefined behavior). That's expected — it's preserved as-is. The modern
> version in `src/` is the one to actually use.

---

## 🗂️ Project layout

```
.
├── src/
│   ├── main.cpp        # Entry point — build an ATM, run a session
│   ├── Atm.{hpp,cpp}   # Session flow: register → login → menu
│   ├── Bank.{hpp,cpp}     # The "database": persistence + transfers + bills
│   ├── Account.{hpp,cpp}  # Balance + history + bills, no-overdraft rule
│   ├── Money.{hpp,cpp}    # Integer-paisa money type (exact arithmetic)
│   ├── Transaction.hpp    # A single statement entry
│   ├── Clock.hpp          # Cross-platform date/time helpers
│   ├── Hash.hpp           # Deterministic PIN hashing (FNV-1a)
│   └── Console.{hpp,cpp}  # Validated, cross-platform terminal I/O
├── tests/
│   └── smoke_test.cmake   # End-to-end test: scripted sessions, checked output
├── legacy/
│   └── ATM.cpp         # The original 1st-semester version, untouched
├── CMakeLists.txt
└── README.md
```

---

## 🔧 What changed, and why

| Then (`legacy/ATM.cpp`) | Now (`src/`) |
|---|---|
| One 173-line `main()` doing everything | Small single-responsibility units |
| Global `int balance` | Each `Account` owns its balance |
| `double` deposits truncated into an `int` | `Money` stored as integer paisa — exact |
| `system("cls")` (Windows-only, spawns a shell) | ANSI screen clear, cross-platform |
| PIN typed in plain sight | Masked, non-echoing PIN entry |
| `cin >> x` with no validation | Re-prompts on bad input, never gets stuck |
| `rand()` never seeded (same "random" bill every run) | Each bill amount is derived from the month, so it stays fixed until paid and changes next month |
| Non-`void` functions returning nothing (UB) | Correct return types throughout |
| Could log in but lockout logic was tangled | Clean 3-attempt lockout |

The behavior you remember is all still here — the refactor is about **how** it
works under the hood, not **what** it does.

---

## 🧭 Roadmap

- [x] Persist accounts to disk so balances survive a restart
- [x] Multiple accounts and real account-to-account transfers
- [x] Transaction history / mini-statement
- [x] Real monthly bills, one payment per month
- [ ] Hash PINs with a real salted KDF (bcrypt/argon2) instead of FNV-1a
- [x] End-to-end test wired into CMake (`ctest`), run in CI on 3 operating systems
- [ ] Unit tests for `Money`, `Account` and `Bank`
- [ ] Export statement to CSV

---

## 📄 License

[MIT](LICENSE) © 2023 [Umair Habib](https://github.com/Umair-Habibx123)

---

<p align="center"><sub>Built first as a student. Rebuilt for the nostalgia. 💙</sub></p>
