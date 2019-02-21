NAME = ft_strace

SRC_PATH = ./src/

SRCS =	$(SRC_PATH)main.c \
		$(SRC_PATH)parse_options.c \
		$(SRC_PATH)ft_strace.c \
		$(SRC_PATH)get_command_path.c \

OBJS = $(SRCS:.c=.o)

CC = gcc

FLAGS = -Wall -Werror -Wextra -g

HEADER = -I./inc

RM = rm -rf

all	: $(NAME)

$(NAME):$(OBJS)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(HEADER)

%.o: %.c ./inc/ft_strace.h
	gcc $(FLAGS) -o $@ -c  $< $(HEADER)

clean	:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re	:	fclean
	make

.PHONY: all lib clean fclean re