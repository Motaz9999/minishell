/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 10:24:18 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 01:46:12 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to add to the list (last node)
void	redirect_add_back(t_redirect **head, t_redirect *new_one)
{
	t_redirect	*last;

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

// here i must handle this redire
// may this redir have a linked list so must check in new_redire
// it also handle the heredoc in here and in execute
//
static t_redirect	*create_redirection(t_token *tok, t_redir_type redir_type)
{
	t_redirect	*redir;
	char		*key;

	if (redir_type != REDIR_HEREDOC)
		return (new_redirect(redir_type, tok->next->value));
	key = normalize_heredoc_key(tok->next->value);
	if (!key)
		return (NULL);
	redir = new_redirect(redir_type, key);
	free(key);
	return (redir);
}

static int	handle_heredoc_redirection(t_command *cmd, t_redirect *redir,
		t_token *tok, t_shell *shell)
{
	if (redir->type != REDIR_HEREDOC)
		return (0);
	if (fill_heredoc(cmd, redir, shell, tok->next->quote_type))
		return (0);
	free_redirects(redir);
	return (1);
}

int	parse_redirection(t_command *cmd, t_token **tok, t_shell *shell)
{
	t_redirect		*redir;
	t_redir_type	redir_type;

	redir_type = get_redir_type((*tok)->type);
	if (check_redir(*tok))
	{
		shell->last_exit_status = 2;
		return (1);
	}
	redir = create_redirection(*tok, redir_type);
	if (!redir)
		return (1);
	if (handle_heredoc_redirection(cmd, redir, *tok, shell))
		return (1);
	redirect_add_back(&cmd->redirects, redir);
	*tok = (*tok)->next;
	return (0);
}
