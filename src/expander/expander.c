/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 17:50:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 20:12:14 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_for_special(char *word);

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

// now i have 3 strs just join them and return the join
char	*replace_str_helper_2(char *expand_it, t_expander *exp1)
{
	exp1->new_one = ft_merge_join(exp1->prefix, exp1->value, exp1->suffix);
	if (exp1->new_one == NULL)
	{
		ft_free_all((void *)exp1->key, (void *)exp1->prefix,
			(void *)exp1->suffix, (void *)exp1->value, NULL);
		return (expand_it);
	}
	ft_free_all((void *)exp1->key, (void *)exp1->prefix, (void *)exp1->suffix,
		(void *)exp1->value, (void *)expand_it, NULL);
	return (exp1->new_one);
}

char	*replace_str_helper_1(char *expand_it, t_expander *exp1, t_shell *shell)
{
	exp1->prefix = ft_substr(expand_it, 0, exp1->dollar_pos);
	// anything before the $
	if (!exp1->prefix)
	{
		ft_free_all((void *)exp1->key, NULL);
		return (expand_it);
	}
	exp1->suffix = ft_substr(expand_it, exp1->key_start + exp1->key_len,
			ft_strlen(expand_it + exp1->key_start + exp1->key_len));
	if (!exp1->suffix)
	{
		ft_free_all((void *)exp1->key, (void *)exp1->prefix, NULL);
		return (expand_it);
	}
	exp1->value = extract_key_value(exp1->key, shell->env_list,
			shell->last_exit_status);
	if (!exp1->value)
	{
		ft_free_all((void *)exp1->key, (void *)exp1->prefix,
			(void *)exp1->suffix, NULL);
		return (expand_it);
	}
	return (replace_str_helper_2(expand_it, exp1));
}

char	*replace_str(char *expand_it, t_shell *shell)
{
	t_expander	exp1;

	ft_memset(&exp1, 0, sizeof(exp1));
	exp1.dollar_pos = search_for_special(expand_it); // where $ located
	if (exp1.dollar_pos < 0)
		return (expand_it);
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
	if (exp1.key_len == 0) // no key exist
		return (expand_it);
	// return the original word// now after i make sure there is a key and have len
	exp1.key = cut_key(expand_it + exp1.dollar_pos + 1, exp1.key_len);
	if (exp1.key == NULL)
		return (expand_it);
	return (replace_str_helper_1(expand_it, &exp1, shell));
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
char	*expand_cmd(char *expand_it, t_shell *shell)
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
