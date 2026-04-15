/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:53:19 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 01:37:03 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if a sys fails use errno
// for all sys calls like    open, read, write,
// fork, pipe, dup2 , chdir, access
int	error_syscall(char *context, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	if (context)
	{
		ft_putstr_fd(context, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (exit_code);
}

int	error_syntax(char *token, int exit_code)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token != NULL)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	return (exit_code);
}

// used for "cmd not found" and for logic errors
// like "custom errors" but but the cmd as NULL
int	error_cmd(char *cmd, char *msg, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (exit_code);
}

// execve failure handler (just for child)
// this is used after execve (it dosent work for a resone)
// there also builtins errors and signals errors 
//but for later
void	error_execve(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	if (errno == EACCES)
		exit(126);
	exit(127);
}
