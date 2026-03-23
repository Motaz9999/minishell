/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 20:53:48 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 18:09:39 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

/*
typedef enum e_token_type
{
	TOKEN_WORD,          // A command, argument, or filename (e.g., "ls", "-la")
	TOKEN_PIPE,          // The pipe symbol: |
	TOKEN_REDIR_IN,      // Input redirection: <
	TOKEN_REDIR_OUT,     // Output redirection: >
	TOKEN_REDIR_APPEND,  // Append output redirection: >>
	TOKEN_REDIR_HEREDOC, // Here-document redirection: <<
	TOKEN_EOF            // A marker for the end of the input
}						t_token_type;
*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
}						t_token_type;

// Purpose: To store one piece of the input string in a linked list.

/*
typedef struct s_token
{
	t_token_type type;    // The category of this token (from the enum above).
	char *value;          // The actual text, e.g., "echo" or "hello world".
	int quote_type;       // Records how the token was quoted.
							// 0 = no quotes
							// 1 = single quotes ('')
							// 2 = double quotes ("")
	struct s_token *next; // Pointer to the next token in the chain.
}						t_token;
*/

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	int					quote_type;
	struct s_token		*next;
}						t_token;

/*
typedef enum e_redir_type
{
	REDIR_IN,     // Input redirection: <
	REDIR_OUT,    // Output redirection: >
	REDIR_APPEND, // Append redirection: >>
	REDIR_HEREDOC // Here-document: <<
}						t_redir_type;
*/
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

// Purpose: To store information for a single redirection instruction.

/*
typedef struct s_redirect
{
	t_redir_type type;       // The category of redirection (from enum above).
	char *file;              // Filename or heredoc delimiter.
	int fd;                  // The file descriptor, filled in by the Executor.
	int heredoc_fd;          // For heredoc for the parser to set
	struct s_redirect *next; // Pointer to the next redirection in the list.
}						t_redirect;
*/
typedef struct s_redirect
{
	t_redir_type		type;
	char				*file;
	int					fd;
	int					heredoc_fd;
	struct s_redirect	*next;
}						t_redirect;

/*
typedef struct s_command
{
	char **args; // NULL-terminated array for execve().
	t_redirect			*redirects;
	// A linked list of all redirections for this command.
		t_quote_type         *quote_types;//this a what type of quote it have and it have one per arg (this is an array or linked list as the same size as args)
	struct s_command *next; // Pointer to the next command in a pipeline.
}						t_command;
*/

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}						t_quote_type;

typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	t_quote_type         *quote_types;
	struct s_command	*next;
}						t_command;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

/*
	// --- Persistent Shell State ---
	t_env *env_list;      // The head of the environment variable linked list.
	int last_exit_status; // The exit code of the last command (for $?).
	// --- Data for a SINGLE command cycle (must be freed after each command) ---
	t_token *tokens;     // The head of the token list from the lexer.
	t_command *commands; // The head of the command list from the parser.
*/
typedef struct s_shell
{
	t_env				*env_list;
	int					last_exit_status;

	t_token				*tokens;
	t_command			*commands;
}						t_shell;

typedef struct s_ext
{
	int					pipe_fds[2];
	int					prev_fd_in;
	pid_t				*pids;
	int					i;
	t_command			*cmd;
}						t_ext;

typedef enum e_builtin
{
	BI_NONE = 0,
	BI_ECHO,
	BI_CD,
	BI_PWD,
	BI_EXPORT,
	BI_UNSET,
	BI_ENV,
	BI_EXIT
}						t_builtin;

typedef struct s_update_env
{
	int					i;
	char				*key;
	char				*value;
	int					cut;
	int					ret;
}						t_update_env;

#endif