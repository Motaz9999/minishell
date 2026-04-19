/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_handle.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:01 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/19 03:49:45 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// It iterates over all commands and all redirections.
// For each REDIR_HEREDOC with heredoc_fd != -1
// it closes that FD
//close inherited/unneeded heredoc FDs across command lis
void	close_all_heredoc_fds_except(t_command *commands, t_command *keep_cmd)
{
	t_command	*cmd;
	t_redirect	*redir;

	cmd = commands;
	while (cmd)
	{
		if (cmd != keep_cmd)
		{
			redir = cmd->redirects;
			while (redir)
			{
				if (redir->type == REDIR_HEREDOC && redir->heredoc_fd != -1)
				{
					close(redir->heredoc_fd);
					redir->heredoc_fd = -1;
				}
				redir = redir->next;
			}
		}
		cmd = cmd->next;
	}
}

// here we handle  heredoc redir << // already opend  and filed and sent
// open fails
//apply one command heredoc as stdin via dup2(fd, 0), then close it.
static int	handle_redir_heredoc(t_redirect *redir, t_shell *shell)
{
	if (redir->heredoc_fd == -1)
	{
		error_syscall("open", 1);
		return (FALSE);
	}
	if (dup2(redir->heredoc_fd, 0) == -1)
	{
		shell->last_exit_status = 1;
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
		return (error_syscall("dup2", 1) - 1);
	}
	close(redir->heredoc_fd);
	redir->heredoc_fd = -1;
	return (TRUE);
}

// here we handle input redir <
// open fails
// 1 => true
// open fails
static int	handle_redir_in(t_redirect *redir, t_shell *shell)
{
	redir->fd = open(redir->file, O_RDONLY);
	if (redir->fd == -1)
	{
		shell->last_exit_status = 1;
		return (error_syscall("open", 1) - 1);
	}
	if (dup2(redir->fd, 0) == -1)
	{
		shell->last_exit_status = 1;
		close(redir->fd);
		return (error_syscall("dup2", 1) - 1);
	}
	close(redir->fd);
	return (TRUE);
}

// here we handle output redir >
static int	handle_redir_out(t_redirect *redir, t_shell *shell)
{
	redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->fd == -1)
	{
		shell->last_exit_status = 1;
		return (error_syscall("open", 1) - 1);
	}
	if (dup2(redir->fd, 1) == -1)
	{
		shell->last_exit_status = 1;
		close(redir->fd);
		return (error_syscall("dup2", 1) - 1);
	}
	close(redir->fd);
	return (TRUE);
}

// here we handle append redir >>
static int	handle_redir_append(t_redirect *redir, t_shell *shell)
{
	redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->fd == -1)
	{
		shell->last_exit_status = 1;
		return (error_syscall("open", 1) - 1);
	}
	if (dup2(redir->fd, 1) == -1)
	{
		shell->last_exit_status = 1;
		close(redir->fd);
		return (error_syscall("dup2", 1) - 1);
	}
	close(redir->fd);
	return (TRUE);
}

// here we are inside child process
// it handles cases like
//  Example 1: Input AND output redirection
// cat < input.txt > output.txt
//  Example 2: Multiple output redirections (last one wins)
// echo hello > file1.txt > file2.txt > file3.txt
//  Example 3: Multiple input redirections (last one wins)
// cat < file1.txt < file2.txt < file3.txt
//  Example 4: Mix of input, output, and append
// grep pattern < input.txt > output.txt >> log.txt
//  Example 5: Heredoc with output redirection
// cat << EOF > output.txt
// line 1
// line 2
// EOF
// here it return true or false
int	handle_redir(t_redirect *redirections, t_shell *shell)
{
	t_redirect	*redir;

	redir = redirections;
	while (redir != NULL)
	{
		if (redir->type == REDIR_HEREDOC && !handle_redir_heredoc(redir, shell))
			return (FALSE);
		else if (redir->type == REDIR_IN && !handle_redir_in(redir, shell))
			return (FALSE);
		else if (redir->type == REDIR_OUT && !handle_redir_out(redir, shell))
			return (FALSE);
		else if (redir->type == REDIR_APPEND && !handle_redir_append(redir,
				shell))
			return (FALSE);
		redir = redir->next;
	}
	return (TRUE);
}
