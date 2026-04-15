/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:22:13 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 01:52:20 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this to see its redir or not
int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC);
}

// see pipe or not
int	check_pipe(t_token *tok)
{
	if (!tok->next || tok->next->type == TOKEN_PIPE
		|| tok->next->type == TOKEN_EOF)
		return (error_syntax("|", 2));
	return (0);
}

// make sure it
int	check_redir(t_token *tok)
{
	if (!tok->next || tok->next->type == TOKEN_EOF)
		return (error_syntax("newline", 2));
	if (tok->next->type != TOKEN_WORD)
		return (error_syntax(tok->next->value, 2));
	return (0);
}

int	validate_syntax(t_token *tokens)
{
	if (tokens && tokens->type == TOKEN_PIPE)
		return (error_syntax("|", 2));
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_PIPE && check_pipe(tokens))
			return (2);
		if (is_redir_token(tokens->type) && check_redir(tokens))
			return (2);
		tokens = tokens->next;
	}
	return (0);
}
