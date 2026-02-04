/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:40:13 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:42:49 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ptdest;
	unsigned char	*ptsrc;

	ptdest = (unsigned char *)dest;
	ptsrc = (unsigned char *)src;
	if (src < dest)
	{
		ptdest += n - 1;
		ptsrc += n - 1;
		while (n > 0)
		{
			*ptdest = *ptsrc;
			ptdest--;
			ptsrc--;
			n--;
		}
	}
	else
	{
		dest = ft_memcpy(dest, src, n);
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>
#include "libft.h"

int main(void)
{
    char buffer[30] = "1234567890";

    printf("Original buffer: %s\n", buffer);

    // نسخ نصوص كاملة داخل نفس المصفوفة بدون تداخل
    ft_memmove(buffer + 15, buffer, 10); // نسخ "1234567890" إلى نهاية المصفوفة
    printf("Copy without overlap: %s\n", buffer + 15);

    // نسخ مع تداخل: إلى الأمام
    ft_memmove(buffer + 2, buffer, 8); // النسخ الأمامي المتداخل
    buffer[10] = '\0';
    printf("Copy with forward overlap: %s\n", buffer);

    // نسخ مع تداخل: إلى الخلف
    ft_memmove(buffer, buffer + 2, 8); // النسخ الخلفي المتداخل
    buffer[8] = '\0';
    printf("Copy with backward overlap: %s\n", buffer);

    // نسخ جزئي فقط
    ft_memmove(buffer, "HELLO", 5);
    buffer[5] = '\0';
    printf("Partial copy: %s\n", buffer);

    return 0;
}
*/
