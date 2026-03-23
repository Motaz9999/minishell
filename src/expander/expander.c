/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 17:50:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 19:27:30 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_key_value(char *key, t_env *env_list, int *len_of_value,
		int last_exit_status)
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
	*len_of_value = ft_strlen(value);
	return (value);
}
//check on chars 
int	check_valid_key_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (TRUE);
	return (FALSE);
}

char	*replace_str_helper_1(char *expand_it, t_expander *exp1, t_shell *shell)
{
}

char	*replace_str(char *expand_it, t_shell *shell)
{
	t_expander	exp1;

	ft_memset(exp1, 0, sizeof(exp1));
	exp1.dollar_pos = search_for_special(expand_it); // where $ located
	if (expand_it[exp1.dollar_pos] != '$')
		return (expand_it);
	exp1.key_start = exp1.dollar_pos + 1;
	exp1.key_len = 0;
	if (expand_it[exp1.key_start] == '?') // check for exit code here
		exp1.key_len = 1;
	else
	{
		while (expand_it[exp1.key_start + exp1.key_len] != '\0'
			&& check_valid_key_char(expand_it[exp1.key_start + exp1.key_len]))
			exp1.key_len++;
	}
	if (exp1.key_len == 0)  // no key exist
		return (expand_it); // return the original word
	// now after i make sure there is a key and have len
	return (replace_str_helper_1(expand_it, &exp1 , shell));
}

// Yes, all expansions start with $, but NO it doesn't have to be at the start:
// echo $HOME          # start → expands
// echo hello$HOME     # middle → expands
// echo hello$HOME/end # middle → expands
// it return false if there nothing to expand and len if true
int	search_for_special(char *word)
{
	int	flag;
	int	i;

	i = 0;
	flag = FALSE;
	while (word[i] != '\0' && word[i] != '$')
		i++;
	if (word[i] != '\0' && word[i] == '$')
		flag = i;
	return (flag);
}
// ok this fun is for expanding the cmd (replace it with new one that have more info)
// anything u want to expand must start with $ if it not exist just skip it and return the original str
char	*expand_cmd(char *expand_it, t_shell *shell)
{
	if (search_for_special(expand_it) != FALSE)
		expand_it = replace_str(expand_it, ft_strlen(expand_it), shell);
	return (expand_it);
}
// first what i well deal with is shell and cmds
// this fun is used once for each cmd
void	expand_args_from_cmd(t_shell *shell, t_command *cmd)
{
	int	i;

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
