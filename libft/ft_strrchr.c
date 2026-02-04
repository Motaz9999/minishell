/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:06:41 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:42 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>
char	*ft_strrchr(const char *s, int c)
{
	int			str_len;

	if (!s)
		return (NULL);
	str_len = (int)ft_strlen(s);
	while (str_len >= 0)
	{
		if (s[str_len] == (char)c)
			return ((char *)(s + str_len));
		str_len--;
	}
	return (NULL);
}
/*


int main(void)
{
    const char *str = "HelloWorld";
    char *res;

    res = ft_strrchr(str, 'l');
    if (res)
        printf("%s\n", res);

    res = ft_strrchr(str, 'H');
    if (res)
        printf("%s\n", res);

    res = ft_strrchr(str, 'z');
    if (res)
        printf("%s\n", res);
    else
        printf("Character not found!\n");

    res = ft_strrchr(str, '\0');
    if (res)
        printf("Found null terminator at the end of string.\n");
}*/
