/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamr <aamr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:46:14 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/17 22:35:45 by aamr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Normalize stored heredoc delimiter text after lexer encoding.
*/
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

/*
** Map lexer token type to parser redirection type.
*/
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

/*
** Allocate and initialize one redirection node.
*/
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
	{
		free(redir);
		return (NULL);
	}
	return (redir);
}
