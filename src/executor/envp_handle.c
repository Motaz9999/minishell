/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:05:49 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/18 23:02:07 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

// this fun make envp and merge the envp list i well be givin
char	**make_envp(t_env *envp_list)
{
	char	**envp;
	int		i;
	int count;   // so see the size of the envp list
	t_env *curr; // the head is here

    count = count_env_vars(envp_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	curr = envp_list;
	while (curr)
	{
		envp[i] = ft_strjoin(envp_list->key, envp_list->value);
		if (envp[i] == NULL)
		{
			ft_free_all2((void **)envp, NULL);
			return (NULL);
		}
		curr = curr->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
