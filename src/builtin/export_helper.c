/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:19:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/26 15:19:52 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	new->value = ft_strdup(node->value);
	new->next = NULL;
	return (new);
}

void	del_env(void *raw)
{
	t_env *node = (t_env *)raw;

	free(node->key);
	free(node->value);
}