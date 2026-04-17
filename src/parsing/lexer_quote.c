/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamr <aamr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 02:06:37 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/17 22:35:21 by aamr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	protect_dollars(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '$')
			s[i] = 1;
		i++;
	}
}

char	*read_single_quoted(char *line, int *i)
{
	int		start;
	char	*part;

	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	if (line[*i] != '\'')
	{
		error_syntax("newline", 2);
		return (NULL);
	}
	part = ft_substr(line, start, *i - start);
	if (part)
		protect_dollars(part);
	(*i)++;
	return (part);
}

char	*read_double_quoted(char *line, int *i)
{
	int		start;
	char	*part;

	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != '"')
		(*i)++;
	if (line[*i] != '"')
	{
		error_syntax("newline", 2);
		return (NULL);
	}
	part = ft_substr(line, start, *i - start);
	(*i)++;
	return (part);
}
