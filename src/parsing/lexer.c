/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:51:17 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 01:52:06 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_eof_token(t_token **head)
{
	t_token	*eof;

	eof = new_token(TOKEN_EOF, NULL, NO_QUOTE);
	if (!eof)
		return (1);
	token_add_back(head, eof);
	return (0);
}

static int	add_word_token(t_token **head, char *line, int *i)
{
	t_token	*token;
	char	*word;
	int		quote_type;

	word = read_word(line, i, &quote_type);
	if (!word)
		return (1);
	token = new_token(TOKEN_WORD, word, quote_type);
	if (!token)
		return (free(word), 1);
	token_add_back(head, token);
	return (0);
}

t_token	*lexer(char *line)
{
	t_token	*head;
	t_token	*token;
	int		i;

	head = NULL;
	i = 0;
	while (line && line[i])
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (is_operator_char(line[i]))
		{
			token = read_operator_token(line, &i);
			if (!token)
				return (free_tokens(head), NULL);
			token_add_back(&head, token);
		}
		else if (add_word_token(&head, line, &i))
			return (free_tokens(head), NULL);
	}
	if (add_eof_token(&head))
		return (free_tokens(head), NULL);
	return (head);
}
