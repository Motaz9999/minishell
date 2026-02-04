/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:45:28 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:14 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	unsigned char		*str;
	unsigned char		*ptr;
	int					len;

	len = ft_strlen(s);
	str = malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	ptr = str;
	while (*s != '\0')
	{
		*str = *s;
		str++;
		s++;
	}
	*str = '\0';
	return ((char *)ptr);
}
/*
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

int main(void)
{
    char *original = "Hello, 42!";
    char *copy = ft_strdub(original, 5);

    printf("Copy: %s\n", copy); // Copy: Hello

    free(copy); // لا تنسى تحرير الذاكرة
    return 0;
}*/
