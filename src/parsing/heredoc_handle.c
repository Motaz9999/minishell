/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 01:06:31 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/13 02:21:03 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	fill_heredoc_helper(t_shell *shell, char *key, int fds[],
		int quote_type)
{
	char	*line;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		shell->last_exit_status = error_syscall("fork", 1);
		return (-1);
	}
	if (pid == 0) // child process
	{
		close(fds[0]);
		setup_signals_heredoc(); // igNOR all even but not ctrl +c
		while (1)
		{
			line = readline("> ");
			if (!line)
				break ;
			if (ft_strcmp(line, "") == 0)
			{
				write(fds[1], line, ft_strlen(line));
				write(fds[1], "\n", 1);
				free(line);
				continue ;
			}
			if (ft_strcmp(line, key) == 0) // stops here
			{
				free(line);
				break ;
			}
			if (quote_type == 0)
				line = expand_cmd(line, shell);
			if (!line)
				continue ;
			write(fds[1], line, ft_strlen(line));
			write(fds[1], "\n", 1);
			free(line);
		}
		close(fds[1]);
		free_shell(shell);
		exit(0);
		// all good  else the sig well be out with number must be handle to make sens
	}
	return (pid);
}

// update later to let take expand and signals
// ridir file is the name of KEY
// if quote == 1 or 2 dont expand
// else expand
int	fill_heredoc(t_redirect *redir, t_shell *shell, int quote_type)
{
	int		fds[2];
	pid_t	pid;
	int		status;

	pid = -1;
	if (!redir)
		return (FALSE);
	if (pipe(fds) == -1)
	{
		shell->last_exit_status = error_syscall("pipe", 1);
		return (FALSE);
	}
	pid = fill_heredoc_helper(shell, redir->file, fds, quote_type);
	close(fds[1]);
	if (pid == -1)
	{
		close(fds[0]);
		return (FALSE);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		shell->last_exit_status = error_syscall("waitpid", 1);
		close(fds[0]);
		return (FALSE);
	}
	if (WIFEXITED(status)) // dose it exit normally
		shell->last_exit_status = WEXITSTATUS(status);
	// what is the code that exit normally
	else if (WIFSIGNALED(status)) // dose it exit bc of sig
	{
		status = WTERMSIG(status);
		shell->last_exit_status = status + 128; // so it make sens
		close(fds[0]);
		return (FALSE);
	}
	redir->heredoc_fd = fds[0]; // save the output pipe
	return (TRUE);
}
