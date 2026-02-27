/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 09:32:15 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static t_token	*new_token(t_token_type type, char *value, int quote_type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = value;
	tok->quote_type = quote_type;
	tok->next = NULL;
	return (tok);
}

static void	token_add_back(t_token **head, t_token *tok)
{
	t_token	*curr;

	if (!*head)
	{
		*head = tok;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = tok;
}

// Reads a quoted segment and appends chars into buf at position *j
// Returns the updated quote_type (1 = single, 2 = double)
static int	read_quoted(const char *input, int *i, char *buf, int *j)
{
	char	quote;
	int		qtype;

	quote = input[(*i)++];
	qtype = (quote == '\'') ? 1 : 2;
	while (input[*i] && input[*i] != quote)
		buf[(*j)++] = input[(*i)++];
	if (input[*i] == quote)
		(*i)++;
	return (qtype);
}

// Reads one full word token (handles adjacent quotes + unquoted chars)
static char	*read_word(const char *input, int *i, int *quote_type)
{
	char	buf[4096];
	int		j;
	int		qt;

	j = 0;
	*quote_type = 0;
	while (input[*i] && !is_space(input[*i])
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			qt = read_quoted(input, i, buf, &j);
			if (*quote_type == 0)
				*quote_type = qt;
		}
		else
			buf[j++] = input[(*i)++];
	}
	buf[j] = '\0';
	return (ft_strdup(buf));
}

// Handles operator tokens: | < > >> <<
static t_token	*read_operator(const char *input, int *i)
{
	t_token_type	type;
	char			*val;

	if (input[*i] == '|')
	{
		type = TOKEN_PIPE;
		val = ft_strdup("|");
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		type = TOKEN_REDIR_HEREDOC;
		val = ft_strdup("<<");
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		type = TOKEN_REDIR_APPEND;
		val = ft_strdup(">>");
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		type = TOKEN_REDIR_IN;
		val = ft_strdup("<");
		(*i)++;
	}
	else
	{
		type = TOKEN_REDIR_OUT;
		val = ft_strdup(">");
		(*i)++;
	}
	return (new_token(type, val, 0));
}

// Main lexer: turns a raw input line into a t_token linked list
t_token	*lexer(const char *input)
{
	t_token	*head;
	t_token	*tok;
	char	*val;
	int		quote_type;
	int		i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			tok = read_operator(input, &i);
		else
		{
			val = read_word(input, &i, &quote_type);
			tok = new_token(TOKEN_WORD, val, quote_type);
		}
		if (!tok)
		{
			free_tokens(head);
			return (NULL);
		}
		token_add_back(&head, tok);
	}
	return (head);
}
