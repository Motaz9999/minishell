/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:12:35 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/08 16:27:27 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// just add new elements to the env list  OR update if key already exist
// it can change or add multiple env VARS

int	update_env_export_helper(char **args, t_env **env_list, t_update_env *u_env)
{
	u_env->cut = valid_arg(args[u_env->i]);
	if (u_env->cut == FALSE)
	{
		error_cmd_export(args[u_env->i]);
		u_env->ret = FALSE;
		u_env->i++;
		return (FALSE);
	}
	u_env->key = cut_key(args[u_env->i], u_env->cut);
	if (u_env->key == NULL)
	{
		error_cmd_export(args[u_env->i]);
		u_env->ret = FALSE;
		u_env->i++;
		return (FALSE);
	}
	return (TRUE);
}

int	update_env_export(char **args, t_env **env_list)
{
	t_update_env	u_env;

	u_env.i = 1;
	u_env.ret = TRUE;
	while (args && args[u_env.i])
	{
		if (update_env_export_helper(args, env_list, &u_env) == FALSE)
			continue ;
		u_env.value = cut_value(args[u_env.i], u_env.cut);
		if (!add_key_env(env_list, u_env.key, u_env.value))
		{
			ft_free_all((void *)u_env.key, (void *)u_env.value, NULL);
			error_cmd_export(args[u_env.i]);
			u_env.ret = FALSE;
		}
		u_env.i++;
	}
	return (u_env.ret);
}
