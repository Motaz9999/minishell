/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamr <aamr <aamr@student.42.fr>>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 01:06:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/17 21:35:24 by aamr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	finish_heredoc(pid_t pid, int fds[2], t_shell *shell,
		t_redirect *redir)
{
	int	status;

	close(fds[1]);
	if (pid == -1)
	{
		shell->last_exit_status = error_syscall("fork", 1);
		close(fds[0]);
		return (FALSE);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		shell->last_exit_status = error_syscall("waitpid", 1);
		close(fds[0]);
		return (FALSE);
	}
	if (WIFSIGNALED(status))
	{
		shell->last_exit_status = WTERMSIG(status) + 128;
		close(fds[0]);
		return (FALSE);
	}
	shell->last_exit_status = WEXITSTATUS(status);
	redir->heredoc_fd = fds[0];
	return (TRUE);
}

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
