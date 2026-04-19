
NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
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
    src/executor/executor.c \
    src/executor/executor_utils.c \
    src/executor/executor_utils2.c \
    src/executor/path_handle.c \
    src/executor/pipes_handle.c \
    src/executor/redir_helper.c \
    src/executor/redirections_handle.c \
    src/signals/signal_handle.c \
    src/signals/signal_handle_helper.c \
    src/utils/error_handle.c \
    src/utils/free_shell.c \
    src/expander/expander.c \
    src/expander/expander_utils.c \
    src/expander/replace_str.c \
    src/parsing/lexer.c \
    src/parsing/lexer_quote.c \
    src/parsing/lexer_utils.c \
    src/parsing/lexer_word.c \
    src/parsing/parser.c \
    src/parsing/heredoc_handle.c \
    src/parsing/heredoc_handle_helper.c \
    src/parsing/parser_and_execute.c \
    src/parsing/parser_utils.c \
    src/parsing/parser_redir.c \
    src/parsing/parser_redir_helper.c \
    src/parsing/syntax_checker.c \
    src/parsing/free_parser.c
    

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

.PHONY: clean fclean re all