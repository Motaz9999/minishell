/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:58:03 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 02:02:57 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
