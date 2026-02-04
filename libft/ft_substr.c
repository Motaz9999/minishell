/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:01:45 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:57 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	char	*ptr;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	s += start;
	if (len > ft_strlen(s))
		len = ft_strlen(s);
	sub = malloc((len + 1) * sizeof(char));
	if (sub == NULL)
		return (NULL);
	ptr = sub;
	while (*s != '\0' && len > 0)
	{
		*sub = *s;
		s++;
		sub++;
		len--;
	}
	*sub = '\0';
	return (ptr);
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *str = "HelloWorld";
    char *s1 = ft_substr(str, 0, 5);    // "Hello"
    char *s2 = ft_substr(str, 5, 10);   // "World"
    char *s3 = ft_substr(str, 20, 5);   // "" (start > strlen)
    char *s4 = ft_substr(str, 3, 4);    // "loWo"

    printf("'%s'\n", s1);
    printf("'%s'\n", s2);
    printf("'%s'\n", s3);
    printf("'%s'\n", s4);

    free(s1);
    free(s2);
    free(s3);
    free(s4);
}*/
