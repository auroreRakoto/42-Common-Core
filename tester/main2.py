#!/usr/bin/env python3
import subprocess
import sys

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
RESET = "\033[0m"

def banner(msg, char="="):
    print(f"\n{YELLOW}{char * 50}{RESET}")
    print(f"{YELLOW}{msg.center(50)}{RESET}")
    print(f"{YELLOW}{char * 50}{RESET}\n")

def run_test(name, command, expected_outputs):
    """
    Run a single test.
    - name: test name
    - command: command to run
    - expected_outputs: list of expected outputs (strings)
    """
    try:
        result = subprocess.check_output(command, shell=True, text=True).strip()
    except subprocess.CalledProcessError as e:
        result = e.output.strip()

    trace = []
    passed = 0

    for expected in expected_outputs:
        if expected in result:
            trace.append(GREEN + "✅" + RESET)
            passed += 1
        else:
            trace.append(RED + "❌" + RESET)

    print(f"{name} : {''.join(trace)}")
    return passed, len(expected_outputs)

def libft_tests():
    tests = []
    # Example: run your libft binary with different inputs
    tests.append(("ft_atoi", "./a.out", ["42", "-123", "0", "999"]))
    tests.append(("ft_strlen", "./a.out", ["5", "0", "3"]))  # fake expected outputs
    return tests

def main():
    if len(sys.argv) < 2:
        print("Usage: main.py [libft|philo|gnl]")
        return

    project = sys.argv[1]
    total_passed = 0
    total_tests = 0

    banner(f"STARTING TESTS for {project.upper()}")

    if project == "libft":
        test_list = libft_tests()
    else:
        print(f"No tests configured for {project}")
        return

    for (name, cmd, expected) in test_list:
        passed, total = run_test(name, cmd, expected)
        total_passed += passed
        total_tests += total

    banner("📊 RESULT SUMMARY")
    print(f"Passed: {GREEN}{total_passed}{RESET}/{total_tests}")

if __name__ == "__main__":
    main()
