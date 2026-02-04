/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:09:07 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:33 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*new_str;
	char			*ptr;

	i = 0;
	if (!s || !f)
		return (NULL);
	new_str = malloc(ft_strlen(s) + 1);
	ptr = new_str;
	if (!new_str)
		return (NULL);
	while (s[i] != '\0')
	{
		new_str[i] = f(i, s[i]);
		i++;
	}
	new_str[i] = '\0';
	return (ptr);
}
/*
#include <stdio.h>
#include <stdlib.h>

char ft_upper_even(unsigned int i, char c)
{
    if (i % 2 == 0 && c >= 'a' && c <= 'z')
        return c - 32; // uppercase
    return c;
}
int main()
{
	char *s="heelo My name Is Motaz from 42!!?";
	char *m;
	m = (char *)ft_strmapi(s,ft_upper_even);
	printf("%s\n",m);
	free(m);
	return 0;
}*/
