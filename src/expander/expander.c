/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 17:50:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/25 02:58:51 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_for_special(char *word)
{
	int	i;

	if (!word)
		return (-1);
	i = 0;
	while (word[i] != '\0' && word[i] != '$')
		i++;
	if (word[i] == '$')
		return (i);
	return (-1);
}

static void	restore_protected_dollars(char *arg)
{
	int	i;

	if (!arg)
		return ;
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '\1')
			arg[i] = '$';
		i++;
	}
}

char	*expand_cmd(char *expand_it, t_shell *shell)
{
	char	*new_one;

	if (expand_it == NULL || *expand_it == '\0')
		return (expand_it);
	while (search_for_special(expand_it) >= 0)
	{
		new_one = replace_str(expand_it, shell);
		if (!new_one)
			return (expand_it);
		if (new_one == expand_it)
			break ;
		expand_it = new_one;
	}
	restore_protected_dollars(expand_it);
	return (expand_it);
}

static void	remove_empty_unquoted_args(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd->args[i])
	{
		if (cmd->quote_types[i] == NO_QUOTE && cmd->args[i][0] == '\0')
		{
			free(cmd->args[i]);
			j = i;
			while (cmd->args[j + 1])
			{
				cmd->args[j] = cmd->args[j + 1];
				cmd->quote_types[j] = cmd->quote_types[j + 1];
				j++;
			}
			cmd->args[j] = NULL;
			cmd->quote_types[j] = NO_QUOTE;
			continue ;
		}
		i++;
	}
}

void	expand_args_from_cmd(t_shell *shell, t_command *cmd)
{
	int	i;

	if (!shell || !cmd || !cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		cmd->args[i] = expand_cmd(cmd->args[i], shell);
		i++;
	}
	remove_empty_unquoted_args(cmd);
}
