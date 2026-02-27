/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:19:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 12:13:08 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_args_count(char **args)
{
	int	count;

	count = 1; // bc we are at (after the cmd)
	if (args == NULL || *args == NULL)
		return (0);
	while (args[count] != NULL)
		count++;
	return (count);
}

// deep copies ONE node
// must: malloc, copy all fields, set next = NULL, return it
void	*dup_env(void *raw)
{
	t_env	*node;
	t_env	*new;

	node = (t_env *)raw;
	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(node->key);
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	if (node->value)
	{
		new->value = ft_strdup(node->value);
		if (!new->value)
		{
			free(new->key);
			free(new);
			return (NULL);
		}
	}
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	del_env(void *raw)
{
	t_env	*node;

	node = (t_env *)raw;
	free(node->key);
	free(node->value);
}

// returns a malloc'd copy of the key portion: arg[0..cut-1]
char	*cut_key(char *arg, int cut)
{
	return (ft_substr(arg, 0, cut));
}

// returns a malloc'd copy of the value portion after '=',
// or NULL if there is no '=' (bare export VAR with no value)
char	*cut_value(char *arg, int cut)
{
	if (arg[cut] != '=')
		return (NULL);
	return (ft_strdup(arg + cut + 1));
}