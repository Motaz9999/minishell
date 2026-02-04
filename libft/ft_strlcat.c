/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:21:13 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:24 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
//#include <string.h>
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t		len_dest;
	size_t		len_src;
	size_t		i;

	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	if (len_dest >= size)
		return (size + len_src);
	i = 0;
	while (src[i] && (len_dest + i < size - 1))
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = '\0';
	return (len_dest + len_src);
}
/*
size_t my_strlcat(char *dst, const char *src, size_t dstsize)
{
    size_t dlen = strlen(dst);
    size_t slen = strlen(src);
    size_t i;

    if (dstsize <= dlen)
        return slen + dstsize;

    i = 0;
    while (src[i] && dlen + i < dstsize - 1)
    {
        dst[dlen + i] = src[i];
        i++;
    }
    dst[dlen + i] = '\0';
    return dlen + slen;
}

void test_strlcat(char *dst_ft, char *dst_std, const char *src, size_t size)
{
    size_t ret_ft = ft_strlcat(dst_ft, src, size);
    size_t ret_std = my_strlcat(dst_std, src, size);

    printf("dstsize = %zu\n", size);
    printf("ft_strlcat result:  %s (returned: %zu)\n", dst_ft, ret_ft);
    printf("strlcat   result:  %s (returned: %zu)\n", dst_std, ret_std);

    if (strcmp(dst_ft, dst_std) == 0 && ret_ft == ret_std)
        printf("✅ Test passed\n");
    else
        printf("❌ Test failed\n");
    printf("-------------------------\n");
}

int main(void)
{
    char buf1[20];
    char buf2[20];

    // الحالة 1: حجم كافي
    strcpy(buf1, "Hello, ");
    strcpy(buf2, "Hello, ");
    test_strlcat(buf1, buf2, "42!", 20);

    // الحالة 2: حجم غير كافي
    strcpy(buf1, "Hello, ");
    strcpy(buf2, "Hello, ");
    test_strlcat(buf1, buf2, "World!", 10);

    // الحالة 3: دمج مع نص فارغ
    strcpy(buf1, "");
    strcpy(buf2, "");
    test_strlcat(buf1, buf2, "Test", 10);

    // الحالة 4: دمج نص مع نص فارغ
    strcpy(buf1, "Data");
    strcpy(buf2, "Data");
    test_strlcat(buf1, buf2, "", 10);

    // الحالة 5: dstsize = 0
    strcpy(buf1, "Hello");
    strcpy(buf2, "Hello");
    test_strlcat(buf1, buf2, "World", 0);

    return 0;
}
	*/
