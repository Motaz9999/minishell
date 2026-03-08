/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:19:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/08 16:57:02 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	count = 1; // bc we are at (after the cmd)
int	find_args_count(char **args)
{
	int	count;

	count = 1;
	if (args == NULL || *args == NULL)
		return (0);
	while (args[count] != NULL)
		count++;
	return (count);
}

static int	dup_env_helper(t_env *node, t_env *new)
{
	new->value = ft_strdup(node->value);
	if (!new->value)
	{
		free(new->key);
		free(new);
		return (FALSE);
	}
	return (TRUE);
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
		if (!dup_env_helper(node, new))
			return (NULL);
	}
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

// print all the env vars with specific format AND in ASCII order
// 1- we make a new env_list and sort it using (...)
// for the sorting we want sort using ASCII
// 2- print declare-x
//		to every variable and puts double quotes around the value
// If the variable has a value: declare -x USER="moulinette"
// If the variable is empty (VAR=): declare -x VAR=""
// If the variable was exported without a value (export VAR):
// declare -x VAR (notice there is no = and no quotes).
//	t_lst_ops ops; // this for copying
//	merge_sort(&copy); // sort it
//			printf("declare -x %s\n", print->key); // export VAR
//			printf("declare -x %s=\"\"\n", print->key); // VAR=
//	ft_lstfree_generic(copy, ops); // free it all
//	return (TRUE);                 // in case we print everything we must print

int	print_all_env_in_order(t_env *env_list)
{
	t_env		*copy;
	t_env		*print;
	t_lst_ops	ops;

	ops.del = del_env;
	ops.dup = dup_env;
	ops.next_offset = offsetof(t_env, next);
	copy = ft_lstcopy_generic(env_list, ops);
	if (!copy)
		return (FALSE);
	merge_sort(&copy);
	print = copy;
	while (print != NULL)
	{
		if (print->value == NULL)
			printf("declare -x %s\n", print->key);
		else if (print->value[0] == '\0')
			printf("declare -x %s=\"\"\n", print->key);
		else
			printf("declare -x %s=\"%s\"\n", print->key, print->value);
		print = print->next;
	}
	ft_lstfree_generic(copy, ops);
	return (TRUE);
}

// this fun is for checking for the input rules
// first we need no see the whole word or till i find =
// it return where to cut
// if this is null this mean i just have name
// must check the first letter A-Za-z or _
//			break ; // end the key

int	valid_arg(char *arg)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(arg[i]) || arg[i] == '_'))
		return (FALSE);
	i++;
	while (arg[i] != '\0')
	{
		if (arg[i] == '=')
			break ;
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (FALSE);
		i++;
	}
	return (i);
}
