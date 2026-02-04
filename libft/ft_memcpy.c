/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:40:14 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:42:45 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, void const *src, size_t size)
{
	unsigned char	*srcptr;
	unsigned char	*destptr;

	srcptr = (unsigned char *)src;
	destptr = (unsigned char *)dest;
	while (size > 0)
	{
		*destptr = *srcptr;
		destptr++;
		srcptr++;
		size--;
	}
	return (dest);
}
/*
#include <stdio.h>

int	main()
{
	char	str[] = "ABCDEFJ";
	printf("%s",(char *)ft_memcpy(str+2, str, 5));
}
*/
/*
#include <stdio.h>
#include <string.h>
#include "libft.h"

void test_memcpy(const void *src, size_t n)
{
    char dest_ft[50] = {0};
    char dest_std[50] = {0};

    ft_memcpy(dest_ft, src, n);
    memcpy(dest_std, src, n);

    printf("Testing memcpy with %zu bytes:\n", n);
    printf("ft_memcpy:  ");
    for (size_t i = 0; i < n; i++)
        printf("%02X ", (unsigned char)dest_ft[i]);
    printf("\n");

    printf("memcpy    :  ");
    for (size_t i = 0; i < n; i++)
        printf("%02X ", (unsigned char)dest_std[i]);
    printf("\n");

    // تحقق من التطابق
    if (memcmp(dest_ft, dest_std, n) == 0)
        printf("✅ Test passed\n");
    else
        printf("❌ Test failed\n");

    printf("-----------------------------\n");
}

int main(void)
{
    char str1[] = "Hello, 42!";
    int numbers[] = {1, 2, 3, 4, 5};

    // نسخ سلسلة نصية كاملة
    test_memcpy(str1, strlen(str1) + 1);

    // نسخ أول 5 بايتات فقط
    test_memcpy(str1, 5);

    // نسخ بيانات غير نصية (int array)
    test_memcpy(numbers, sizeof(numbers));

    // نسخ بيانات جزئية من int array
    test_memcpy(numbers, 3 * sizeof(int));

    return 0;
}
*/
