/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 22:31:19 by aamr              #+#    #+#             */
/*   Updated: 2026/04/24 22:51:40 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Finalize heredoc collection in parent after fork.
** Wait child, propagate status, and store read-end fd on success.
*/
static int	finish_heredoc(pid_t pid, int fds[2], t_shell *shell,
		t_redirect *redir)
{
	int		status;
	pid_t	ret;

	close(fds[1]);
	if (pid == -1)
	{
		shell->last_exit_status = error_syscall("fork", 1);
		close(fds[0]);
		return (FALSE);
	}
	ret = waitpid(pid, &status, 0);
	while (ret == -1 && errno == EINTR)
		ret = waitpid(pid, &status, 0);
	if (ret == -1)
	{
		shell->last_exit_status = error_syscall("waitpid", 1);
		close(fds[0]);
		return (FALSE);
	}
	if (WIFSIGNALED(status))
	{
		shell->last_exit_status = 130;
		close(fds[0]);
		return (FALSE);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		shell->last_exit_status = 130;
		close(fds[0]);
		return (FALSE);
	}
	shell->last_exit_status = WEXITSTATUS(status);
	redir->heredoc_fd = fds[0];
	return (TRUE);
}

/*
** Create a heredoc pipe and launch reader child.
** The parent returns success/failure and keeps read-end for execution.
*/
int	fill_heredoc(t_command *cmd, t_redirect *redir, t_shell *shell,
		int quote_type)
{
	t_heredoc_ctx	hd;
	int				fds[2];
	pid_t			pid;

	if (!redir)
		return (FALSE);
	if (pipe(fds) == -1)
	{
		shell->last_exit_status = error_syscall("pipe", 1);
		return (FALSE);
	}
	hd.cmd = cmd;
	hd.redir = redir;
	hd.shell = shell;
	hd.key = redir->file;
	hd.quote_type = quote_type;
	pid = fill_heredoc_helper(&hd, fds);
	return (finish_heredoc(pid, fds, shell, redir));
}
