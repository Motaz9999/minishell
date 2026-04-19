/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 15:54:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/19 03:54:13 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Execute builtin command in child process context.
** Applies pipes/redirections then runs builtin implementation.
*/
void	execute_in_child(t_ext *ext, t_shell *shell)
{
	setup_signals_child();
	close_all_heredoc_fds_except(shell->commands, ext->cmd);
	if (!handle_pipes(ext->prev_fd_in, ext->pipe_fds, count_commands(ext->cmd),
			shell))
	{
		if (ext && ext->pids)
			free(ext->pids);
		free_shell(shell);
		exit(error_syscall("dup2", 1));
	}
	if (!handle_redir(ext->cmd->redirects, shell))
	{
		if (ext && ext->pids)
			free(ext->pids);
		free_shell(shell);
		exit(shell->last_exit_status);
	}
	execute_builtin_cmd(ext, shell);
	if (ext && ext->pids)
		free(ext->pids);
	free_shell(shell);
	exit(shell->last_exit_status);
}

// here we execute the cmds but first we decide what cmd we
// use depends on get_builtin
// shell->last_exit_status = 0; // this mean the process run ok
// shell->last_exit_status = 1; // general error
void	execute_builtin_cmd(t_ext *ext, t_shell *shell)
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
		shell->last_exit_status = exit_shell(ext, shell);
	else if (type == BI_EXPORT)
		shell->last_exit_status = export(ext->cmd->args, shell);
	else if (type == BI_PWD)
		shell->last_exit_status = pwd(shell->env_list);
	else if (type == BI_UNSET)
		shell->last_exit_status = unset(ext->cmd->args, shell);
	if (shell->last_exit_status == TRUE)
		shell->last_exit_status = 0;
	else
		shell->last_exit_status = 1;
}
