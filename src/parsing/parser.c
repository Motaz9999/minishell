#include "minishell.h"

static int	alloc_arrays(t_command *cmd, int count)
{
	cmd->args = ft_calloc((count + 1), sizeof(char *));
	if (!cmd->args)
		return (1);
	cmd->quote_types = ft_calloc((count + 1), sizeof(t_quote_type));
	if (!cmd->quote_types)
		return (free(cmd->args), 1);
	return (0);
}

static int	add_argument(t_command *cmd, t_token *tok, int *i)
{
	cmd->args[*i] = ft_strdup(tok->value);
	if (!cmd->args[*i])
		return (1);
	cmd->quote_types[*i] = tok->quote_type;
	(*i)++;
	return (0);
}

t_command	*parse_one_command(t_token **tok, t_shell *shell)
{
	t_command	*cmd;
	int			count;
	int			i;

	cmd = new_command();
	if (!cmd)
		return (NULL);
	count = count_args_in_segment(*tok);
	if (alloc_arrays(cmd, count))
		return (free(cmd), NULL);
	i = 0;
	while (*tok && (*tok)->type != TOKEN_EOF && (*tok)->type != TOKEN_PIPE)
	{
		if ((*tok)->type == TOKEN_WORD && add_argument(cmd, *tok, &i))
			return (free_commands(cmd), NULL);
		if (is_redir_token((*tok)->type) && parse_redirection(cmd, tok, shell))
			return (free_commands(cmd), NULL);
		*tok = (*tok)->next;
	}
	cmd->args[i] = NULL;
	return (cmd);
}

t_command	*parser(t_token *tokens, t_shell *shell)
{
	t_command *head;
	t_command *cmd;

	head = NULL;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		cmd = parse_one_command(&tokens, shell);
		if (!cmd)
			return (free_commands(head), NULL);
		command_add_back(&head, cmd);
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (head);
}
