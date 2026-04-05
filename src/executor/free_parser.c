void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
}

void	free_redirects(t_redirect *redirects)
{
	t_redirect	*next;

	while (redirects)
	{
		next = redirects->next;
		free(redirects->file);
		free(redirects);
		redirects = next;
	}
}

static void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_commands(t_command *commands)
{
	t_command	*next;

	while (commands)
	{
		next = commands->next;
		free_args(commands->args);
		free(commands->quote_types);
		free_redirects(commands->redirects);
		free(commands);
		commands = next;
	}
}