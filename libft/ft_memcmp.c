/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:34:43 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:42:42 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (n > 0)
	{
		if (*str1 != *str2)
		{
			return (*str1 - *str2);
		}
		str1++;
		n--;
		str2++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
#include "libft.h"

void test_memcmp(const void *s1, const void *s2, size_t n)
{
    int res_ft = ft_memcmp(s1, s2, n);
    int res_std = memcmp(s1, s2, n);

    printf("Comparing first %zu bytes of:\n", n);
    printf("  s1: \"%s\"\n", (char *)s1);
    printf("  s2: \"%s\"\n", (char *)s2);

    printf("ft_memcmp result: %d\n", res_ft);
    printf("memcmp   result: %d\n", res_std);

    if ((res_ft == 0 && res_std == 0) || (res_ft * res_std > 0))
        printf("✅ Test passed\n");
    else
        printf("❌ Test failed\n");

    printf("-----------------------------\n");
}

int main(void)
{
    char str1[] = "Hello, 42!";
    char str2[] = "Hello, 42!";
    char str3[] = "Hello, 43!";
    char str4[] = "Hella, 42!";
    char str5[] = "";

    // حالات مختلفة
    test_memcmp(str1, str2, 12); // متطابقة بالكامل
    test_memcmp(str1, str3, 12); // اختلاف في آخر حرف
    test_memcmp(str1, str4, 5);  // اختلاف في منتصف السلسلة
    test_memcmp(str1, str4, 2);  // أول بايت متطابق
    test_memcmp(str5, str5, 1);  // بايتات فارغة
    test_memcmp(str1, str5, 0);  // طول صفر (يجب أن ترجع 0)

    return 0;
}
*/
