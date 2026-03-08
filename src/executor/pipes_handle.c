/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:12 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/08 15:37:54 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	close_pipes(int pipes[], t_shell *shell)
{
	if (pipes[0] != -1)
		close(pipes[0]);
	if (pipes[1] != -1)
		close(pipes[1]);
	pipes[0] = -1;
	pipes[1] = -1;
	shell->last_exit_status = 1;
	return (FALSE);
}

int	close_pipes_prev(int pipes[], int *prev, t_shell *shell)
{
	close_pipes(pipes, shell);
	if (*prev != -1)
		close(*prev);
	*prev = -1;
	shell->last_exit_status = 1;
	return (FALSE);
}

// here we put the pipes then over ride it
// return 1 if it success and 0 if it fail (the dub) and it also
// closes the pipes and anything else
// here it just dup problem handle it inside the caller fun
// have more than 1 cmd (middle) or start//but how to know
// if it the first(easy by prev_fd_in)
int	handle_pipes(int prev_fd_in, int pipes[], int remaining_cmd, t_shell *shell)
{
	if (remaining_cmd > 1)
	{
		if (prev_fd_in == -1)
		{
			if (dup2(pipes[1], 1) == -1)
				return (close_pipes(pipes, shell));
		}
		else
		{
			if (dup2(prev_fd_in, 0) == -1 || dup2(pipes[1], 1) == -1)
				return (close_pipes_prev(pipes, &prev_fd_in, shell));
		}
	}
	else if (remaining_cmd == 1)
	{
		if (prev_fd_in == -1)
			return (TRUE);
		else if (dup2(prev_fd_in, 0) == -1)
			return (close_pipes_prev(pipes, &prev_fd_in, shell));
	}
	close_pipes_prev(pipes, &prev_fd_in, shell);
	shell->last_exit_status = 0;
	return (TRUE);
}
