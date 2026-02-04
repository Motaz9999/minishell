/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:22:05 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:41:27 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	num;
	int	sign;

	num = 0;
	sign = 1;
	if (!str)
		return (0);
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str != '\0' && (*str >= 48 && *str <= 57))
	{
		if (num > 214748364 && (*str - '0') > 7 && sign == 1)
			return (-1);
		if (num > 214748364 && (*str - '0') > 8 && sign == -1)
			return (0);
		num = num * 10 + (*str - '0');
		str++;
	}
	return (sign * num);
}
/*
#include <stdio.h>
#include <stdlib.h> // for original atoi

int	main(void)
{
	char *tests[] = {
		"42",
		"   42",
		"   -42",
		"+42",
		"--42",
		"4193 with words",
		"words and 987",
		"2147483647",    // INT_MAX
		"-2147483648",   // INT_MIN
		"2147483648",    // Overflow
		"-2147483649",   // Underflow
		"",
		"   +0",
		"0000123",
		NULL
	};
	for (int i = 0; tests[i] != NULL; i++)
	{
		printf("Input: \"%s\"\n", tests[i]);
		printf("ft_atoi: %d | atoi: %d\n", ft_atoi(tests[i]), atoi(tests[i]));
		printf("-----------------------------\n");
	}
	return (0);
}

*/
