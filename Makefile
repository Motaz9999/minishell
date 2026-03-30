
NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a

INC = -I includes -I libft
LINK = -lreadline -lft

SRC =	src/main.c \
    src/builtin/builtin.c \
    src/builtin/builtin_utils.c \
    src/builtin/cd.c \
    src/builtin/echo.c \
    src/builtin/env.c \
    src/builtin/exit.c \
    src/builtin/export.c \
    src/builtin/export_helper.c \
    src/builtin/export_helper2.c \
    src/builtin/export_utils.c \
    src/builtin/merge_sort.c \
    src/builtin/pwd.c \
    src/builtin/unset.c \
    src/env_list/env_list.c \
    src/executor/envp_handle.c \
    src/executor/error_handle.c \
    src/executor/executor.c \
    src/executor/executor_utils.c \
    src/executor/path_handle.c \
    src/executor/pipes_handle.c \
    src/executor/redirections_handle.c \
    src/signals/signal_handle.c \
    src/utils/free_shell.c \
    tests/lexer/lexer.c \
    tests/lexer/lexer_utils.c \
    src/expander/expander.c \
    src/expander/replace_str.c \
    tests/parsing/parser.c \
    tests/parsing/parser_utils.c \
    tests/parsing/parse_and_execute.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

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