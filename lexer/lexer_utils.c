/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 21:10:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/27 17:37:23 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Free all tokens
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*tmp;

	if (!tokens)
		return ;
	current = tokens;
	while (current)
	{
		tmp = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = tmp;
	}
}

// Count the number of tokens
int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	if (!tokens)
		return (0);
	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_EOF)
	{
		count++;
		current = current->next;
	}
	return (count);
}

// Get token at index
t_token	*get_token_at(t_token *tokens, int index)
{
	int		i;
	t_token	*current;

	if (!tokens || index < 0)
		return (NULL);
	current = tokens;
	i = 0;
	while (current && i < index)
	{
		current = current->next;
		i++;
	}
	return (current);
}

// Print tokens for debugging
void	print_tokens(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
	{
		printf("No tokens\n");
		return ;
	}
	current = tokens;
	while (current && current->type != TOKEN_EOF)
	{
		printf("Token Type: %d, Value: %s, Quote: %d\n",
			current->type, current->value, current->quote_type);
		current = current->next;
	}
	printf("Token Type: %d (EOF)\n", current->type);
}
