/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 09:32:15 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
}

static void	free_redirects(t_redirect *redirs)
{
	t_redirect	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->file);
		if (redirs->heredoc_fd != -1)
			close(redirs->heredoc_fd);
		free(redirs);
		redirs = next;
	}
}

void	free_commands(t_command *cmds)
{
	t_command	*next;
	int			i;

	while (cmds)
	{
		next = cmds->next;
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
				free(cmds->args[i++]);
			free(cmds->args);
		}
		free_redirects(cmds->redirects);
		free(cmds);
		cmds = next;
	}
}
