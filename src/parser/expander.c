/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 09:32:15 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Appends a string s into buf at position *j
static void	append_str(char *buf, int *j, const char *s)
{
	while (s && *s)
		buf[(*j)++] = *s++;
}

// Reads a $? or $VARNAME and expands it into buf at position *j
// Returns updated i after consuming the variable reference
static int	expand_var(const char *str, int i, char *buf, int *j,
			t_shell *shell)
{
	char	varname[256];
	int		k;
	t_env	*node;
	char	*val;

	i++; // skip '$'
	if (str[i] == '?')
	{
		val = ft_itoa(shell->last_exit_status);
		append_str(buf, j, val);
		free(val);
		return (i + 1);
	}
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		k = 0;
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
			varname[k++] = str[i++];
		varname[k] = '\0';
		node = find_node(shell->env_list, varname);
		if (node && node->value)
			append_str(buf, j, node->value);
		return (i);
	}
	// bare '$' with no valid var name — keep literal
	buf[(*j)++] = '$';
	return (i);
}

// Expands $VAR and $? in a string (used for unquoted and double-quoted tokens)
static char	*expand_str(const char *str, t_shell *shell)
{
	char	buf[4096];
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			i = expand_var(str, i, buf, &j, shell);
		else
			buf[j++] = str[i++];
	}
	buf[j] = '\0';
	return (ft_strdup(buf));
}

// Walks through all tokens and expands variables where allowed:
//   - quote_type 1 (single quotes) → no expansion
//   - quote_type 0 or 2 (unquoted / double-quoted) → expand $VAR and $?
void	expand_tokens(t_token *tokens, t_shell *shell)
{
	t_token	*tok;
	char	*expanded;

	tok = tokens;
	while (tok)
	{
		if (tok->type == TOKEN_WORD && tok->quote_type != 1)
		{
			expanded = expand_str(tok->value, shell);
			free(tok->value);
			tok->value = expanded;
		}
		tok = tok->next;
	}
}
