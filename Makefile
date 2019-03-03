NAME = ft_strace

SRC_PATH = ./src/

SRCS =	$(SRC_PATH)main.c \
		$(SRC_PATH)ft_strace.c \
		$(SRC_PATH)get_command_path.c \
		$(SRC_PATH)buffer.c \
		$(SRC_PATH)get_env_size.c \
		$(SRC_PATH)padding.c \
		$(SRC_PATH)buff_define.c \
		$(SRC_PATH)strace_without_opt.c \


OBJS = $(SRCS:.c=.o)

CC = gcc

FLAGS = -Wall -Werror -Wextra

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
