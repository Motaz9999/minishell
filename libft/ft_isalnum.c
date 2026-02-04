/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:00:50 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:41:48 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int arg)
{
	if ((arg >= '0' && arg <= '9')
		|| (arg >= 'A' && arg <= 'Z')
		|| (arg >= 'a' && arg <= 'z'))
		return (1);
	return (0);
}
/*
#include <stdio.h>
#include <ctype.h>   // عشان نقارن مع isalnum الأصلي
#include "libft.h"   // دالتك

void test_char(int c)
{
    int std_res = isalnum(c);
    int ft_res  = ft_isalnum(c);

    if (std_res != ft_res)
        printf("❌ Mismatch for '%c' (int=%d) | isalnum=%d, ft_isalnum=%d\n",
               (c >= 32 && c <= 126) ? c : '?', c, std_res, ft_res);
}

int main(void)
{
    int i;

    // 1. اختبر كل ASCII characters
    for (i = -10; i <= 130; i++)  // شمل أقل من 0 و أكبر من 127
        test_char(i);

    // 2. اختبارات خاصة
    int specials[] = {'0', '9', 'A', 'Z', 'a', 'z', '!', ' ', '\n', 127, 255};
    int size = sizeof(specials) / sizeof(specials[0]);

    printf("\n--- Special tests ---\n");
    for (i = 0; i < size; i++)
    {
        int c = specials[i];
        printf("Char: '%c' (int=%d) | isalnum=%d, ft_isalnum=%d\n",
               (c >= 32 && c <= 126) ? c : '?', c,
               isalnum(c), ft_isalnum(c));
    }

    printf("\n✅ Advanced tests finished.\n");
    return (0);
}
*/