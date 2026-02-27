/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:33:35 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/25 05:56:55 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_node(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

// this fun for updating the env list
// its update the PWD and OLD PWD
void	update_env_pwd(t_env *env_list, char *new_pwd_val)
{
	t_env	*node_pwd;
	t_env	*node_old;

	node_pwd = find_node(env_list, "PWD");
	node_old = find_node(env_list, "OLDPWD");
	if (node_old && node_pwd)
	{
		free(node_old->value);
		node_old->value = node_pwd->value ? ft_strdup(node_pwd->value) : NULL;
	}
	if (node_pwd && new_pwd_val)
	{
		free(node_pwd->value);
		node_pwd->value = ft_strdup(new_pwd_val);
	}
}

int	pwd(t_env *env_list)
{
	char	*path;
	t_env	*node;

	node = find_node(env_list, "PWD");
	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		if (node && node->value)
		{
			ft_printf("%s\n", node->value);
			return (FALSE);
		}
		return (error_syscall("getcwd", 0));
	}
	ft_printf("%s\n", path);
	free(path);
	return (TRUE);
}
