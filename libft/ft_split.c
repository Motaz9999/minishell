/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:15:05 by moodeh            #+#    #+#             */
/*   Updated: 2025/11/06 16:58:18 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count_words(char const *s, char c)
{
	int	count;
	int	words;

	count = 0;
	words = 0;
	while (*s != '\0')
	{
		if (*s != c && !words)
		{
			words = 1;
			count++;
		}
		else if (*s == c)
			words = 0;
		s++;
	}
	return (count);
}

static char	*ft_subs(char const *start, char const *end)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = (int)(end - start);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (len > i)
	{
		str[i] = start[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static void	ft_free_split(char **substring)
{
	int	i;

	if (!substring)
		return ;
	i = 0;
	while (substring[i])
	{
		free(substring[i]);
		i++;
	}
	free(substring);
}

static char	**finally(char **substrings, int count, char const *s, char c)
{
	char const	*start;
	char const	*end;
	int			i;

	i = 0;
	while (i < count && *s != '\0')
	{
		while (*s == c)
			s++;
		start = s;
		while (*s != c && *s != '\0')
			s++;
		end = s;
		substrings[i] = ft_subs(start, end);
		if (!substrings[i])
		{
			ft_free_split(substrings);
			return (NULL);
		}
		i++;
	}
	substrings[i] = NULL;
	return (substrings);
}

char	**ft_split(char const *s, char c)
{
	char	**substrings;
	int		count;

	if (!s)
		return (NULL);
	count = ft_count_words(s, c);
	substrings = (char **)malloc(sizeof(char *) * (count + 1));
	if (!substrings)
		return (NULL);
	if (!finally(substrings, count, s, c))
		return (NULL);
	return (substrings);
}
