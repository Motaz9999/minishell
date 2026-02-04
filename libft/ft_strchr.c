/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:03:01 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:11 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0' && *s != (char)c)
		s++;
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}
// #include "libft.h"
// #include <stdio.h>
// #include <string.h>

// void test_ft_strchr(const char *s, int c)
// {
//     char *res_libc = strchr(s, c);
//     char *res_ft = ft_strchr(s, c);
//     printf("Testing strchr('%s', '%c')\n", s, c);
//     if (res_libc == res_ft)
//         printf(" ✅ Passed: returned %p\n\n", (void *)res_ft);
//     else
//  
// int main(void)
// {
//     // 1. حرف موجود في البداية
//     test_ft_strchr("Hello World", 'H');
//     // 2. حرف موجود في الوسط
//     test_ft_strchr("Hello World", 'o');
//     // 3. حرف موجود في النهاية
//     test_ft_strchr("Hello World", 'd');
//     // 4. حرف غير موجود
//     test_ft_strchr("Hello World", 'x');
//     // 5. البحث عن نهاية السلسلة (\0)
//     test_ft_strchr("Hello", '\0');
//     // 6. البحث في سلسلة فارغة
//     test_ft_strchr("", 'a');
//     // 7. البحث عن مسافة
//     test_ft_strchr("Hello World", ' ');
//     // 8. بحث عن حرف متكرر للتأكد أنه يأخذ الأول
//     test_ft_strchr("banana", 'a');
//     return 0;
// }