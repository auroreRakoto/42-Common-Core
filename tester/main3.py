#!/usr/bin/env python3
import subprocess
import sys
import os

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
RESET = "\033[0m"

def banner(msg, char="="):
    print(f"\n{YELLOW}{char * 50}{RESET}")
    print(f"{YELLOW}{msg.center(50)}{RESET}")
    print(f"{YELLOW}{char * 50}{RESET}\n")

def run_test(name, command, expected_outputs):
    try:
        result = subprocess.check_output(command, shell=True, text=True).strip()
    except subprocess.CalledProcessError as e:
        result = e.output.strip()

    trace = []
    passed = 0
    for expected in expected_outputs:
        if expected == result:
            trace.append(GREEN + "✅" + RESET)
            passed += 1
        else:
            trace.append(RED + "❌" + RESET)

    print(f"{name} : {''.join(trace)}")
    return passed, len(expected_outputs)

def load_libft_tests(filename="libft_data.txt"):
    tests = []
    current_fn = None
    with open(filename, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if line.startswith("[") and line.endswith("]"):
                current_fn = line[1:-1]
                continue
            if current_fn:
                try:
                    input_str, expected = line.split("|", 1)
                    exe = "libft_test.exe" if os.name == "nt" else "./libft_test"
                    cmd = f'{exe} {current_fn} "{input_str}"'

                    tests.append((current_fn, cmd, [expected]))
                except ValueError:
                    print(f"Skipping malformed line: {line}")
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
        test_list = load_libft_tests()
    else:
        print(f"No tests configured for {project}")
        return

    # Group by function name
    grouped = {}
    for name, cmd, expected in test_list:
        grouped.setdefault(name, []).append((cmd, expected))

    for name, cases in grouped.items():
        trace = []
        passed = 0
        for cmd, expected in cases:
            ok, total = run_test(name, cmd, expected)
            passed += ok
            trace.append("".join([GREEN+"✅"+RESET if ok else RED+"❌"+RESET for ok in [ok==1]]))
        print(f"{name} : {''.join(trace)}")
        total_passed += passed
        total_tests += len(cases)

    banner("📊 RESULT SUMMARY")
    print(f"Passed: {GREEN}{total_passed}{RESET}/{total_tests}")

if __name__ == "__main__":
    main()
