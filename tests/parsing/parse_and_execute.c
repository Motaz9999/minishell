/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 21:10:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/24 21:33:12 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	fill_heredoc(t_redirect *redir, t_shell *shell)
{
	int		fds[2];
	char	*line;

	if (!redir)
		return (FALSE);
	if (pipe(fds) == -1)
		return (error_syscall("pipe", 1), FALSE);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(fds[1], line, ft_strlen(line));
		write(fds[1], "\n", 1);
		free(line);
	}
	close(fds[1]);
	redir->heredoc_fd = fds[0];
	(void)shell;
	return (TRUE);
}

static int	prepare_heredocs(t_command *commands, t_shell *shell)
{
	t_command	*cmd;
	t_redirect	*redir;

	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC && !fill_heredoc(redir, shell))
				return (FALSE);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (TRUE);
}

static void	expand_all_commands(t_shell *shell)
{
	t_command	*cmd;

	cmd = shell->commands;
	while (cmd)
	{
		expand_args_from_cmd(shell, cmd);
		cmd = cmd->next;
	}
}

// Main parse and execute function
void	parse_and_execute(char *line, t_shell *shell)
{
	if (!line || !shell)
		return ;
	// Lexer: Convert input line into tokens
	shell->tokens = lexer(line);
	if (!shell->tokens || count_tokens(shell->tokens) == 0)
	{
		if (shell->tokens)
			free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
	// Parser: Convert tokens into commands
	shell->commands = parser(shell->tokens, shell);
	if (!shell->commands)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
	expand_all_commands(shell);
	if (!prepare_heredocs(shell->commands, shell))
	{
		free_tokens(shell->tokens);
		free_commands(shell->commands);
		shell->tokens = NULL;
		shell->commands = NULL;
		return ;
	}
	// Execute the commands
	execute(shell);
	// Free tokens and commands for the next iteration
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
	shell->tokens = NULL;
	shell->commands = NULL;
}
