# libft.py
from __future__ import annotations

from pathlib import Path
import os
import shlex
import subprocess
from typing import Iterable, List, Sequence, Tuple, Union

ROOT = Path(__file__).resolve().parent

GREEN = "\033[32m"
RED   = "\033[31m"
RESET = "\033[0m"

Cmd = Union[str, Sequence[str]]

def _unicode_unescape(s: str) -> str:
    """Interpret backslash escapes like \\t and \\n in expectations."""
    return bytes(s, "utf-8").decode("unicode_escape")

def _run(command: Cmd) -> str:
    """
    Run the command and return stdout (text). For argv lists, use shell=False.
    For strings, use shell=True on POSIX so quoted args are respected.
    """
    if isinstance(command, (list, tuple)):
        cp = subprocess.run(command, capture_output=True, text=True, shell=False)
    else:
        cp = subprocess.run(command, capture_output=True, text=True, shell=(os.name != "nt"))
    return cp.stdout or ""

def run_libft_test(command: Cmd, expected_outputs: Iterable[str]) -> Tuple[int, int, str, str]:
    """
    Execute the harness once and compare the single output against multiple
    expected variants. Returns (passed_count, total_count, glyphs, actual_output).
    """
    try:
        fn = ""
        if isinstance(command, (list, tuple)):
            argv = list(command)
            fn = argv[1] if len(argv) > 1 else ""
        out = _run(command)
    except subprocess.CalledProcessError as e:
        out = (getattr(e, "output", "") or "")
    except Exception:
        out = ""

    # Normalise newlines
    out = out.replace("\r\n", "\n")

    # For ft_put* with argv form, keep exact newlines; otherwise trim
    if not (isinstance(command, (list, tuple)) and fn.startswith("ft_put")):
        out = out.strip()

    result = out
    passed = 0
    glyphs: List[str] = []
    for expected in expected_outputs:
        expected_list = expected.split("|")
        result_list   = result.split("|")
        if result_list == expected_list:
            passed += 1
            glyphs.append(f"{GREEN}✅{RESET}")
        else:
            glyphs.append(f"{RED}❌{RESET}")
    return passed, len(list(expected_outputs)), "".join(glyphs), result


def load_libft_tests(filename: str = "../data/libft_data.txt",
                     exe_path: Path | None = None) -> List[Tuple[str, Cmd, List[str]]]:
    """
    Parse a test data file and return a list of (fn_name, cmd, [expected]) tuples.
    Special-cases:
      - ft_split: expected may contain '|' → pass argv list with "s|sep"
      - ft_strjoin: keep expected literal; strip outer quotes on inputs
      - ft_put*: use argv form to preserve exact newlines
    """
    tests: List[Tuple[str, Cmd, List[str]]] = []
    current_fn: str | None = None

    data_file = (ROOT / filename).resolve()
    if not data_file.exists():
        print(f"{RED}❌ Test data file not found: {data_file}{RESET}")
        return tests

    if exe_path is None:
        exe_path = ROOT / ("libft/libft_test.exe" if os.name == "nt" else "libft/libft_test")
    exe = str(exe_path)

    for raw in data_file.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue

        # section header: [ft_...]
        if line.startswith("[") and line.endswith("]"):
            current_fn = line[1:-1].strip()
            continue
        if not current_fn:
            continue

        # ---- ft_split ----
        if current_fn == "ft_split":
            parts = line.split("|")
            if len(parts) < 3:
                continue
            s, sep = parts[0].strip(), parts[1].strip()
            expected_raw = "|".join(parts[2:]).strip()
            expected = _unicode_unescape(expected_raw)
            input_str = f"{s}|{sep}"
            cmd: Cmd = [exe, current_fn, input_str]  # argv form
            tests.append((current_fn, cmd, [expected]))
            continue

        # ---- ft_strjoin ----
        if current_fn == "ft_strjoin":
            parts = line.split("|")
            if len(parts) < 3:
                continue
            s1, s2 = parts[0].strip(), parts[1].strip()
            if s1.startswith('"') and s1.endswith('"'):
                s1 = s1[1:-1]
            if s2.startswith('"') and s2.endswith('"'):
                s2 = s2[1:-1]
            expected = "|".join(parts[2:]).strip()  # keep literal (harness prints visible escapes)
            input_str = f"{s1}|{s2}"
            cmd = [exe, current_fn, input_str]      # argv form
            tests.append((current_fn, cmd, [expected]))
            continue

        # ---- ft_put* ----
        if current_fn in ("ft_putendl_fd", "ft_putstr_fd", "ft_putnbr_fd", "ft_putchar_fd"):
            if "|" not in line:
                continue
            input_str, expected_raw = [part.strip() for part in line.rsplit("|", 1)]
            expected = _unicode_unescape(expected_raw)
            if input_str.startswith('"') and input_str.endswith('"'):
                input_str = input_str[1:-1]
            cmd = [exe, current_fn, input_str]      # argv form to preserve newlines
            tests.append((current_fn, cmd, [expected]))
            continue

		 # ---- list tests with '|' in expected ----
        if current_fn in (
            "ft_lstadd_front_bonus",
            "ft_lstadd_back_bonus",
        ):
            parts = line.split("|")
            if len(parts) < 3:
                continue

            # first two fields = inputs
            s1, s2 = parts[0].strip(), parts[1].strip()
            if s1.startswith('"') and s1.endswith('"'):
                s1 = s1[1:-1]
            if s2.startswith('"') and s2.endswith('"'):
                s2 = s2[1:-1]

            # everything after = expected (may still contain '|')
            expected_raw = "|".join(parts[2:]).strip()
            expected = _unicode_unescape(expected_raw)

            # argv form: pass both inputs as args
            cmd: Cmd = [exe, current_fn, f"{s1}|{s2}"]
            tests.append((current_fn, cmd, [expected]))
            continue

        # ---- list tests with '|' in expected ----
        if current_fn in (
            "ft_lstiter_bonus",
            "ft_lstmap_bonus",
        ):
            parts = line.split("|")
            if len(parts) < 2:
                continue

            # first field is the whole input (may be quoted to allow internal '|')
            s = parts[0].strip()
            if s.startswith('"') and s.endswith('"'):
                s = s[1:-1]

            # everything after the first '|' belongs to expected (and may contain '|')
            expected_raw = "|".join(parts[1:]).strip()
            expected = _unicode_unescape(expected_raw)

            # pass input via argv form (avoids shell re-quoting)
            cmd: Cmd = [exe, current_fn, s]
            tests.append((current_fn, cmd, [expected]))
            continue


        # ---- default ----
        if "|" not in line:
            continue
        input_str, expected_raw = [part.strip() for part in line.rsplit("|", 1)]
        expected = _unicode_unescape(expected_raw)
        arg = input_str if (input_str.startswith('"') and input_str.endswith('"')) else shlex.quote(input_str)
        cmd = f"{exe} {current_fn} {arg}"           # string form ok
        tests.append((current_fn, cmd, [expected]))

    return tests
