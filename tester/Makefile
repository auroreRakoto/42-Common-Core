# =========================
# tester/Makefile
# =========================

# OS detection for .exe suffix (Windows) vs none (Unix)
ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

NAME     = libft_test$(EXE)
CC       = gcc
CFLAGS   = -Wall -Wextra -Werror
INCLUDES = -I ../libft
LIBFT    = ../libft/libft.a

SRCS = libft_test.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

# Build harness and link against ../libft/libft.a
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(INCLUDES)

# Ensure the library exists / up to date
$(LIBFT):
	$(MAKE) -C ../libft

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# Convenience target used by tester.sh
libft_test: $(NAME)

.PHONY: all clean fclean re libft_test
