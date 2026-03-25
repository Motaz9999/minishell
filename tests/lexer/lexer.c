/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 21:10:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/24 21:33:12 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Create a new token
static t_token	*create_token(t_token_type type, char *value, int quote_type)
{
	t_token	*token;

	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quote_type = quote_type;
	token->next = NULL;
	return (token);
}

// Add token to the end of the linked list
static void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

// Check if character is a space or tab
static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

// Check if character is a special operator
static int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

// Extract a word token (handles quotes)
static char	*extract_word(const char *input, int *pos, int *quote_type)
{
	char	*word;
	int		len;
	int		i;
	int		j;
	int		has_single;
	int		has_double;
	char	quote_char;

	len = 0;
	has_single = FALSE;
	has_double = FALSE;
	*quote_type = NO_QUOTE;
	while (input[*pos + len] && !is_space(input[*pos + len])
		&& !is_special(input[*pos + len]))
	{
		if (input[*pos + len] == '\'' || input[*pos + len] == '"')
		{
			quote_char = input[*pos + len];
			if (quote_char == '\'')
				has_single = TRUE;
			else
				has_double = TRUE;
			len++;
			while (input[*pos + len] && input[*pos + len] != quote_char)
				len++;
			if (input[*pos + len] == quote_char)
				len++;
		}
		else
			len++;
	}
	if (has_single && !has_double)
		*quote_type = SINGLE_QUOTE;
	else if (has_double && !has_single)
		*quote_type = DOUBLE_QUOTE;
	else
		*quote_type = NO_QUOTE;
	word = (char *)ft_calloc(len + 1, sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (input[*pos + i] == '\'' || input[*pos + i] == '"')
		{
			quote_char = input[*pos + i++];
			while (i < len && input[*pos + i] && input[*pos + i] != quote_char)
				word[j++] = input[*pos + i++];
			if (i < len && input[*pos + i] == quote_char)
				i++;
		}
		else
			word[j++] = input[*pos + i++];
	}
	word[j] = '\0';
	*pos += len;
	return (word);
}

// Main lexer function
t_token	*lexer(const char *line)
{
	t_token	*tokens;
	int		pos;
	int		quote_type;
	char	*word;

	if (!line)
		return (NULL);
	tokens = NULL;
	pos = 0;
	while (line[pos])
	{
		// Skip whitespace
		while (line[pos] && is_space(line[pos]))
			pos++;
		if (!line[pos])
			break ;
		// Handle pipe operator
		if (line[pos] == '|' && (pos == 0 || line[pos - 1] != '|'))
		{
			add_token(&tokens, create_token(TOKEN_PIPE, ft_strdup("|"), 0));
			pos++;
		}
		// Handle output redirection (>> or >)
		else if (line[pos] == '>')
		{
			if (line[pos + 1] == '>')
			{
				add_token(&tokens,
						create_token(TOKEN_REDIR_APPEND, ft_strdup(">>"), 0));
				pos += 2;
			}
			else
			{
				add_token(&tokens,
						create_token(TOKEN_REDIR_OUT, ft_strdup(">"), 0));
				pos++;
			}
		}
		// Handle input redirection or heredoc
		else if (line[pos] == '<')
		{
			if (line[pos + 1] == '<')
			{
				add_token(&tokens,
						create_token(TOKEN_REDIR_HEREDOC, ft_strdup("<<"), 0));
				pos += 2;
			}
			else
			{
				add_token(&tokens,
						create_token(TOKEN_REDIR_IN, ft_strdup("<"), 0));
				pos++;
			}
		}
		// Handle semicolon (command separator)
		else if (line[pos] == ';')
		{
			pos++;
		}
		// Extract word tokens
		else
		{
			word = extract_word(line, &pos, &quote_type);
			if (word)
				add_token(&tokens, create_token(TOKEN_WORD, word, quote_type));
		}
	}
	// Add EOF token
	add_token(&tokens, create_token(TOKEN_EOF, NULL, 0));
	return (tokens);
}
