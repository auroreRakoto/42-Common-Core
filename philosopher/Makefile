NAME	:=	philo
CC		:=	gcc
FLAGS	:=	-Wall -Wextra -Werror
INCS	:=	-I includes
SRCS	:=	srcs/ft_errors.c srcs/ft_free.c srcs/ft_print.c srcs/ft_time.c srcs/main.c
OBJS	:= $(SRCS:.c=.o)
RM		:= rm -rf

CC_TEST	?=	gcc

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) $(INCS) $(SRCS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) $(INCS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re