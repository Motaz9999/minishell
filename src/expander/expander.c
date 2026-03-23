/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 17:50:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 20:16:41 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_key_value(char *key, t_env *env_list, int last_exit_status)
{
	t_env	*node;
	char	*value;

	if (ft_strcmp(key, "?") == 0)
		value = ft_itoa(last_exit_status);
	else
	{
		node = find_node(env_list, key);
		if (node && node->value)
			value = ft_strdup(node->value);
		else
			value = ft_strdup("");
	}
	if (!value)
		return (NULL);
	return (value);
}

// check on chars that is the same form as key in env list
int	check_valid_key_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (TRUE);
	return (FALSE);
}



// Yes, all expansions start with $, but NO it doesn't have to be at the start:
// echo $HOME          # start → expands
// echo hello$HOME     # middle → expands
// echo hello$HOME/end # middle → expands
// it return false if there nothing to expand and len if true
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
// ok this fun is for expanding the cmd (replace it with new one that have more info)
// anything u want to expand must start with $ if it not exist just skip it and return the original str
static char	*expand_cmd(char *expand_it, t_shell *shell)
{
	char	*new_one;

	while (search_for_special(expand_it) >= 0)
	{
		new_one = replace_str(expand_it, shell);
		if (!new_one)
			return (expand_it);
		if (new_one == expand_it)
			break ;
		expand_it = new_one;
	}
	return (expand_it);
}
// first what i well deal with is shell and cmds
// this fun is used once for each cmd
void	expand_args_from_cmd(t_shell *shell, t_command *cmd)
{
	int	i;

	if (!shell || !cmd || !cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		if (cmd->quote_types[i] == SINGLE_QUOTE) // skip here
		{
			i++;
			continue ;
		} // double and none i want to expand it
		cmd->args[i] = expand_cmd(cmd->args[i], shell);
		i++;
	}
}
