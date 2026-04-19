/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 04:01:53 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/19 04:02:25 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Close all currently tracked heredoc read-end fds except one command.
** Used in child paths to prevent inherited descriptor leaks.
*/
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
