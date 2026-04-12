/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 19:33:27 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/13 01:59:32 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _POSIX_C_SOURCE 200809L
# include "colors.h"
# include "libft.h"
# include "struct.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
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

extern volatile sig_atomic_t	g_sigint_received;
// path handler funs
void							resolve_path(char **p, char *c, t_env *e,
									t_shell *s);
void							execute(t_shell *shell);
int								count_commands(t_command *cmds);
// error handles
void							error_execve(char *cmd);
int								error_cmd(char *cmd, char *msg, int exit_code);
int								error_syscall(char *context, int exit_code);
int								error_syntax(char *token, int exit_code);
// redir + pipes handle
int								handle_redir(t_redirect *redirections,
									t_shell *shell);
int								handle_pipes(int p, int fds[], int rem,
									t_shell *s);
// signals
void							setup_signals_child(void);
void							setup_signals_parent(void);
void							setup_signals_waits(void);
void							setup_signals_heredoc(void);
// builtins
t_builtin						get_builtin(t_command *cmd);
pid_t							execute_builtin(t_ext *ext, t_shell *shell,
									int casee);
int								echo(char **args);
int								env(t_env *env_list);
int								pwd(t_env *env_list);
t_env							*find_node(t_env *env_list, char *key);
void							update_env_pwd(t_env *env_list,
									char *new_pwd_val);
int								cd(t_ext *ext, t_shell *shell);
void							del_env(void *raw);
void							*dup_env(void *raw);
void							merge_sort(t_env **head_ref);
int								find_args_count(char **args);
// export
int								add_key_env(t_env **env_list, char *key,
									char *value);
int								export(char **args, t_shell *shell);
int								exit_shell(t_ext *ext, t_shell *shell);
// unset
void							update_env_unset(t_env **head, char *args);
int								unset(char **args, t_shell *shell);
// export helpers
int								valid_arg(char *arg);
int								check_key_exist(t_env *env_list, char *key);
int								update_env_export(char **args,
									t_env **env_list);
char							*cut_key(char *arg, int cut);
char							*cut_value(char *arg, int cut);
int								error_cmd_export(char *input);
int								print_all_env_in_order(t_env *env_list);
// parser / lexer
t_token							*lexer(char *line);
int								is_space(char c);
int								is_operator_char(char c);
t_token							*new_token(t_token_type type, char *value,
									int quote_type);
void							token_add_back(t_token **head,
									t_token *new_one);
t_token							*read_operator_token(char *line, int *i);
char							*read_word(char *line, int *i, int *quote_type);
char							*read_plain_part(char *line, int *i);
char							*read_single_quoted(char *line, int *i);
char							*read_double_quoted(char *line, int *i);
void							expand_tokens(t_token *tokens, t_shell *shell);
t_command						*parser(t_token *tokens, t_shell *shell);
void							parse_and_execute(char *line, t_shell *shell);
int								validate_syntax(t_token *tokens);
int								is_redir_token(t_token_type type);
int								check_redir(t_token *tok);
int								parse_redirection(t_command *cmd, t_token **tok,
									t_shell *shell);
int								count_args_in_segment(t_token *tok);
t_command						*new_command(void);
void							command_add_back(t_command **head,
									t_command *new_one);
t_redir_type					get_redir_type(t_token_type type);
t_redirect						*new_redirect(t_redir_type type, char *file);
void							redirect_add_back(t_redirect **head,
									t_redirect *new_one);
void							free_tokens(t_token *tokens);
void							free_commands(t_command *commands);
void							free_redirects(t_redirect *redirects);
int								count_tokens(t_token *tokens);
t_token							*get_token_at(t_token *tokens, int index);
void							print_tokens(t_token *tokens);
int								count_commands_parser(t_command *commands);
void							print_commands(t_command *commands);
int								fill_heredoc(t_redirect *redir, t_shell *shell,
									int quote_type);
// env init
t_env							*init_env(char **envp);
void							free_env_list(t_env *list);
// envp handle
char							**make_envp(t_env *env_list);
int								count_env_vars(t_env *env);
// executor internals
void							free_pids(t_ext *ext);
// shell cleanup
void							free_shell(t_shell *shell);

// utitles
pid_t							execute_one_cmd(t_ext *ext, t_shell *shell);
void							execute_in_child(t_ext *ext, t_shell *shell);
void							execute_builtin_cmd(t_ext *ext, t_shell *shell);

// expander
void							expand_args_from_cmd(t_shell *shell,
									t_command *cmd);
char							*expand_cmd(char *expand_it, t_shell *shell);

int								search_for_special(char *word);
int								check_valid_key_char(char c);
// replace str
char							*replace_str(char *expand_it, t_shell *shell);
#endif