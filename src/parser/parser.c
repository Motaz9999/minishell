/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 09:32:15 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

// Appends one argument to cmd->args (grows the array by one each call)
static int	cmd_add_arg(t_command *cmd, char *word)
{
	int		count;
	char	**new_args;
	int		i;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (FALSE);
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_strdup(word);
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (TRUE);
}

static t_redirect	*new_redir(t_redir_type type, char *file)
{
	t_redirect	*r;

	r = malloc(sizeof(t_redirect));
	if (!r)
		return (NULL);
	r->type = type;
	r->file = file;
	r->fd = -1;
	r->heredoc_fd = -1;
	r->next = NULL;
	return (r);
}

static void	redir_add_back(t_redirect **head, t_redirect *r)
{
	t_redirect	*curr;

	if (!*head)
	{
		*head = r;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = r;
}

// Maps a TOKEN_REDIR_* type to its t_redir_type counterpart
static t_redir_type	token_to_redir(t_token_type ttype)
{
	if (ttype == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (ttype == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (ttype == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

// Handles a redirection token: reads the next token as filename/delimiter
// and fills the heredoc pipe if needed. Returns FALSE on syntax error.
static int	handle_redir_token(t_token **tok, t_command *cmd, t_shell *shell)
{
	t_redir_type	rtype;
	t_redirect		*redir;

	rtype = token_to_redir((*tok)->type);
	*tok = (*tok)->next;
	if (!*tok || (*tok)->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (FALSE);
	}
	redir = new_redir(rtype, ft_strdup((*tok)->value));
	if (!redir)
		return (FALSE);
	if (rtype == REDIR_HEREDOC)
	{
		if (!fill_heredoc(redir, shell))
		{
			free(redir->file);
			free(redir);
			return (FALSE);
		}
	}
	redir_add_back(&cmd->redirects, redir);
	return (TRUE);
}

// Converts a token list into a linked list of t_command structs
t_command	*parser(t_token *tokens, t_shell *shell)
{
	t_command	*head;
	t_command	*curr;
	t_token		*tok;

	curr = new_command();
	if (!curr)
		return (NULL);
	head = curr;
	tok = tokens;
	while (tok && tok->type != TOKEN_EOF)
	{
		if (tok->type == TOKEN_PIPE)
		{
			curr->next = new_command();
			curr = curr->next;
			if (!curr)
			{
				free_commands(head);
				return (NULL);
			}
		}
		else if (tok->type == TOKEN_WORD)
			cmd_add_arg(curr, tok->value);
		else
		{
			if (!handle_redir_token(&tok, curr, shell))
			{
				free_commands(head);
				return (NULL);
			}
		}
		tok = tok->next;
	}
	return (head);
}
