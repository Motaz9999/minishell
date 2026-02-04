/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:41:30 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:42:04 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lenint(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		len++;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_fillstr(int len, int sign, int n)
{
	char	*s;

	if (sign == 1)
		len++;
	s = (char *)malloc(len + 1);
	if (!s)
		return (NULL);
	s[len] = '\0';
	if (sign == 1)
		s[0] = '-';
	if (n == 0)
		s[--len] = '0';
	while (n > 0)
	{
		s[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		sign;

	sign = 0;
	if (-2147483648 == n)
		return (ft_strdup("-2147483648"));
	if (n < 0)
	{
		n *= -1;
		sign = 1;
	}
	len = ft_lenint(n);
	str = ft_fillstr(len, sign, n);
	return (str);
}
/*
#include <stdio.h>
#include <limits.h>

int main(void)
{
    int numbers[] = {0, 123, -123, 42, -42, INT_MAX, INT_MIN + 1};
    size_t size = sizeof(numbers) / sizeof(numbers[0]);
    char *str;
    
    for (size_t i = 0; i < size; i++)
    {
        str = ft_itoa(numbers[i]);
        if (str)
        {
            printf("ft_itoa(%d) = %s\n", numbers[i], str);
            free(str);
        }
        else
        {
            printf("ft_itoa(%d) returned NULL\n", numbers[i]);
        }
    }
    return 0;
}*/
