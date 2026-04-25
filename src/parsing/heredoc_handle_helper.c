/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:58:03 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/25 02:59:31 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Close heredoc read ends already created on the current command.
** This prevents inherited descriptors from leaking in later heredoc forks.
*/
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

/*
** Process one heredoc input line.
** Stop on delimiter match, optionally expand variables, then write to pipe.
*/
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

// here u must free all stuff and handle them well
// then do the normal
static void	dose_the_user_send_sig(int fds[], t_heredoc_ctx *ctx, char **line)
{
	if (g_sigint_received == SIG_STATE_INT_HEREDOC)
	{
		if (line && *line)
			free(*line);
		if (fds[0] != -1)
			close(fds[0]);
		if (fds[1] != -1)
			close(fds[1]);
		free_redirects(ctx->redir);
		free_commands(ctx->cmd);
		free_shell(ctx->shell);
		setup_signals_child();
		kill(getpid(), SIGINT);
	}
}

/*
** Child-side heredoc reader loop.
** Cleans inherited fds, reads lines until delimiter/EOF, then exits.
*/
static void	run_heredoc_child(t_heredoc_ctx *ctx, int fds[])
{
	char	*line;

	line = NULL;
	while (TRUE)
	{
		dose_the_user_send_sig(fds, ctx, &line);
		line = readline("> ");
		dose_the_user_send_sig(fds, ctx, &line);
		if (!line)
		{
			warn_heredoc_eof(ctx->key);
			break ;
		}
		if (!handle_heredoc_line(ctx, fds[1], line))
			break ;
	}
	if (fds[1] != -1)
		close(fds[1]);
	free_redirects(ctx->redir);
	free_commands(ctx->cmd);
	free_shell(ctx->shell);
	exit(0);
}

/*
** Fork a dedicated child that collects heredoc input into the pipe.
** Parent gets child pid for wait/status handling.
*/
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
	{
		close_all_heredoc_fds_except(ctx->shell->commands, NULL);
		close_cmd_heredoc_fds(ctx->cmd);
		close(fds[0]);
		fds[0] = -1;
		setup_signals_heredoc();
		run_heredoc_child(ctx, fds);
	}
	return (pid);
}
