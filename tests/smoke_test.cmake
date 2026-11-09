# End-to-end smoke test: drives the real ATM program with scripted keyboard input
# and checks what it prints. Run through CTest:  ctest --test-dir build
#
# Expects -DATM=<path to the atm executable> and -DWORK=<empty scratch folder>.

file(REMOVE_RECURSE "${WORK}")
file(MAKE_DIRECTORY "${WORK}")

set(failures 0)

# run_session(<name> <input text> <expected text>...)
function(run_session name input)
  file(WRITE "${WORK}/${name}.in" "${input}")
  execute_process(
    COMMAND "${ATM}"
    WORKING_DIRECTORY "${WORK}"
    INPUT_FILE "${WORK}/${name}.in"
    OUTPUT_VARIABLE out
    ERROR_VARIABLE out
    RESULT_VARIABLE code
    TIMEOUT 20)
  if(NOT code EQUAL 0)
    message(SEND_ERROR "[${name}] exited with ${code}")
  endif()
  foreach(expected IN LISTS ARGN)
    string(FIND "${out}" "${expected}" at)
    if(at EQUAL -1)
      message(SEND_ERROR "[${name}] expected to see: ${expected}")
    else()
      message(STATUS "[${name}] ok: ${expected}")
    endif()
  endforeach()
endfunction()

# 1. Create an account, then deposit, withdraw, try to overdraw, send money and view history.
run_session(new_account
  "2\nalice\n1234\n\nalice\n1234\n2\n500.50\n\n3\n200\n\n3\n99999\n\n4\n1001\n100\n\n5\n\n7\n"
  "Account created! Your account number is 1003."
  "Deposited Rs 500.50. New balance: Rs 500.50"
  "Withdrew Rs 200.00. New balance: Rs 300.50"
  "amount exceeds your available balance"
  "Sent Rs 100.00 to account #1001."
  "Current balance: Rs 200.50")

# 2. The recipient sees the money; a bill can be paid only once per month.
run_session(recipient_and_bills
  "1\nbob\n0000\n5\n\n6\n1\n1\n\n6\n1\n1\n\n7\n"
  "+Rs 100.00  RECEIVED  (from alice (#1003))"
  "Current balance: Rs 50,100.00"
  "Paid your electricity bill"
  "You have already paid your electricity bill this month.")

# 3. Three wrong PINs lock the session.
run_session(lockout
  "1\nbob\n1111\n\nbob\n2222\n\nbob\n3333\n\n"
  "Incorrect credentials -- attempt 3 of 3."
  "LIMIT EXCEEDED -- access locked.")
