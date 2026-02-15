/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 20:53:48 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/15 21:10:17 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

/*
** =============================================================================
**   1. TOKEN STRUCTURES (For the Lexer -> output is a token list)
** =============================================================================
*/

// Purpose: To give a specific category to each piece of the input string.
typedef enum e_token_type
{
	TOKEN_WORD,          // A command, argument, or filename (e.g., "ls", "-la")
	TOKEN_PIPE,          // The pipe symbol: |
	TOKEN_REDIR_IN,      // Input redirection: <
	TOKEN_REDIR_OUT,     // Output redirection: >
	TOKEN_REDIR_APPEND,  // Append output redirection: >>
	TOKEN_REDIR_HEREDOC, // Here-document redirection: <<
	TOKEN_EOF            // A marker for the end of the input
}					t_token_type;

// Purpose: To store one piece of the input string in a linked list.
typedef struct s_token
{
	t_token_type type;    // The category of this token (from the enum above).
	char *value;          // The actual text, e.g., "echo" or "hello world".
	int quote_type;       // Records how the token was quoted.
							// 0 = no quotes
							// 1 = single quotes ('')
							// 2 = double quotes ("")
	struct s_token *next; // Pointer to the next token in the chain.
}					t_token;

/*
** =============================================================================
**   2. COMMAND STRUCTURES (For the Parser -> output is a command list)
** =============================================================================
*/

// Purpose: To define the specific type of I/O redirection.

typedef enum e_redir_type
{
	REDIR_IN,     // Input redirection: <
	REDIR_OUT,    // Output redirection: >
	REDIR_APPEND, // Append redirection: >>
	REDIR_HEREDOC // Here-document: <<
}					t_redir_type;

// Purpose: To store information for a single redirection instruction.

typedef struct s_redirect
{
	t_redir_type type;       // The category of redirection (from enum above).
	char *file;              // Filename or heredoc delimiter.
	int fd;                  // The file descriptor, filled in by the Executor.
	int         heredoc_fd;  // For heredoc for the parser to set 
	struct s_redirect *next; // Pointer to the next redirection in the list.
}					t_redirect;

// Purpose: To represent a single command with all its parts.
typedef struct s_command
{
	char **args;            // NULL-terminated array for execve().
	t_redirect *redirects; 
		// A linked list of all redirections for this command.
	struct s_command *next; // Pointer to the next command in a pipeline.
}					t_command;

/*
** =============================================================================
**   3. SHELL & ENVIRONMENT STRUCTURES (The main state of the program)
** =============================================================================
*/

// Purpose: To store a single environment variable (e.g., "PATH=/bin").
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

// Purpose: To hold all the important state of the shell in one place.
typedef struct s_shell
{
	// --- Persistent Shell State ---
	t_env *env_list;      // The head of the environment variable linked list.
	int last_exit_status; // The exit code of the last command (for $?).
	// --- Data for a SINGLE command cycle (must be freed after each command) ---
	t_token *tokens;     // The head of the token list from the lexer.
	t_command *commands; // The head of the command list from the parser.
}					t_shell;

/*
** =============================================================================
**   3. extract struct for motaz
** =============================================================================
*/
typedef struct s_ext
{
	int				pipe_fds[2];
	int				prev_fd_in;
	pid_t			*pids;
	int				i;
	t_command		*cmd;
}				t_ext;

#endif