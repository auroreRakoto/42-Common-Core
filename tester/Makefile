# -------- shared -----------
ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

CC						:= gcc
CFLAGS					:= -Wall -Wextra -Werror
RM						:= rm -f

# -------- libft harness -----------
LIBFT_DIR               := ../libft
LIBFT_A                 := $(LIBFT_DIR)/libft.a
CPPFLAGS                += -I$(LIBFT_DIR)

LIBFT_TEST              := libft_test$(EXE)
LIBFT_TEST_SRCS         := libft_test.c
LIBFT_TEST_OBJS         := $(LIBFT_TEST_SRCS:.c=.o)

LIBFT_TEST_BONUS        := libft_test_bonus$(EXE)
LIBFT_TEST_BONUS_SRCS   := libft_test_bonus.c
LIBFT_TEST_BONUS_OBJS   := $(LIBFT_TEST_BONUS_SRCS:.c=.o)

.PHONY: all bonus libft_bonus libft_test

all: $(LIBFT_TEST)

$(LIBFT_TEST): $(LIBFT_TEST_OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) -o $@ $(LIBFT_TEST_OBJS) $(LIBFT_A)

# --- build libft with bonus objects before linking the bonus harness ---
bonus: libft_bonus $(LIBFT_TEST_BONUS)

libft_bonus:
	$(MAKE) -C $(LIBFT_DIR) bonus

$(LIBFT_TEST_BONUS): $(LIBFT_TEST_BONUS_OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) -o $@ $(LIBFT_TEST_BONUS_OBJS) $(LIBFT_A)

# fallback for non-bonus targets that still need libft
$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

libft_test: $(LIBFT_TEST)



# -------- printf harness -----------
# Change ../printf if your folder is named ../ft_printf
PRINTF_DIR			:= ../ft_printf
PRINTF_LIB			:= $(PRINTF_DIR)/libftprintf.a
INC_PRINTF			:= -I $(PRINTF_DIR)

PRINTF_TEST			:= printf_test$(EXE)
PRINTF_TEST_SRCS	:= printf_test.c
PRINTF_TEST_OBJS	:= $(PRINTF_TEST_SRCS:.c=.o)

printf_test: $(PRINTF_TEST)

$(PRINTF_TEST): $(PRINTF_TEST_OBJS) $(PRINTF_LIB)
	$(CC) $(CFLAGS) -o $@ $(PRINTF_TEST_OBJS) $(PRINTF_LIB) $(INC_PRINTF)

$(PRINTF_LIB):
	$(MAKE) -C $(PRINTF_DIR)

# -------- gnl harness (NEW) -----------
TESTER_DIR := $(abspath .)
GNL_DIR    := $(abspath ../get_next_line)
GNL_SRC    := $(GNL_DIR)/get_next_line.c
GNL_HDR    := $(GNL_DIR)/get_next_line.h
GNL_INC    := -I$(GNL_DIR)

GNL_HARNESS := gnl_test
GNL_MAIN    := gnl_test.c

gnl_test_bs_%$(EXE): $(GNL_MAIN) $(GNL_SRC) $(GNL_HDR)
	$(CC) $(CFLAGS) $(GNL_INC) -DBUFF_SIZE=$* -o $@ $(GNL_MAIN) $(GNL_SRC) $(LIBFT_LNK)

# convenience target builds three useful sizes
gnl_tests: gnl_test_bs_1$(EXE) gnl_test_bs_32$(EXE) gnl_test_bs_9999$(EXE)

gnl_clean:
	$(RM) gnl_test_bs_1$(EXE) gnl_test_bs_32$(EXE) gnl_test_bs_9999$(EXE)

clean:
	$(RM) $(LIBFT_TEST_OBJS) $(PRINTF_TEST_OBJS)

fclean: clean
	$(RM) $(LIBFT_TEST) $(PRINTF_TEST)

re: fclean all

.PHONY: all clean fclean re libft_test printf_test gnl_tests gnl_clean
