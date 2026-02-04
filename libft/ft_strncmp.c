/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:16:51 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:36 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t size)
{
	unsigned char	*pts1;
	unsigned char	*pts2;
	size_t			i;

	pts1 = (unsigned char *)s1;
	pts2 = (unsigned char *)s2;
	i = 0;
	while (size > i)
	{
		if (pts1[i] != pts2[i])
		{
			return (pts1[i] - pts2[i]);
		}
		if (pts1[i] == '\0')
			return (0);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *a = "Hello";
    char *b = "Hello";
    char *c = "HelLo";
    char *d = "HelloWorld";
    char *e = "";
    char *f = "H";

    size_t n_values[] = {0, 1, 3, 5, 10};
    int i;

    printf("===== ft_strncmp Advanced Test =====\n\n");

    // Case 1: Equal strings
    for (i = 0; i < 5; i++)
    {
        size_t n = n_values[i];
        printf("Compare \"%s\" vs \"%s\" for n=%zu: ft=%d, std=%d\n",
               a, b, n, ft_strncmp(a, b, n), strncmp(a, b, n));
    }

    printf("\n");

    // Case 2: Case sensitive difference
    for (i = 0; i < 5; i++)
    {
        size_t n = n_values[i];
        printf("Compare \"%s\" vs \"%s\" for n=%zu: ft=%d, std=%d\n",
               a, c, n, ft_strncmp(a, c, n), strncmp(a, c, n));
    }

    printf("\n");

    // Case 3: One string shorter
    for (i = 0; i < 5; i++)
    {
        size_t n = n_values[i];
        printf("Compare \"%s\" vs \"%s\" for n=%zu: ft=%d, std=%d\n",
               a, d, n, ft_strncmp(a, d, n), strncmp(a, d, n));
    }

    printf("\n");

    // Case 4: Empty string
    for (i = 0; i < 5; i++)
    {
        size_t n = n_values[i];
        printf("Compare \"%s\" vs \"%s\" for n=%zu: ft=%d, std=%d\n",
               e, f, n, ft_strncmp(e, f, n), strncmp(e, f, n));
    }

    return 0;
}
*/
