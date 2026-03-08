/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 04:04:27 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/08 16:55:08 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this fun print the whole env list
// return (FALSE); // error
// if (curr->value != NULL) // only print vars that have a value

int	env(t_env *env_list)
{
	t_env	*curr;

	if (env_list == NULL)
		return (FALSE);
	curr = env_list;
	while (curr)
	{
		if (curr->value != NULL)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	return (TRUE);
}
