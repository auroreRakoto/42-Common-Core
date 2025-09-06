NAME = libftprintf.a
FLAGS = -Wall -Wextra -Werror
CC = gcc
AR = ar crs
RM = rm -rf
SRCS_DIR = srcs
SRCS = ft_printf.c \
	$(SRCS_DIR)/ft_putnbr_fd.c \
	$(SRCS_DIR)/ft_putstr_fd.c \
	$(SRCS_DIR)/ft_strlen.c \
	$(SRCS_DIR)/ft_toupper.c \
	$(SRCS_DIR)/ft_write_str.c \
	$(SRCS_DIR)/ft_write_c.c \
	$(SRCS_DIR)/ft_write_d.c \
	$(SRCS_DIR)/ft_write_u.c \
	$(SRCS_DIR)/ft_write_p.c \
	$(SRCS_DIR)/ft_write_x.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)
.c.o:
	gcc $(FLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re