/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:26:49 by moodeh            #+#    #+#             */
/*   Updated: 2025/08/19 14:42:16 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*delete;

	if (!lst || !del)
		return ;
	while ((*lst) != NULL)
	{
		delete = *lst;
		*lst = (*lst)->next;
		del(delete->content);
		free(delete);
		delete = NULL;
	}
}
