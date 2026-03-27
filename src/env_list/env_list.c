/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 20:21:44 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/27 21:34:06 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this fun is for makeing env_list
// just creat a linked list
void	free_env_list(t_env *list)
{
	t_env	*next;

	while (list)
	{
		next = list->next;
		free(list->key);
		free(list->value);
		free(list);
		list = next;
	}
}

// each time it have 1 part of the envp
// then make it a node
// if one is not true it return false
static int	setup_node(char *entry, char **key, char **value)
{
	int	cut;

	cut = 0;
	while (entry[cut] && entry[cut] != '=')
		cut++;
	*key = cut_key(entry, cut);
	if (!*key)
		return (FALSE);
	*value = cut_value(entry, cut);
	if (entry[cut] == '=' && !*value)
	{
		free(*key);
		return (FALSE);
	}
	return (TRUE);
}

t_env	*new_node(char *envp)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env) * 1);
	if (new_node == NULL)
		return (NULL);
	ft_memset(new_node, 0, sizeof(t_env));
	if (!setup_node(envp, &new_node->key, &new_node->value))
	{
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

//add to the list
void	add_back(t_env **head, t_env *new_node)
{
	t_env	*curr;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new_node;
}

//create new list of env
t_env	*init_env(char **envp)
{
	int		i;
	t_env	*head;
	t_env	*node;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		node = new_node(envp[i]);
		if (new_node == NULL)
		{
			free_env_list(head);
			return (NULL);
		}
		add_back(&head, new_node);
		i++;
	}
	return (head);
}
