/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamr <aamr <aamr@student.42.fr>>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:51:17 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/17 21:52:37 by aamr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_eof_token(t_token **head)
{
	t_token	*eof;

	eof = new_token(TOKEN_EOF, NULL, NO_QUOTE);
	if (!eof)
		return (1);
	token_add_back(head, eof);
	return (0);
}

static int	add_word_token(t_token **head, char *line, int *i)
{
	t_token	*token;
	char	*word;
	int		quote_type;

	word = read_word(line, i, &quote_type);
	if (!word)
		return (1);
	token = new_token(TOKEN_WORD, word, quote_type);
	if (!token)
	{
		free(word);
		return (1);
	}
	token_add_back(head, token);
	return (0);
}

t_token	*read_operator_token(char *line, int *i)
{
	t_token	*token;

	token = NULL;
	if (line[*i] == '|')
		token = make_operator_token(TOKEN_PIPE, "|", i, 1);
	else if (line[*i] == '<' && line[*i + 1] == '<')
		token = make_operator_token(TOKEN_REDIR_HEREDOC, "<<", i, 2);
	else if (line[*i] == '>' && line[*i + 1] == '>')
		token = make_operator_token(TOKEN_REDIR_APPEND, ">>", i, 2);
	else if (line[*i] == '<')
		token = make_operator_token(TOKEN_REDIR_IN, "<", i, 1);
	else if (line[*i] == '>')
		token = make_operator_token(TOKEN_REDIR_OUT, ">", i, 1);
	return (token);
}

static int	process_token(t_token **head, char *line, int *i)
{
	t_token	*token;

	if (is_operator_char(line[*i]))
	{
		token = read_operator_token(line, i);
		if (!token)
			return (1);
		token_add_back(head, token);
	}
	else if (add_word_token(head, line, i))
		return (1);
	return (0);
}

t_token	*lexer(char *line)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (line && line[i])
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (process_token(&head, line, &i))
		{
			free_tokens(head);
			return (NULL);
		}
	}
	if (add_eof_token(&head))
	{
		free_tokens(head);
		return (NULL);
	}
	return (head);
}
