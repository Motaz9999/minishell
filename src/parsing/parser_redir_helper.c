/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:46:14 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 01:51:50 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this fun is for finding out if the key have to expand anything
// so the key must be normalize first
char	*normalize_heredoc_key(char *raw)
{
	char	*key;
	int		i;

	key = ft_strdup(raw);
	if (!key)
		return (NULL);
	i = 0;
	while (key[i])
	{
		if (key[i] == '\1')
			key[i] = '$';
		i++;
	}
	return (key);
}

// know the redir type
t_redir_type	get_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

// add new node but must have init values
t_redirect	*new_redirect(t_redir_type type, char *file)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->fd = -1;
	redir->heredoc_fd = -1;
	redir->next = NULL;
	if (!redir->file)
		return (free(redir), NULL);
	return (redir);
}
