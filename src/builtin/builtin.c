/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 00:26:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/25 06:25:33 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_in_child(t_ext *ext, t_shell *shell);
static void	execute_builtin_cmd(t_ext *ext, t_shell *shell);

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

// here we decide how to execute the builtin and what it  needs
// and if it a child or single cmd(no fork needed)
// if single return -1 there is noo pid
// if child return the pid of it
// just put it in the ext.pids
pid_t	execute_builtin(t_ext *ext, t_shell *shell, int casee)
{
	pid_t	pid;
	int		saved_stdin;
	int		saved_stdout;

	if (casee == 1) // there is a child
	{
		pid = fork();
		if (pid == -1)
			return (error_syscall("fork", -1));
		if (pid == 0)
			execute_in_child(ext, shell);
		// here there just use the the handles that u used in the fork for a child
		return (pid);
	}
	// ok here i want to handle the redir if it in the parent (must save the IN AND OUT after handle the redir return it to the original)
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (error_syscall("dup", -1));
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		close(saved_stdin);
		return (error_syscall("dup", -1));
	}
	if (!handle_redir(ext->cmd->redirects, shell))
		shell->last_exit_status = 1;
	else
		execute_builtin_cmd(ext, shell);
	dup2(saved_stdin, STDIN_FILENO);   // return it into the original input
	dup2(saved_stdout, STDOUT_FILENO); // return it into the original input
	close(saved_stdin);
	return (close(saved_stdout) - 1); // it return -1 EASY
									  //-1 => pid_t is parent so no need to wait
}

// here i want to execute in child
static void	execute_in_child(t_ext *ext, t_shell *shell)
{
	setup_signals_child();
	if (!handle_pipes(ext->prev_fd_in, ext->pipe_fds, count_commands(ext->cmd),
			shell) || !handle_redir(ext->cmd->redirects, shell))
		exit(error_syscall("dup2", 1));
	execute_builtin_cmd(ext, shell);
	exit(shell->last_exit_status);
}

// here we execute the cmds but first we decide what cmd we
// use depends on get_builtin
static void	execute_builtin_cmd(t_ext *ext, t_shell *shell)
{
	t_builtin	type;

	type = get_builtin(ext->cmd);
	if (type == BI_CD)
		shell->last_exit_status = cd(ext, shell);
	else if (type == BI_ECHO)
		shell->last_exit_status = echo(ext->cmd->args);
	else if (type == BI_ENV)
		shell->last_exit_status = env(shell->env_list);
	else if (type == BI_EXIT)
		builtin_exit(ext->cmd->args, shell);
	else if (type == BI_EXPORT)
		shell->last_exit_status = export(ext->cmd->args, shell);
	else if (type == BI_PWD)
		shell->last_exit_status = pwd(shell->env_list);
	else if (type == BI_UNSET)
		shell->last_exit_status = unset(ext->cmd->args, shell);
	
	if (shell->last_exit_status == TRUE)
		shell->last_exit_status = 0;//this mean the process run ok
	else
		shell->last_exit_status = 1;// general error
}
