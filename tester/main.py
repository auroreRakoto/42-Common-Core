#!/usr/bin/env python3
import os
import sys
import time
import shutil
import subprocess
import datetime
import shlex
from pathlib import Path
from libft.libft import load_libft_tests, run_libft_test
from ft_printf.ft_printf import load_printf_tests

# --- console encoding + glyphs fallback ---
def _choose_glyphs():
	ok, fail = "✅", "❌"
	enc = (sys.stdout.encoding or "").lower()
	try:
		(ok + fail).encode(enc or "utf-8")
		return ok, fail
	except Exception:
		return "[OK]", "[FAIL]"

# Try to switch to UTF-8; if not possible, we’ll still fall back to ASCII.
try:
	if (sys.stdout.encoding or "").lower() != "utf-8":
		sys.stdout.reconfigure(encoding="utf-8")
except Exception:
	pass

OK_GLYPH, FAIL_GLYPH = _choose_glyphs()

# Paths
ROOT = Path(__file__).resolve().parent       # .../tester
PROJECTS_ROOT = ROOT.parent                  # .../Common_Core
LOG_DIR = (ROOT / "logs").resolve()
LOG_DIR.mkdir(exist_ok=True)

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


def new_log_file(project: str) -> Path:
	ts = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
	return LOG_DIR / f"{project}_tests_{ts}.log"

def log_write(fp: Path, text: str):
	with fp.open("a", encoding="utf-8") as f:
		f.write(text + "\n")

def _truncate_cell(s: str, n: int) -> str:
	s = "" if s is None else str(s)
	s = s.replace("\r", "␍").replace("\n", "␤")  # keep rows one-line
	return s if len(s) <= n else s[: max(0, n - 1)] + "…"

# Fixed widths for columns: FUNCTION | INPUT | EXPECTED | GOT | PASS
COLW = (14, 36, 28, 28, 6)

def format_row(fn: str, inp: str, exp: str, got: str, status: str) -> str:
	cols = (fn, inp, exp, got, status)
	cells = [_truncate_cell(c, w) for c, w in zip(cols, COLW)]
	return " | ".join(c.ljust(w) for c, w in zip(cells, COLW))



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

# ---- GNL fixtures & parsing ----
def _gnl_make_fixtures(root: Path) -> dict:
	fx = {}
	data_dir = root / "tmp_gnl"
	data_dir.mkdir(exist_ok=True)
	cases = {
		"empty.txt": "",
		"one.txt": "hello\n",
		"multi.txt": "a\nbb\nccc\n",
		"no_newline.txt": "last line without newline",
		"longline.txt": "X" * 100_000 + "\n",  # long line > typical BUFF_SIZE
	}
	for name, content in cases.items():
		p = data_dir / name
		p.write_text(content, encoding="utf-8")
		fx[name] = (p, content)
	return fx

def _gnl_collect_lines(output: str, tag="L"):
	"""Parse lines like 'L:<content>' -> returns list[str]"""
	lines = []
	for raw in output.splitlines():
		if raw.startswith(tag + ":"):
			lines.append(raw[len(tag) + 1:])  # after "L:"
	return lines


def run_project_tests(project: str, harness: str | None = None):
	total_passed = 0
	total_tests  = 0

	# create a new log file for this run
	log_path = new_log_file(project)
	log_write(log_path, f"=== {project.upper()} TEST RUN @ {datetime.datetime.now()} ===")
	log_write(log_path, format_row("FUNCTION", "INPUT", "EXPECTED", "GOT", "PASS"))
	log_write(log_path, format_row("-"*8, "-"*5, "-"*8, "-"*3, "----"))

	exe_name = (project + "/" + harness) or (project + "/" + project + "_test")
	exe_path = ROOT / (exe_name + (".exe" if os.name == "nt" else ""))

	is_bonus = exe_path.name.startswith(project + "_test_bonus")
	data_file = ("../data/" + project + "_bonus_data.txt") if is_bonus else ("../data/" + project + "_data.txt")

	if project == "libft":	
		test_list = load_libft_tests(data_file, exe_path=exe_path)
		grouped = {}
		for name, cmd, expected in test_list:
			grouped.setdefault(name, []).append((cmd, expected))

		for name, cases in grouped.items():
			line_passed = 0
			line_total  = 0
			glyphs = []

			for cmd, expected in cases:
				if isinstance(cmd, list):
					input_arg = cmd[-1]
				else:
					input_arg = cmd.rsplit(" ", 1)[-1].strip()
					if input_arg.startswith('"') and input_arg.endswith('"'):
					    input_arg = input_arg[1:-1]

				p, t, trace, got = run_libft_test(cmd, expected)
				line_passed += p
				line_total  += t
				glyphs.append(trace)

				pass_flag = "PASS" if p == t else "FAIL"
				log_write(log_path, format_row(name, input_arg, expected[0], got, pass_flag))

			# console: single grouped line with only ticks
			print(f"{name} : {''.join(glyphs)}")

			total_passed += line_passed
			total_tests  += line_total


	elif project == "ft_printf":
		test_list = load_printf_tests(data_file)
		grouped = {"ft_printf": []}
		for name, cmd, expected in test_list:
			grouped["ft_printf"].append((cmd, expected))

		for name, cases in grouped.items():
			line_passed = 0
			line_total = 0
			glyphs = []

			for cmd, expected in cases:
				if isinstance(cmd, list):
					input_arg = cmd[-1]
				else:
					input_arg = cmd.rsplit(" ", 1)[-1].strip()
					if input_arg.startswith('"') and input_arg.endswith('"'):
					    input_arg = input_arg[1:-1]

				p, t, trace, got = run_libft_test(cmd, expected)
				line_passed += p; line_total += t; glyphs.append(trace)

				pass_flag = "PASS" if p == t else "FAIL"
				log_write(log_path, format_row(name, input_arg, expected[0], got, pass_flag))
				#log_write(log_path, f"{name} | {expected[0]} | {got} | {'PASS' if p==t else 'FAIL'}")
			print(f"{name} : {''.join(glyphs)}")
			total_passed += line_passed; total_tests += line_total


	elif project in ("get_next_line", "gnl"):
		# locate built harnesses
		tester_bin_dir = ROOT
		bins = [b for b in ["gnl_test_bs_1", "gnl_test_bs_32", "gnl_test_bs_9999"]
				if (tester_bin_dir / b).exists()]
		if not bins:
			print(f"{RED}❌ No gnl_test binaries found. Did the Makefile build run?{RESET}")
			return 0, 0

		fixtures = _gnl_make_fixtures(ROOT)
		for b in bins:
			print(f"\nTesting with {YELLOW}{b}{RESET}")
			for name, (path, content) in fixtures.items():
				# run in "file" mode
				cmd = f'"{tester_bin_dir / b}" file "{path}"'
				p, t, trace, got = run_libft_test(cmd, expected_outputs=["<ignore>"])  # we'll compute verdict ourselves
				got_lines = _gnl_collect_lines(got, tag="L")
				reconstructed = "\n".join(got_lines)
				# If original file ended with '\n', add it back for full equality
				ends_nl = content.endswith("\n")
				compare_to = reconstructed + ("\n" if ends_nl else "")
				ok = (compare_to == content)

				glyph = (GREEN + "✅" + RESET) if ok else (RED + "❌" + RESET)
				print(f"{name} : {glyph}")
				#log_write(log_path, format_row(name, input_arg, expected[0], got, pass_flag))
				log_write(log_path,
						  f"{b} file {name} | EXPECT:len={len(content)} | GOT:len={len(compare_to)} | {'PASS' if ok else 'FAIL'}")
				total_tests += 1
				if ok: total_passed += 1

			# One multi-fd alternation check (multi.txt + no_newline.txt)
			p1, c1 = fixtures["multi.txt"]
			p2, c2 = fixtures["no_newline.txt"]
			cmd = f'"{tester_bin_dir / b}" multifile "{p1}" "{p2}"'
			p, t, trace, got = run_libft_test(cmd, expected_outputs=["<ignore>"])
			got1 = "\n".join(_gnl_collect_lines(got, tag="1"))
			got2 = "\n".join(_gnl_collect_lines(got, tag="2"))
			ok1 = (got1 + "\n" == c1)   # multi.txt ends with '\n'
			ok2 = (got2 == c2)          # no_newline.txt has no trailing '\n'
			ok = ok1 and ok2
			glyph = (GREEN + "✅" + RESET) if ok else (RED + "❌" + RESET)
			print(f"multifile(multi,no_newline) : {glyph}")
			#log_write(log_path, format_row(name, input_arg, expected[0], got, pass_flag))			
			log_write(log_path, f"{b} multifile | {'PASS' if ok else 'FAIL'}")
			total_tests += 1
			if ok: total_passed += 1
			log_write(log_path, format_row(name, input_arg, expected[0], got, pass_flag))


	return total_passed, total_tests


# ---------- Main ----------
def main():
	if len(sys.argv) < 2:
		print("Usage: main.py [libft|philo|gnl]")
		return
	project = sys.argv[1]
	harness = sys.argv[2] if len(sys.argv) > 2 else None

	banner(f"STARTING PRE TESTS for {project.upper()}")

	total_passed, total_tests = 0, 0
	"""
	norm_ok, norm_total = run_norm_tests(project)
	total_passed += norm_ok; total_tests += norm_total
	"""
	#mk_ok, mk_total = run_makefile_tests(project)
	#total_passed += mk_ok; total_tests += mk_total

	banner("📊 RESULT SUMMARY")
	print(f"Passed: {GREEN}{total_passed}{RESET}/{total_tests}")

	#if total_tests > 0 and total_passed != total_tests:
	#    print(f"\n{RED}Pre-tests failed — aborting project tests.{RESET}")
	#    sys.exit(1)

	banner(f"STARTING TESTS for {project.upper()}")
	p_ok, p_total = run_project_tests(project, harness)
	banner("📊 RESULT SUMMARY")
	print(f"Passed: {GREEN}{p_ok}{RESET}/{p_total}")
	# after printing final summary
	print(f"\nLog saved to: {YELLOW}{LOG_DIR}{RESET} (latest file for {project})")


if __name__ == "__main__":
	main()
