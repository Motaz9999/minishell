/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 03:07:21 by moodeh            #+#    #+#             */
/*   Updated: 2025/12/23 05:40:37 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// this fun is designed to free what ever args you want its works like printf
//This only frees heap-allocated pointers (from malloc, calloc, realloc).
//Always terminate with NULL
void	ft_free_all(void *first, ...)
{
	void	*ptr;
	va_list	args_to_free;

	if (first == NULL)
		return ;
	va_start(args_to_free, first);
	ptr = first;
	while (ptr != NULL)
	{
		free(ptr);
		ptr = va_arg(args_to_free, void *);
	}
	va_end(args_to_free);
}

// this fun is to free ** like split
//Only use this on null-terminated arrays, like what ft_split creates.
void	ft_free_all2(void **first, ...)
{
	void	**ptr;
	int		i;
	va_list	args_to_free;

	if (first == NULL)
		return ;
	va_start(args_to_free, first);
	ptr = first;
	while (ptr != NULL)
	{
		i = 0;
		while (ptr[i] != NULL)
		{
			free(ptr[i]);
			i++;
		}
		free(ptr);
		ptr = va_arg(args_to_free, void **);
	}
	va_end(args_to_free);
}
