/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:12:48 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/17 17:54:12 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*strpt ;

	strpt = s;
	while (n != 0)
	{
		*strpt = '\0';
		strpt++;
		n--;
	}
}
/*
#include <stdio.h>
#include <string.h> // عشان نستخدم bzero الأصلية للمقارنة
#include "libft.h"  // هان في ft_bzero تبعتك

int main(void)
{
    char str1[20] = "Hello World";
    char str2[20] = "Hello World";

    printf(":\n");
    printf("str1 = %s\n", str1);
    printf("str2 = %s\n", str2);

    ft_bzero(str1, 5);

    bzero(str2, 5);

    printf("\nbzero:\n");
    printf("str1 = %s\n", str1);
    printf("str2 = %s\n", str2);

    
    printf("\n str1  ft_bzero:\n");
    for (int i = 0; i < 12; i++)
        printf("%d ", (unsigned char)str1[i]);

    printf("\n\nstr2  bzero:\n");
    for (int i = 0; i < 12; i++)
        printf("%d ", (unsigned char)str2[i]);

    printf("\n");

    return 0;
}

*/