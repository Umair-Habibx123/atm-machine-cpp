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

# End-to-end test: runs the program with scripted input. Use: ctest --test-dir build
enable_testing()
add_test(NAME smoke
    COMMAND ${CMAKE_COMMAND}
        -DATM=$<TARGET_FILE:atm>
        -DWORK=${CMAKE_CURRENT_BINARY_DIR}/smoke-work
        -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/smoke_test.cmake)
