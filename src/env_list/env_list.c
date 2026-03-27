/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 20:21:44 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/27 19:20:11 by moodeh           ###   ########.fr       */
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

//this fun is the one it create node
// each time it have 1 part of the envp
//then make it a node
//if one is not true it return false
static int	init_env_add_one(t_env **list, char *entry)
{
	char	*key;
	char	*value;
	int		cut;

	cut = 0;
	while (entry[cut] && entry[cut] != '=')
		cut++;
	key = cut_key(entry, cut);
	if (!key)
		return (FALSE);
	value = cut_value(entry, cut);	
	if (entry[cut] == '=' && !value)
	{
		free(key);
		return (FALSE);
	}
	if (!add_key_env(list, key, value))
	{
		ft_free_all((void *)key, (void *)value, NULL);
		return (FALSE);
	}
	return (TRUE);
}

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*list;

	if (!envp)
		return (NULL);
	list = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (!init_env_add_one(&list, envp[i]))
		{
			free_env_list(list);
			return (NULL);
		}
		i++;
	}
	return (list);
}
