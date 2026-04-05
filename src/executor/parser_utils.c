t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->quote_types = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	command_add_back(t_command **head, t_command *new_one)
{
	t_command	*last;

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

int	count_args_in_segment(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && tok->type != TOKEN_EOF && tok->type != TOKEN_PIPE)
	{
		if (tok->type == TOKEN_WORD)
			count++;
		else if (is_redir_token(tok->type) && tok->next)
			tok = tok->next;
		tok = tok->next;
	}
	return (count);
}