/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 19:33:27 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 12:04:29 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _POSIX_C_SOURCE 200809L
# include "libft.h"
# include "struct.h"
# include <dirent.h> //for readdie and opendir and close dir
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h> // this is for handling signals
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# define TRUE 1
# define FALSE 0

// global var
// for signals
// this a special int type defined in signals
// Guaranteed to be read/written atomically
// Safe to modify inside a signal handler
// Prevents data corruption if an interrupt happens
// volatile => This variable can change at any time — don’t optimize it away
// Without volatile,
//	the compiler might cache the value and never see updates from the handler.
// volatile sig_atomic_t	g_in_cmd = 0; this false

extern volatile sig_atomic_t g_sigint_received; // defined in signal_handle.c
// path handler funs
char		*resolve_path(char *cmd_name, t_env *env_list);
// the execute funs for now
void		execute(t_shell *shell);
int			count_commands(t_command *cmds);
// error handles
void		error_execve(char *cmd);
int			error_cmd(char *cmd, char *msg, int exit_code);
int			error_syscall(char *context, int exit_code);

//redir + pipes handle
int			handle_redir(t_redirect *redirections, t_shell *shell);
int	handle_pipes(int prev_fd_in, int pipes[], int remaining_cmd , t_shell *shell);
// signals
void		setup_signals_child(void);
void		setup_signals_parent(void);
// builtins
t_builtin	get_builtin(t_command *cmd);
pid_t		execute_builtin(t_ext *ext, t_shell *shell, int casee);
int			echo(char **args);
int			env(t_env *env_list);
int			pwd(t_env *env_list);
t_env		*find_node(t_env *env_list, char *key);
void		update_env_pwd(t_env *env_list, char *new_pwd_val);
int			cd(t_ext *ext, t_shell *shell);
void		del_env(void *raw);
void		*dup_env(void *raw);
void		merge_sort(t_env **head_ref);
int			find_args_count(char **args);
// export
int			valid_arg(char *arg);
int			check_key_exist(t_env *env_list, char *key);
int			add_key_env(t_env **env_list, char *key, char *value);
int			update_env_export(char **args, t_env **env_list);
int			export(char **args, t_shell *shell);
char		*cut_key(char *arg, int cut);
char		*cut_value(char *arg, int cut);
int			find_args_count(char **args);
int			exit_shell(t_ext *ext);
// unset
void		update_env_unset(t_env **head, char *args);
int			unset(char **args, t_shell *shell);
// parser / lexer
t_token		*lexer(const char *line);
void		expand_tokens(t_token *tokens, t_shell *shell);
t_command	*parser(t_token *tokens, t_shell *shell);
void		parse_and_execute(char *line, t_shell *shell);
void		free_tokens(t_token *tokens);
void		free_commands(t_command *cmds);
int			fill_heredoc(t_redirect *redir, t_shell *shell);
// env init
t_env		*init_env(char **envp);
// envp handle
char		**make_envp(t_env *env_list);
int			count_env_vars(t_env *env);
// executor internals
void		waiting_loop_free_pids(pid_t pids[], t_shell *shell, int cmd_count);
#endif