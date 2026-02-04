/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 17:36:42 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:42:57 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	len;

	len = ft_strlen(s);
	write(fd, s, len);
	write(fd, "\n", 1);
}
/*
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>

int	main(void)
{
	int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (1);

	ft_putendl_fd("Hello, World!", fd);  // تكتب النص + \n
	close(fd);

	// الطباعة على الشاشة:
	ft_putendl_fd("Printed on screen", 1);
	ft_putendl_fd("Printed on screen", 1);

	return (0);
}*/
