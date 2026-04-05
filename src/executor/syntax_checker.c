int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC);
}

static int	syntax_error(char *value)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(value, 2);
	ft_putendl_fd("'", 2);
	return (2);
}

static int	check_pipe(t_token *tok)
{
	if (!tok->next || tok->next->type == TOKEN_PIPE
		|| tok->next->type == TOKEN_EOF)
		return (syntax_error("|"));
	return (0);
}

static int	check_redir(t_token *tok)
{
	if (!tok->next || tok->next->type == TOKEN_EOF)
		return (syntax_error("newline"));
	if (tok->next->type != TOKEN_WORD)
		return (syntax_error(tok->next->value));
	return (0);
}

int	validate_syntax(t_token *tokens)
{
	if (tokens && tokens->type == TOKEN_PIPE)
		return (syntax_error("|"));
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_PIPE && check_pipe(tokens))
			return (2);
		if (is_redir_token(tokens->type) && check_redir(tokens))
			return (2);
		tokens = tokens->next;
	}
	return (0);
}