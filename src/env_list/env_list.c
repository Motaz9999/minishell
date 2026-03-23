/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 20:21:44 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 21:06:12 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this fun is for makeing env_list
// just creat a linked list

static void	free_env_list(t_env *list)
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

t_env	*init_env(char **envp)
{
	char	*key;
	char	*value;
	int		i;
	int		cut;
	t_env	*list;

	list = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		cut = 0;
		while (envp[i][cut] && envp[i][cut] != '=')
			cut++;
		key = cut_key(envp[i], cut);
		value = cut_value(envp[i], cut);
		if (!key || !add_key_env(&list, key, value))
		{
			ft_free_all((void *)key, (void *)value, NULL);
			free_env_list(list);
			return (NULL);
		}
		i++;
	}
	return (list);
}
