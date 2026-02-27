
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

# ── Unit tests ──────────────────────────────────────────────────────────────
TEST_BIN = tests/run_tests

TEST_SRCS = tests/test_builtins.c tests/stubs.c \
            src/builtin/echo.c \
            src/builtin/export.c \
            src/builtin/export_helper.c \
            src/builtin/merge_sort.c \
            src/builtin/env.c \
            src/builtin/pwd.c \
            src/builtin/unset.c

test: $(LIBFT)
	$(CC) $(CFLAGS) $(INC) $(TEST_SRCS) $(LIBFT) -lreadline -o $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: clean fclean re all test