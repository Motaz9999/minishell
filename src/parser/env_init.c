/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 09:32:15 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Splits "KEY=VALUE" into a new t_env node
static t_env	*env_entry_to_node(const char *entry)
{
	t_env	*node;
	int		eq;

	eq = 0;
	while (entry[eq] && entry[eq] != '=')
		eq++;
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_substr(entry, 0, eq);
	if (entry[eq] == '=')
		node->value = ft_strdup(entry + eq + 1);
	else
		node->value = ft_strdup("");
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

// Builds a t_env linked list from the raw envp array passed to main
t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*curr;
	t_env	*node;
	int		i;

	head = NULL;
	curr = NULL;
	i = 0;
	while (envp && envp[i])
	{
		node = env_entry_to_node(envp[i]);
		if (!node)
			return (head);
		if (!head)
		{
			head = node;
			curr = head;
		}
		else
		{
			curr->next = node;
			curr = curr->next;
		}
		i++;
	}
	return (head);
}
