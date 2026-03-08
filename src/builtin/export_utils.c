/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 16:06:23 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/08 16:09:01 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// custom error handler from export
int	error_cmd_export(char *input)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(input, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (FALSE);
}

int	check_key_exist(t_env *env_list, char *key)
{
	t_env	*ptr;
	int		flag;

	flag = FALSE;
	ptr = env_list;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, key) == 0)
		{
			flag = TRUE;
			break ;
		}
		ptr = ptr->next;
	}
	return (flag);
}
