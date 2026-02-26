/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cpy_free_lst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 12:30:34 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/26 15:13:25 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lst_generic.h"

// get the next pointer of any node using the offset
static void	*get_next(void *node, size_t offset)
{
	return (*(void **)((char *)node + offset));
}

// helper: set the next pointer of any node using the offset
static void	set_next(void *node, size_t offset, void *value)
{
	*(void **)((char *)node + offset) = value;
}

/*
** ft_lstcopy_generic
** walks the list, calls ops.dup on each node to deep copy it,
** wires the new nodes together, returns the new head.
** if any dup fails, frees everything so far and returns NULL.
*/
void	*ft_lstcopy_generic(void *head, t_lst_ops ops)
{
	void	*new_head;
	void	*prev;
	void	*new_node;
	void	*current;

	new_head = NULL;
	prev = NULL;
	current = head;
	while (current)
	{
		new_node = ops.dup(current);
		if (!new_node)
		{
			ft_lstfree_generic(new_head, ops);
			return (NULL);
		}
		if (!new_head)
			new_head = new_node;
		if (prev)
			set_next(prev, ops.next_offset, new_node);
		prev = new_node;
		current = get_next(current, ops.next_offset);
	}
	return (new_head);
}

/*
** ft_lstfree_generic
** walks the list, calls ops.del on each node (to free inner pointers),
** then frees the node itself.
*/
void	ft_lstfree_generic(void *head, t_lst_ops ops)
{
	void	*current;
	void	*next;

	current = head;
	while (current)
	{
		next = get_next(current, ops.next_offset);
		if (ops.del)
			ops.del(current);
		free(current);
		current = next;
	}
}
