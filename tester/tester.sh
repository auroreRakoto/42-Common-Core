#!/bin/bash
set -e

# Absolute path to this tester folder
SCRIPT_DIR="$(cd -- "$(dirname "$0")" && pwd)"

PROJECT="$1"
if [ -z "$PROJECT" ]; then
  echo "Usage: ./tester.sh [libft|philo|gnl]"
  exit 1
fi

# Determine project dir:
# - If we're already inside that project (basename $PWD == $PROJECT), use current dir
# - Otherwise, fallback to ../$PROJECT relative to tester
if [ "$(basename "$PWD")" = "$PROJECT" ]; then
  PROJECT_DIR="$PWD"
else
  PROJECT_DIR="$SCRIPT_DIR/../$PROJECT"
fi

if [ ! -d "$PROJECT_DIR" ]; then
  echo "❌ Project folder $PROJECT_DIR not found."
  exit 1
fi

# Build the project if it has a Makefile
if [ -f "$PROJECT_DIR/Makefile" ] || [ -f "$PROJECT_DIR/makefile" ] || [ -f "$PROJECT_DIR/GNUmakefile" ]; then
  echo "📂 Found Makefile in $PROJECT_DIR"
  make -s -C "$PROJECT_DIR" 1>/dev/null
else
  echo "⚠️  No Makefile found in $PROJECT_DIR (Makefile pre-tests will be skipped)."
fi

# Build the test harness via tester/Makefile (only for libft tests)
if [ "$PROJECT" = "libft" ]; then
  make -s -C "$SCRIPT_DIR" libft_test 1>/dev/null
fi

# Run Python runner (Windows-friendly selection)
if command -v py >/dev/null 2>&1; then
  PY=py
elif command -v python3 >/dev/null 2>&1; then
  PY=python3
else
  PY=python
fi

"$PY" "$SCRIPT_DIR/main.py" "$PROJECT"
