cmake_minimum_required(VERSION 3.15)
project(atm-machine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(atm
    src/main.cpp
    src/Atm.cpp
    src/Bank.cpp
    src/Account.cpp
    src/Money.cpp
    src/Console.cpp
)

if(MSVC)
    target_compile_options(atm PRIVATE /W4)
else()
    target_compile_options(atm PRIVATE -Wall -Wextra -Wpedantic)
endif()
