/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:42:34 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:43:00 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n >= 10)
		ft_putnbr_fd((n / 10), fd);
	ft_putchar_fd(((n % 10) + '0'), fd);
}
/*
#include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int fd = open("nbr_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open failed");
        return 1;
    }

    int numbers[] = {0, 123, -456, INT_MAX, INT_MIN};
    size_t count = sizeof(numbers) / sizeof(numbers[0]);

    write(1, "Printing numbers to screen:\n", 28);
    for (size_t i = 0; i < count; i++)
    {
        ft_putnbr_fd(numbers[i], 1);
        write(1, "\n", 1);
    }

    write(1, "\nPrinting numbers to file:\n", 27);
    for (size_t i = 0; i < count; i++)
    {
        ft_putnbr_fd(numbers[i], fd);
        write(fd, "\n", 1);
    }

    close(fd);

    // تحقق من الملف باستخدام system (اختياري)
    system("echo 'Content of nbr_output.txt:'; cat nbr_output.txt");

    return 0;
}
*/
