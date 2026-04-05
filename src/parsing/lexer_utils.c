int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*new_token(t_token_type type, char *value, int quote_type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quote_type = quote_type;
	token->next = NULL;
	return (token);
}

void	token_add_back(t_token **head, t_token *new_one)
{
	t_token	*last;

	if (!head || !new_one)
		return ;
	if (!*head)
	{
		*head = new_one;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_one;
}

t_token	*read_operator_token(char *line, int *i)
{
	if (line[*i] == '|')
		return ((*i)++, new_token(TOKEN_PIPE, ft_strdup("|"), NO_QUOTE));
	if (line[*i] == '<' && line[*i + 1] == '<')
		return ((*i) += 2, new_token(TOKEN_REDIR_HEREDOC,
				ft_strdup("<<"), NO_QUOTE));
	if (line[*i] == '>' && line[*i + 1] == '>')
		return ((*i) += 2, new_token(TOKEN_REDIR_APPEND,
				ft_strdup(">>"), NO_QUOTE));
	if (line[*i] == '<')
		return ((*i)++, new_token(TOKEN_REDIR_IN, ft_strdup("<"), NO_QUOTE));
	return ((*i)++, new_token(TOKEN_REDIR_OUT, ft_strdup(">"), NO_QUOTE));
}