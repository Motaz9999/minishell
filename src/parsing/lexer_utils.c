/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamr <aamr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:53:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/17 22:35:20 by aamr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*new_token(t_token_type type, char *value, int quote_type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quote_type = quote_type;
	token->next = NULL;
	return (token);
}

void	token_add_back(t_token **head, t_token *new_one)
{
	t_token	*last;

	if (!head || !new_one)
		return ;
	if (!*head)
	{
		*head = new_one;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_one;
}

static t_token	*make_operator_token(t_token_type type, char *str, int *i,
		int step)
{
	char	*value;

	value = ft_strdup(str);
	if (!value)
		return (NULL);
	*i += step;
	return (new_token(type, value, NO_QUOTE));
}
