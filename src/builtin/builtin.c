/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 00:26:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/20 02:13:42 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// so here we check on the cmd if it builtin or not
// if it builtin it works different from the cmds in bins
// soo i must recreate the pipes system and redirection for builtins
t_builtin	get_builtin(t_command *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (BI_NONE); // 0=> false
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

	if (casee == 1)
	{
		pid = fork();
		// now as a kid i also have pipes dont forget it and redir//dont forgot to close the pipes for the main
		if (pid == -1)
		{
			shell->last_exit_status = 127;
			// we are now inside child soo the exit
			exit(error_syscall("fork", 127));
		}
		if (pid != 0)
			return (pid); // end of parent if i had child to deals with (easy )
		// now i am inside child soo what also child needs
		setup_signals_child();
		if (!handle_pipes(ext->prev_fd_in, ext->pipe_fds,
				count_commands(ext->cmd), shell)
			|| !handle_redir(ext->cmd->redirects, shell))
			exit(error_syscall("dup2", shell->last_exit_status));
		// now i handle anything must be handled soo lets continue the code
	}
	else // this mean the parent process
		pid = -1;
	execute_builtin_cmd(ext, shell); // for single cmd and multi
	return (pid);
}

// here we execute the cmds but first we decide what cmd we
// use depends on get_builtin
void	execute_builtin_cmd(t_ext *ext, t_shell *shell)
{
	t_builtin type = get_builtin(ext->cmd); // now i have type
	if (type == BI_CD)
		cd(ext, shell);
	else if (type == BI_ECHO)
		echo(ext, shell);
	else if (type == BI_ENV)
		env(ext, shell);
	else if (type == BI_EXIT)
		exit_bi(ext, shell);
	else if (type == BI_EXPORT)
		export(ext, shell);
	else if (type == BI_PWD)
		pwd(ext, shell);
	else if (type == BI_UNSET)
		unset(ext, shell);
}
