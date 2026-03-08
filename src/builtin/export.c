/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 06:14:01 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/08 16:35:40 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_key_env_helper(char *value, t_env *ptr, char *key)
{
	if (value != NULL)
	{
		free(ptr->value);
		ptr->value = value;
	}
	else
		free(value);
	free(key);
}
// this add the key into the env list if it not exist and update
// it if it exist
int	add_key_env(t_env **env_list, char *key, char *value)
{
	t_env	*ptr;
	t_env	*new;

	ptr = *env_list;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, key) == 0)
		{
			add_key_env_helper(value, ptr, key);
			return (TRUE);
		}
		ptr = ptr->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return (FALSE);
	new->key = key;
	new->value = value;
	new->next = *env_list;
	*env_list = new;
	return (TRUE);
}

// this is for updating env_list add or change
// if it new it creat it  export  NEWVAR => just make node and give it the key
// ok first case is no args with cmd
int	export(char **args, t_shell *shell)
{
	int	args_count;

	args_count = find_args_count(args);
	if (args_count == 1)
		return (print_all_env_in_order(shell->env_list));
	else
		return (update_env_export(args, &shell->env_list));
}
