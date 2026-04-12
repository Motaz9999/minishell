/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 20:16:20 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/13 01:04:59 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_key_value(char *key, t_env *env_list, int last_exit_status)
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

// now i have 3 strs just join them and return the join
static char	*replace_str_helper_2(char *expand_it, t_expander *exp1)
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

// anything before the $
static char	*replace_str_helper_1(char *expand_it, t_expander *exp1,
		t_shell *shell)
{
	exp1->prefix = ft_substr(expand_it, 0, exp1->dollar_pos);
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
//	exp1.dollar_pos = search_for_special(expand_it);
// where $ located
//	if (expand_it[exp1.key_start] == '?')
// check for exit code here
//	if (exp1.key_len == 0) // no key exist
// return the original word// now after i make sure there is a key and have len

char	*replace_str(char *expand_it, t_shell *shell)
{
	t_expander	exp1;

	ft_memset(&exp1, 0, sizeof(exp1));
	exp1.dollar_pos = search_for_special(expand_it);
	if (exp1.dollar_pos < 0)
		return (expand_it);
	if (expand_it[exp1.dollar_pos] != '$')
		return (expand_it);
	exp1.key_start = exp1.dollar_pos + 1;
	exp1.key_len = 0;
	if (expand_it[exp1.key_start] == '?')
		exp1.key_len = 1;
	else
	{
		while (expand_it[exp1.key_start + exp1.key_len] != '\0'
			&& check_valid_key_char(expand_it[exp1.key_start + exp1.key_len]))
			exp1.key_len++;
	}
	if (exp1.key_len == 0)
		return (expand_it);
	exp1.key = cut_key(expand_it + exp1.dollar_pos + 1, exp1.key_len);
	if (exp1.key == NULL)
		return (expand_it);
	return (replace_str_helper_1(expand_it, &exp1, shell));
}
