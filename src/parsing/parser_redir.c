/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 10:24:18 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/15 21:27:25 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//this fun is for finding out if the key have to expand anything
//so the key must be normalize first
static char	*normalize_heredoc_key(char *raw)
{
	char	*key;
	int		i;

	key = ft_strdup(raw);
	if (!key)
		return (NULL);
	i = 0;
	while (key[i])
	{
		if (key[i] == '\1')
			key[i] = '$';
		i++;
	}
	return (key);
}

//know the redir type 
t_redir_type	get_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

//add new node but must have init values
t_redirect	*new_redirect(t_redir_type type, char *file)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->fd = -1;
	redir->heredoc_fd = -1;
	redir->next = NULL;
	if (!redir->file)
		return (free(redir), NULL);
	return (redir);
}

//to add to the list (last node)
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


//here i must handle this redire
//may this redir have a linked list so must check in new_redire
//it also handle the heredoc in here and in execute
int	parse_redirection(t_command *cmd, t_token **tok, t_shell *shell)
{
	t_redirect	*redir;
	t_redir_type	redir_type;
	char		*key;

	redir = NULL;
	redir_type = get_redir_type((*tok)->type);
	key = NULL;
	if (check_redir(*tok))
	{
		shell->last_exit_status = 2;
		return (1);
	}
	if (redir_type == REDIR_HEREDOC)
	{
		key = normalize_heredoc_key((*tok)->next->value);
		if (!key)
			return (1);
		redir = new_redirect(redir_type, key);//added it to the list 
		free(key);
	}
	else
		redir = new_redirect(redir_type, (*tok)->next->value);
	if (!redir)
		return (1);
	if (redir_type == REDIR_HEREDOC
		&& !fill_heredoc(cmd, redir, shell, (*tok)->next->quote_type))//the only redir handled in parsing part AND in executinos
	{
		free_redirects(redir);
		return (1);
	}
	redirect_add_back(&cmd->redirects, redir);
	*tok = (*tok)->next;
	return (0);
}
