/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamr <aamr <aamr@student.42.fr>>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:51:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/17 21:49:58 by aamr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_quote_type(int has_single, int has_double)
{
	if (has_double)
		return (DOUBLE_QUOTE);
	if (has_single)
		return (SINGLE_QUOTE);
	return (NO_QUOTE);
}

char	*read_plain_part(char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i] && !is_space(line[*i]) && !is_operator_char(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

static char	*read_part(char *line, int *i, int *has_single, int *has_double)
{
	if (line[*i] == '\'')
	{
		*has_single = 1;
		return (read_single_quoted(line, i));
	}
	if (line[*i] == '"')
	{
		*has_double = 1;
		return (read_double_quoted(line, i));
	}
	return (read_plain_part(line, i));
}

static char	*join_free(char *left, char *right)
{
	char	*joined;

	if (!left)
		return (right);
	if (!right)
		return (free(left), NULL);
	joined = ft_strjoin(left, right);
	free(left);
	free(right);
	return (joined);
}

char	*read_word(char *line, int *i, int *quote_type)
{
	char	*word;
	char	*part;
	int		has_single;
	int		has_double;

	word = NULL;
	has_single = 0;
	has_double = 0;
	while (line[*i] && !is_space(line[*i]) && !is_operator_char(line[*i]))
	{
		part = read_part(line, i, &has_single, &has_double);
		if (!part)
		{
			free(word);
			return (NULL);
		}
		word = join_free(word, part);
		if (!word)
			return (NULL);
	}
	*quote_type = set_quote_type(has_single, has_double);
	return (word);
}
