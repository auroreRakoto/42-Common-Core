#!/bin/bash

PROJECT=$1
if [ -z "$PROJECT" ]; then
  echo "Usage: ./tester.sh [libft|philo|gnl]"
  exit 1
fi

# Build project before testing
if [ "$PROJECT" = "libft" ]; then
  make -C ../libft || exit 1
  # build the tester driver in this folder (needs a Makefile in tester/)
  make -C . || exit 1
elif [ "$PROJECT" = "philo" ]; then
  make -C ../philo || exit 1
elif [ "$PROJECT" = "gnl" ]; then
  make -C ../get_next_line || exit 1
fi

# Windows compatibility: prefer 'py' if available
if command -v py >/dev/null 2>&1; then
  py main.py $PROJECT
else
  python3 main.py $PROJECT
fi
