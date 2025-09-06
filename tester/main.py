#!/usr/bin/env python3
import os
import sys
import time
import shutil
import subprocess
from pathlib import Path




# Paths
ROOT = Path(__file__).resolve().parent       # .../tester
PROJECTS_ROOT = ROOT.parent                  # .../Common_Core

# Colors / UI
GREEN = "\033[92m"; RED = "\033[91m"; YELLOW = "\033[93m"; RESET = "\033[0m"
def banner(msg, char="="):
    print(f"\n{YELLOW}{char*50}{RESET}")
    print(f"{YELLOW}{msg.center(50)}{RESET}")
    print(f"{YELLOW}{char*50}{RESET}\n")

def cmd_out(cmd, cwd=None):
    return subprocess.run(cmd, shell=True, cwd=cwd, text=True,
                          stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

def has_makefile(project_dir: Path) -> bool:
    for name in ("Makefile", "makefile", "GNUmakefile"):
        if (project_dir / name).exists():
            return True
    return False

# ---------- Norm ----------
def _find_norm_command():
    # Support your alias: you aliased 'norm' to norminette.exe in zsh,
    # but aliases don't apply in Python. Try both.
    if shutil.which("norminette"):
        return "norminette"
    if shutil.which("norm"):
        return "norm"
    return None

def run_norm_tests(project: str):
    project_dir = (PROJECTS_ROOT / project).resolve()
    if not has_makefile(project_dir):
        print(f"⚠️  No Makefile in {project_dir}, skipping Norm (and Makefile) pre-tests.")
        return 0, 0

    norm_cmd = _find_norm_command()
    if not norm_cmd:
        print(f"⚠️  Norminette not found in PATH, skipping Norm test.")
        return 0, 0

    print("🔎 Testing Norminette")
    res = cmd_out(f"{norm_cmd} -R CheckForbiddenSourceHeader", cwd=str(project_dir))
    out = (res.stdout or "")
    errors = out.count("Error:")
    warns  = out.count("Warning:")
    if errors == 0 and warns == 0:
        print(f"{GREEN}✅{RESET} Norminette clean")
        return 1, 1
    else:
        print(f"{RED}❌{RESET} Norminette: {errors} error(s), {warns} warning(s)")
        return 0, 1

# ---------- Makefile tests (generic) ----------
def run_makefile_tests(project: str):
    project_dir = (PROJECTS_ROOT / project).resolve()
    if not has_makefile(project_dir):
        print(f"⚠️  No Makefile in {project_dir}, skipping Makefile tests.")
        return 0, 0

    print(f"🔎 Testing Makefile in {project_dir}")
    ok, total = 0, 0
    def tick(passed, label, details=""):
        nonlocal ok, total
        total += 1
        print((f"{GREEN}✅{RESET}" if passed else f"{RED}❌{RESET}"), label + ("" if not details else f" - {details}"))
        if passed: ok += 1

    # Guess output: lib (*.a) preferred (libft), else a.out
    libs = list(project_dir.glob("*.a"))
    target = libs[0] if libs else (project_dir / "a.out")

    # 1) make re builds output
    _ = cmd_out("make re", cwd=str(project_dir))
    tick(target.exists(), "make re builds output")

    # 2) make all should not relink if unchanged
    before = target.stat().st_mtime if target.exists() else 0
    out_all = cmd_out("make all", cwd=str(project_dir))
    time.sleep(0.2)
    after = target.stat().st_mtime if target.exists() else 0
    no_relink = "Nothing to be done" in (out_all.stdout or "") or (after == before)
    tick(no_relink, "make all does not relink if unchanged")

    # 3) clean removes .o files
    _ = cmd_out("make clean", cwd=str(project_dir))
    objs = list(project_dir.glob("*.o"))
    tick(len(objs) == 0, "make clean removes .o files")

    # 4) fclean removes output
    _ = cmd_out("make fclean", cwd=str(project_dir))
    tick(not target.exists(), "make fclean removes output")

    # 5) rebuild works
    _ = cmd_out("make all", cwd=str(project_dir))
    tick(target.exists(), "make all rebuilds after fclean")

    # 6) cleaning works
    _ = cmd_out("make clean", cwd=project_dir)
    tick(len(objs) == 0, "make clean ready to test")

    return ok, total

# ---------- Project tests (libft) ----------
def run_test(label: str, command: str, expected_outputs):
    try:
        result = subprocess.check_output(command, shell=True, text=True).strip()
    except subprocess.CalledProcessError as e:
        result = (e.output or "").strip()

    trace = []; passed = 0
    for expected in expected_outputs:
        if expected == result:
            trace.append(GREEN + "✅" + RESET); passed += 1
        else:
            trace.append(RED + "❌" + RESET)
    print(f"{label} : {''.join(trace)}")
    return passed, len(expected_outputs)

def load_libft_tests(filename="libft_data.txt"):
    tests = []
    current_fn = None
    data_file = (ROOT / filename).resolve()

    if not data_file.exists():
        print(f"{RED}❌ Test data file not found: {data_file}{RESET}")
        return tests

    # Absolute path to the harness (works from anywhere)
    exe_path = ROOT / ("libft_test.exe" if os.name == "nt" else "libft_test")
    exe = f'"{str(exe_path)}"'  # quote for spaces in path

    for raw in data_file.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue

        # section header: [ft_atoi], [ft_strlen], ...
        if line.startswith("[") and line.endswith("]"):
            current_fn = line[1:-1].strip()
            continue

        # test line inside a section: input|expected
        if current_fn:
            if "|" not in line:
                # malformed test line, skip quietly
                continue
            input_str, expected = [part.strip() for part in line.split("|", 1)]

            # if user already quoted the input (e.g. ""), keep as-is; else quote it
            if input_str.startswith('"') and input_str.endswith('"'):
                arg = input_str  # already quoted in the file
            else:
                arg = f'"{input_str}"'

            cmd = f'{exe} {current_fn} {arg}'
            tests.append((current_fn, cmd, [expected]))
    return tests


def run_project_tests(project: str):
    total_passed = 0; total_tests = 0
    if project == "libft":
        # verify harness exists
        exe_path = ROOT / ("libft_test.exe" if os.name == "nt" else "libft_test")
        if not exe_path.exists():
            print(f"{RED}❌ libft test harness not found at {exe_path}. Build failed or missing Makefile in tester.{RESET}")
            return 0, 0

        # group by function
        test_list = load_libft_tests("libft_data.txt")
        grouped = {}
        for name, cmd, expected in test_list:
            grouped.setdefault(name, []).append((cmd, expected))

        for name, cases in grouped.items():
            line_passed = 0; line_total = 0; glyphs = []
            for cmd, expected in cases:
                p, t = run_test(name, cmd, expected)
                line_passed += p; line_total += t
                glyphs.append(GREEN+"✅"+RESET if p == t else RED+"❌"+RESET)
            print(f"{name} : {''.join(glyphs)}")
            total_passed += line_passed; total_tests += line_total

    elif project == "philo":
        print("ℹ️ Project tests for philo not set up yet.")
    elif project == "gnl":
        print("ℹ️ Project tests for get_next_line not set up yet.")
    else:
        print(f"ℹ️ No project tests configured for '{project}'")
    return total_passed, total_tests

# ---------- Main ----------
def main():
    if len(sys.argv) < 2:
        print("Usage: main.py [libft|philo|gnl]")
        return

    project = sys.argv[1]
    banner(f"STARTING PRE TESTS for {project.upper()}")

    total_passed, total_tests = 0, 0

    norm_ok, norm_total = run_norm_tests(project)
    total_passed += norm_ok; total_tests += norm_total

    mk_ok, mk_total = run_makefile_tests(project)
    total_passed += mk_ok; total_tests += mk_total

    banner("📊 RESULT SUMMARY")
    print(f"Passed: {GREEN}{total_passed}{RESET}/{total_tests}")

    if total_tests > 0 and total_passed != total_tests:
        print(f"\n{RED}Pre-tests failed — aborting project tests.{RESET}")
        sys.exit(1)

    banner(f"STARTING TESTS for {project.upper()}")
    p_ok, p_total = run_project_tests(project)
    banner("📊 RESULT SUMMARY")
    print(f"Passed: {GREEN}{p_ok}{RESET}/{p_total}")

if __name__ == "__main__":
    main()
