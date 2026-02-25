/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 04:04:27 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/25 04:13:00 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this fun print the whole env list
int	env(t_env *env_list)
{
	t_env *curr;

	if (env_list == NULL)
		return (FALSE); // error
	curr = env_list;
	while (curr)
	{
		printf("%s=%s\n", curr->key, curr->value); // print the key + value
		curr = curr->next;
	}
	return (TRUE);
}