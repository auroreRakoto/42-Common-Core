# --- ADD BELOW your current imports / constants ---
import os
from pathlib import Path
import glob
import time

ROOT = Path(__file__).resolve().parent
LIBFT_DIR = (ROOT / ".." / "libft").resolve()
LIBFT_A = LIBFT_DIR / "libft.a"

def cmd_out(cmd, cwd=None):
    return subprocess.run(cmd, shell=True, cwd=cwd, text=True,
                          stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

def check_file_exists(path: Path):
    return path.exists()

def any_objects_exist():
    return len(list(LIBFT_DIR.glob("*.o"))) > 0

def run_make_target(target, extra=""):
    # GNU make works on Windows with MSYS/MinGW too.
    return cmd_out(f"make {extra} {target}", cwd=str(LIBFT_DIR))

def run_makefile_tests():
    print("\nMakefile checks:")
    ok, total = 0, 0

    def tick(passed, label, details=""):
        nonlocal ok, total
        total += 1
        if passed:
            ok += 1
            print(f"{GREEN}✅{RESET} {label}{(' - ' + details) if details else ''}")
        else:
            print(f"{RED}❌{RESET} {label}{(' - ' + details) if details else ''}")

    # 1) $(NAME) builds libft.a
    out = run_make_target("re")
    tick(LIBFT_A.exists(), "Builds libft.a", "target: re")
    # 2) No relink when nothing changed
    before_time = LIBFT_A.stat().st_mtime if LIBFT_A.exists() else 0
    out2 = run_make_target("all")
    no_work_msg = "Nothing to be done" in (out2.stdout or "")
    time.sleep(0.2)  # ensure FS timestamp granularity
    after_time = LIBFT_A.stat().st_mtime if LIBFT_A.exists() else 0
    no_relink = no_work_msg or (after_time == before_time)
    tick(no_relink, "No relink on second make all")

    # 3) clean removes .o files
    _ = run_make_target("clean")
    tick(not any_objects_exist(), "clean removes .o")

    # 4) fclean removes libft.a
    _ = run_make_target("fclean")
    tick(not LIBFT_A.exists(), "fclean removes libft.a")

    # 5) rebuild after fclean works
    out = run_make_target("all")
    tick(LIBFT_A.exists(), "Rebuild after fclean")

    # 6) Flags (-Wall -Wextra -Werror) present in compile lines (dry-run)
    _ = run_make_target("clean")
    dry = cmd_out("make -n all", cwd=str(LIBFT_DIR))  # --dry-run
    # Look for any compile line; if none, force a real small build:
    compile_lines = [line for line in (dry.stdout or "").splitlines() if "gcc" in line or "cc " in line]
    has_flags = all(flag in " ".join(compile_lines) for flag in ["-Wall", "-Wextra", "-Werror"]) if compile_lines else False
    if not has_flags:
        # fallback: force a real build and scan stdout
        real = run_make_target("re")
        comp = [line for line in (real.stdout or "").splitlines() if "gcc" in line or "cc " in line]
        has_flags = all(flag in " ".join(comp) for flag in ["-Wall", "-Wextra", "-Werror"]) if comp else False
    tick(has_flags, "Compile uses -Wall -Wextra -Werror")

    # 7) bonus rule exists and runs (optional; won’t fail hard if absent)
    bonus_try = run_make_target("bonus")
    bonus_ok = bonus_try.returncode == 0
    tick(bonus_ok, "bonus target runs")

    # Summary line for Makefile tests
    print(f"Makefile: {GREEN}{ok}{RESET}/{total} passed\n")
    return ok, total
