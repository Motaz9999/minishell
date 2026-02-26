/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 06:14:01 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/26 15:33:46 by moodeh           ###   ########.fr       */
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
			printf("declare -x %s=\"%s\"\n", print->key, print->value);// VAR=
		print = print->next;
	}
	ft_lstfree_generic(copy, ops); // free it all
	return (TRUE);// in case we print everything we must print
}

// this is for updating env_list add or change
// if it new it creat it  export  NEWVAR => just make node and give it the key
// ok first case is no args with cmd
int	export(char **args, t_shell *shell)
{
	int	args_count;

	args_count = find_args_count(args);
	if (args_count == 1)// in this case we need to print all the env vars with specific format AND in ASCII order
		return (print_all_env_in_order(shell->env_list));
}
