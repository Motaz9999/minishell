/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 19:33:27 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/15 00:39:40 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "struct.h"
# include <dirent.h> //for readdie and opendir and close dir
# include <errno.h>
# include <limits.h>
# include <signal.h> // this is for handling signals
# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# define TRUE 1
# define FALSE 0

// path handler funs
char	*resolve_path(char *cmd_name, t_env *env_list);
// the excute funs for now
void	execute(t_shell *shell);
// error handles
void	error_execve(char *cmd);
int		error_cmd(char *cmd, char *msg, int exit_code);
int		error_syscall(char *context, int exit_code);

#endif