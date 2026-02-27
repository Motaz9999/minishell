/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 06:14:01 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 06:43:58 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// print all the env vars with specific format AND in ASCII order
// 1- we make a new env_list and sort it using (...)
// for the sorting we want sort using ASCII
// 2- print declare-x
//		to every variable and puts double quotes around the value
// If the variable has a value: declare -x USER="moulinette"
// If the variable is empty (VAR=): declare -x VAR=""
// If the variable was exported without a value (export VAR):
// declare -x VAR (notice there is no = and no quotes).
int	print_all_env_in_order(t_env *env_list)
{
	t_env	*copy;
	t_env	*print;

	t_lst_ops ops; // this for copying
	ops.del = del_env;
	ops.dup = dup_env;
	ops.next_offset = offsetof(t_env, next);
	copy = ft_lstcopy_generic(env_list, ops);
	if (!copy)
		return (FALSE);
	merge_sort(&copy); // sort it
	print = copy;
	while (print != NULL)
	{
		if (print->value == NULL)
			printf("declare -x %s\n", print->key); // export VAR
		else if (print->value[0] == '\0')
			printf("declare -x %s=\"\"\n", print->key); // VAR=
		else
			printf("declare -x %s=\"%s\"\n", print->key, print->value); // VAR=
		print = print->next;
	}
	ft_lstfree_generic(copy, ops); // free it all
	return (TRUE);                 // in case we print everything we must print
}

// this fun is for checking for the input rules
// first we need no see the whole word or till i find =
// it return where to cut
int	valid_arg(char *arg)
{
	int	i;

	i = 0;
	// if this is null this mean i just have name
	if (!(ft_isalpha(arg[i]) || arg[i] == '_'))
		// must check the first letter A-Za-z or _
		return (FALSE);
	i++;
	while (arg[i] != '\0')
	{
		if (arg[i] == '=')
			break ; // end the key
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (FALSE);
		i++;
	}
	return (i);
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

//this add the key into the env list if it not exist and update
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
			if (value != NULL)
			{
				free(ptr->value);
				ptr->value = value;
			}
			else
				free(value);
			free(key);
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

// just add new elements to the env list  OR update if key already exist
// it can change or add multiple env VARS
int	update_env_export(char **args, t_env **env_list)
{
	int		i;
	char	*key;
	char	*value;
	int		cut;
	int		ret;

	i = 1;
	ret = TRUE;
	while (args && args[i])
	{
		cut = valid_arg(args[i]);
		if (cut == FALSE)
		{
			error_cmd_export(args[i]);
			ret = FALSE;
			i++;
			continue ;
		}
		key = cut_key(args[i], cut);
		if (key == NULL)
		{
			error_cmd_export(args[i]);
			ret = FALSE;
			i++;
			continue ;
		}
		value = cut_value(args[i], cut);
		if (!add_key_env(env_list, key, value))
		{
			ft_free_all((void *)key, (void *)value, NULL);
			error_cmd_export(args[i]);
			ret = FALSE;
		}
		i++;
	}
	return (ret);
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
