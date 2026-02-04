/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:33:09 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:22 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	char	*ptr;

	join = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (join == NULL)
		return (NULL);
	ptr = join;
	while (*s1 != '\0')
	{
		*join = *s1;
		s1++;
		join++;
	}
	while (*s2 != '\0')
	{
		*join = *s2;
		s2++;
		join++;
	}
	*join = '\0';
	return (ptr);
}
/*
#include <stdio.h>
#include "libft.h"

int main(void)
{
    char *s1 = "Hello my name is motaz and i am student at ";
    char *s2 = "42 amman!";
    char *joined;

    joined = ft_strjoin(s1, s2);
    if (joined)
    {
        printf("Joined string: %s\n", joined);
        free(joined);
    }
    else
    {
        printf("Memory allocation failed\n");
    }
    return 0;
}
*/
