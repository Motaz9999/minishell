/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 10:24:18 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/11 03:17:05 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

//update later to let take expand and signals
//ridir file is the name of KEY
int	fill_heredoc(t_redirect *redir, t_shell *shell)
{
	int		fds[2];
	char	*line;

	if (!redir)
		return (FALSE);
	if (pipe(fds) == -1)
	{
		shell->last_exit_status = error_syscall("pipe", 1);
		return (FALSE);
	}
	setup_heredoc();
	while (1)
	{
		line = expand_cmd(readline("heredoc>> "), shell);
		if (!line)
			break ;
		if (ft_strcmp(line, redir->file) == 0)//stops here
		{
			free(line);
			break ;
		}
		write(fds[1], line, ft_strlen(line));
		write(fds[1], "\n", 1);
		free(line);
	}
	setup_signals_parent();
	close(fds[1]);
	redir->heredoc_fd = fds[0];
	return (TRUE);
}

//here i must handle this redire
//may this redir have a linked list so must check in new_redire
int	parse_redirection(t_command *cmd, t_token **tok, t_shell *shell)
{
	t_redirect	*redir;
	t_redir_type	redir_type;

	redir = NULL;
	redir_type = get_redir_type((*tok)->type);
	if (check_redir(*tok))
	{
		shell->last_exit_status = 2;
		return (1);
	}
	redir = new_redirect(redir_type, (*tok)->next->value);
	if (!redir)
		return (1);
	if (redir_type == REDIR_HEREDOC && !fill_heredoc(redir, shell))
	{
		free_redirects(redir);
		return (1);
	}
	redirect_add_back(&cmd->redirects, redir);
	*tok = (*tok)->next;
	return (0);
}
