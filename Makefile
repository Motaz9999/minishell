
NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a

INC = -I includes -I libft
LINK = -lreadline -lft

SRC = src/main.c 

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -l readline -o $(NAME)

$(LIBFT):
	make -C libft

clean:
	rm -f $(OBJ)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: clean fclean re all