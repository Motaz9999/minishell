/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:03:17 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:27 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t		len_src;
	size_t		i;

	i = 0;
	len_src = ft_strlen(src);
	if (size <= 0)
		return (len_src);
	while (*src != '\0' && i < size - 1)
	{
		*dest = *src;
		src++;
		dest++;
		i++;
	}
	*dest = '\0';
	return (len_src);
}
/*
#include <stdio.h>
#include <string.h>

int main()
{
    char dest[6];
    const char *src = "HelloWorld";
    size_t ret;

    ret = ft_strlcpy(dest, src, sizeof(dest));

    printf("Copied string: %s\n", dest);
    printf("Returned length of src: %zu\n", ret);

    return 0;
}
#include <stdio.h>
#include <string.h>

int main(void)
{
    char src[] = "Hello";
    char dst[3];

    size_t ret = ft_strlcpy(dst, src, sizeof(dst));

    printf("Return = %zu\n", ret);
    printf("Dst = \"%s\"\n", dst);
}*/
