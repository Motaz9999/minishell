/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:58:03 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/19 03:50:12 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//close already-created heredoc FDs only inside current command
// during parse-time heredoc child. 
static void	close_cmd_heredoc_fds(t_command *cmd)
{
	t_redirect	*redir;

	if (!cmd)
		return ;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC && redir->heredoc_fd != -1)
		{
			close(redir->heredoc_fd);
			redir->heredoc_fd = -1;
		}
		redir = redir->next;
	}
}

static int	handle_heredoc_line(t_heredoc_ctx *ctx, int write_fd, char *line)
{
	if (ft_strcmp(line, ctx->key) == 0)
	{
		free(line);
		return (FALSE);
	}
	if (ctx->quote_type == NO_QUOTE)
		line = expand_cmd(line, ctx->shell);
	if (!line)
		return (TRUE);
	write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
	free(line);
	return (TRUE);
}

static void	run_heredoc_child(t_heredoc_ctx *ctx, int fds[])
{
	char	*line;

	close_all_heredoc_fds_except(ctx->shell->commands, NULL);
	close_cmd_heredoc_fds(ctx->cmd);
	close(fds[0]);
	setup_signals_heredoc();
	while (TRUE)
	{
		line = readline("> ");
		if (!line)
		{
			warn_heredoc_eof(ctx->key);
			break ;
		}
		if (!handle_heredoc_line(ctx, fds[1], line))
			break ;
	}
	close(fds[1]);
	free_redirects(ctx->redir);
	free_commands(ctx->cmd);
	free_shell(ctx->shell);
	exit(0);
}

pid_t	fill_heredoc_helper(t_heredoc_ctx *ctx, int fds[])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ctx->shell->last_exit_status = error_syscall("fork", 1);
		return (-1);
	}
	if (pid == 0)
		run_heredoc_child(ctx, fds);
	return (pid);
}
