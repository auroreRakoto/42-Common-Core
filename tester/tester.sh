#!/bin/bash
set -e

SCRIPT_DIR="$(cd -- "$(dirname "$0")" && pwd)"

PROJECT="$1"
if [ -z "$PROJECT" ]; then
  echo "Usage: ./tester.sh [libft|printf|philo|gnl]"
  exit 1
fi

# Are we already inside that project? else fallback to ../<project>
if [ "$(basename "$PWD")" = "$PROJECT" ]; then
  PROJECT_DIR="$PWD"
else
  PROJECT_DIR="$SCRIPT_DIR/../$PROJECT"
fi

if [ ! -d "$PROJECT_DIR" ]; then
  echo "❌ Project folder $PROJECT_DIR not found."
  exit 1
fi

# Detect if a Makefile exists in the project
HAVE_MAKEFILE=0
if [ -f "$PROJECT_DIR/Makefile" ] || [ -f "$PROJECT_DIR/makefile" ] || [ -f "$PROJECT_DIR/GNUmakefile" ]; then
  HAVE_MAKEFILE=1
fi

# --- always-run cleanup (even on error/Exit) ---
cleanup() {
  if [ "$HAVE_MAKEFILE" = "1" ]; then
    echo "🧹 Cleaning $PROJECT_DIR (fclean)..."
    make -s -C "$PROJECT_DIR" fclean 1>/dev/null || true
  fi
}
trap cleanup EXIT

# Build project if Makefile exists (silent)
if [ "$HAVE_MAKEFILE" = "1" ]; then
  echo "📂 Found Makefile in $PROJECT_DIR"
  make -s -C "$PROJECT_DIR" 1>/dev/null
else
  echo "⚠️  No Makefile found in $PROJECT_DIR"
fi


# Build harness in tester via Makefile (silent)
case "$PROJECT" in
  libft)
      if [ "$2" = "bonus" ]; then
          make -s -C "$SCRIPT_DIR" bonus 1>/dev/null
          HARNESS="libft_test_bonus"
      else
          make -s -C "$SCRIPT_DIR" libft_test 1>/dev/null
          HARNESS="libft_test"
      fi
      ;;
  printf|ft_printf)
      make -s -C "$SCRIPT_DIR" printf_test 1>/dev/null
      HARNESS="printf_test"
      ;;
  get_next_line|gnl)
      make -s -C "$SCRIPT_DIR" gnl_tests 1>/dev/null
      HARNESS="gnl_test"
      ;;
  philo)  : ;;
  *)      : ;;
esac



# Run Python
if command -v py >/dev/null 2>&1; then PY=py
elif command -v python3 >/dev/null 2>&1; then PY=python3
else PY=python
fi

"$PY" "$SCRIPT_DIR/main.py" "$PROJECT" "$HARNESS"
