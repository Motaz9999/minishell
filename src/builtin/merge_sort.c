/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:11:54 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/26 15:25:10 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// i will use FAST AND SLOW Pointer technique called Tortoise and Hare algorithm
// O(N) time complexity without having to count the total number of nodes first.
// slow and fast start from the head
static void	split_into_half(t_env *head, t_env **left, t_env **right)
{
	t_env	*slow;
	t_env	*fast;

	if (head == NULL || head->next == NULL)
	{
		*left = head;
		*right = NULL;
		return ;
	}
	slow = head;
	fast = head->next;
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*left = head;
	*right = slow->next;
	slow->next = NULL;
}

static t_env	*sorted_merge(t_env *left, t_env *right)
{
	t_env	*head;

	head = NULL;
	if (left == NULL)
		return (right);
	if (right == NULL)
		return (left);
	if (ft_strcmp(left->key, right->key) <= 0)
	// this mean right < left// right then left// merge right then left
	{
		head = left;
		head->next = sorted_merge(head->next, right);
	}
	else // right > left // merge left then right
	{
		head = right;
		head->next = sorted_merge(left, right->next);
	}
	return (head);
}

// here  i just have a linked list of env vars
// and i want to sort it
// the bigO(n*log n)
// the merge sort
// btw this is list is just a copy
// the head ref is always changed but the head is FOR EACH TIME U CALL WE HAVE NEW HEAD
// bc its new linked list
void	merge_sort(t_env **head_ref)
{
	t_env	*head;
	t_env	*right;
	t_env	*left;

	head = *head_ref;
	if (*head_ref == NULL || (*head_ref)->next == NULL)
		return ;
	right = NULL;
	left = NULL;
	split_into_half(head, &left, &right);
	merge_sort(&left);
	merge_sort(&right);
	*head_ref = sorted_merge(left, right);
}
