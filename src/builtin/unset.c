/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 06:14:11 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/25 07:13:10 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this fun is for del the node
// there is 3 loc
// head =>just
// mid
// last
// this fun is for update the env_list   unset args  => del and free and move pointer
void	update_env_unset(t_env **head, char *args)
{
	t_env	*curr;
	t_env	*one_before;

	one_before = NULL; // there is nothing before the head
	curr = *head;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->key, args) == 0) // found the node i want to del
		{
			if (one_before) // first unlink it
				one_before->next = curr->next;
			else
				// it is the first fun must update the head it self
				*head = curr->next; // here we unlink it
			// after we unlink the node we want to del
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		one_before = curr;
		curr = curr->next;
	}
}

// here it just disable any cmd that well have
// it take 0 or 1 or more VAR and unset
int	unset(char **args, t_shell *shell)
{
	char **curr;
	int i;

	i = 0;
	curr = args + 1; // start from after the cmd
	while (curr[i] != NULL)
		update_env_unset(&shell->env_list, curr[i++]);
	return (TRUE);
}