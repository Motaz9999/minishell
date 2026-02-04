/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:20:39 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:42:52 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int value, size_t num)
{
	unsigned char	*sptr;

	sptr = (unsigned char *)s;
	while (num > 0)
	{
		*sptr = (unsigned char)value;
		sptr++;
		num--;
	}
	return (s);
}
/*
#include <stdio.h>
#include <string.h>

void print_bytes(void *ptr, size_t n)
{
    unsigned char *p = ptr;
    for (size_t i = 0; i < n; i++)
        printf("%02X ", p[i]);
    printf("\n");
}
void test_memset(void *buf_ft, void *buf_std, int c, size_t n)
{
    ft_memset(buf_ft, c, n);
    memset(buf_std, c, n);

    printf("Testing memset with %zu bytes, value %d:\n", n , c);
    printf("ft_memset : ");
    print_bytes(buf_ft, n + 1);
    printf("memset    : ");
    print_bytes(buf_std, n + 1);

    if (memcmp(buf_ft, buf_std, n + 1) == 0)
        printf("✅ Test passed\n");
    else
        printf("❌ Test failed\n");

    printf("-----------------------------\n");
}
int main()
{
    char buffer[30];
    char buffer2[30];
    test_memset(buffer,buffer2,9,20);
    test_memset(buffer, buffer2, 255, 10);
    return 0;
}
*/
