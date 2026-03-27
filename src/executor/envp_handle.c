/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:05:49 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/27 21:10:22 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	if (env == NULL)
		return (count);
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

// this fun make envp and merge the envp list i well be givin
// so see the size of the envp list
// the head is here
char	**make_envp(t_env *envp_list)
{
	char	**envp;
	int		i;
	int		count;
	t_env	*curr;

	count = count_env_vars(envp_list);
	envp = malloc(sizeof(char *) * (count + 1));
	ft_memset(envp, 0, sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	curr = envp_list;
	while (curr)
	{
		envp[i] = ft_merge_join(curr->key, "=", curr->value);
		if (envp[i] == NULL)
		{
			ft_free_all2((void **)envp, NULL);
			return (NULL);
		}
		curr = curr->next;
		i++;
	}
	return (envp);
}
