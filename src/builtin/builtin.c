/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 00:26:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 01:24:07 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// so here we check on the cmd if it builtin or not
// if it builtin it works different from the cmds in bins
// soo i must recreate the pipes system and redirection for builtins
t_builtin	get_builtin(t_command *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (BI_NONE);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (BI_ECHO);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (BI_CD);
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (BI_PWD);
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (BI_EXPORT);
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (BI_UNSET);
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (BI_ENV);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (BI_EXIT);
	return (BI_NONE);
}

//this fun for save stdin and stdout
static int	execute_builtin_helper(t_ext *ext, int *saved_stdin,
		int *saved_stdout)
{
	if (ext)
	{
		ext->pipe_fds[0] = -1;
		ext->pipe_fds[1] = -1;
	}
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1)
		return (error_syscall("dup", -1));
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout == -1)
	{
		close(*saved_stdin);
		return (error_syscall("dup", -1));
	}
	if (ext)
	{
		ext->pipe_fds[0] = *saved_stdin;
		ext->pipe_fds[1] = *saved_stdout;
	}
	return (0);
}

static pid_t	child_builtin(t_ext *ext, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (error_syscall("fork", -1));
	if (pid == 0)
		execute_in_child(ext, shell);
	return (pid);
}

static int	restore_builtin_stdio(t_ext *ext, int saved_stdin, int saved_stdout)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		return (error_syscall("dup2", -1));
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		return (error_syscall("dup2", -1));
	close(saved_stdin);
	close(saved_stdout);
	if (ext)
	{
		ext->pipe_fds[0] = -1;
		ext->pipe_fds[1] = -1;
	}
	return (0);
}

// here we decide how to execute the builtin and what it  needs
// and if it a child or single cmd(no fork needed)
// if single return -1 there is noo pid
// if child return the pid of it
// just put it in the ext.pids
// if (casee == 1) // there is a child
// ok here i want to handle the redir if it in the parent
// (must save the IN AND OUT after handle the redir return it to the original)
// it return -1 EASY //-1 => pid_t is parent so no need to wait
pid_t	execute_builtin(t_ext *ext, t_shell *shell, int casee)
{
	int		saved_stdin;
	int		saved_stdout;

	if (casee == 1)
		return (child_builtin(ext, shell));
	if (execute_builtin_helper(ext, &saved_stdin, &saved_stdout) == -1)
		return (-1);
	if (!handle_redir(ext->cmd->redirects, shell))
		shell->last_exit_status = 1;
	else
		execute_builtin_cmd(ext, shell);
	restore_builtin_stdio(ext, saved_stdin, saved_stdout);
	return (-1);
}
