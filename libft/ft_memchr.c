/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:28:57 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:42:40 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	unsigned char	*pstr;

	pstr = (unsigned char *) str;
	while (n > 0)
	{
		if (*pstr == (unsigned char)c)
		{
			return (pstr);
		}
		n--;
		pstr++;
	}
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>
#include "libft.h"

void test_memchr(const char *str, int c, size_t n)
{
    void *res_ft = ft_memchr(str, c, n);
    void *res_std = memchr(str, c, n);

    printf("Testing character '%c' in \"%s\" (n=%zu)\n", c, str, n);
    if (res_ft)
        printf("ft_memchr: found at index %ld\n", (char *)res_ft - str);
    else
        printf("ft_memchr: not found\n");

    if (res_std)
        printf("memchr   : found at index %ld\n", (char *)res_std - str);
    else
        printf("memchr   : not found\n");

    printf("-----------------------------\n");
}

int main(void)
{
    char str1[] = "Hello, 42!";
    char str2[] = "abcdef";
    char str3[] = "";

    // حالات مختلفة
    test_memchr(str1, '4', 10);   // موجود داخل النطاق
    test_memchr(str1, '!', 20);   // موجود بعد النطاق الأصلي
    test_memchr(str1, 'z', 10);   // غير موجود
    test_memchr(str2, 'c', 3);    // نهاية النطاق
    test_memchr(str2, 'c', 2);    // ضمن النطاق لكن غير موجود
    test_memchr(str3, 'a', 1);    // سلسلة فارغة

    return 0;
}
*/
