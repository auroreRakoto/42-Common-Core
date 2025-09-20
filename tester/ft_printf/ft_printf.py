import os
from pathlib import Path

ROOT = Path(__file__).resolve().parent

GREEN = "\033[32m"
RED   = "\033[31m"
RESET = "\033[0m"

def load_printf_tests(filename="data/printf_data.txt"):
	tests = []
	data_file = (ROOT / filename).resolve()
	if not data_file.exists():
		print(f"{RED}❌ Test data file not found: {data_file}{RESET}")
		return tests


	exe_path = ROOT / ("printf_test.exe" if os.name == "nt" else "printf_test")
	if not exe_path.exists():
		print(f"{RED}❌ printf test harness not found at {exe_path}. Build failed?{RESET}")
		return tests

	exe = f'"{str(exe_path)}"'
	current_fn = None

	for raw in data_file.read_text(encoding="utf-8").splitlines():
		line = raw.strip()
		if not line or line.startswith("#"):
			continue
		if line.startswith("[") and line.endswith("]"):
			current_fn = line[1:-1].strip()
			continue

		if current_fn:
			parts = line.split("|")
			if len(parts) < 2:
				continue
			fmt = parts[0]
			args = parts[1:-1]
			expected = parts[-1]
			# build command
			fmt_clean = fmt.strip('"')
			cmd = f'{exe} "{fmt_clean}"'
			for a in args:
				cmd += f' "{a}"'
			tests.append((current_fn, cmd, [expected]))
	return tests