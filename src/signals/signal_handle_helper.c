/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 02:08:04 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 02:10:39 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// a signal handler and normal code
//	write(1, "\n", 1); // we print new line
void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	if (g_sigint_received == SIG_STATE_PROMPT
		|| g_sigint_received == SIG_STATE_INT_PROMPT)
	{
		g_sigint_received = SIG_STATE_INT_PROMPT;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
		g_sigint_received = SIG_STATE_INT_OUTSIDE;
}

// in wait mode after u kill the child process
// print new line
void	handle_sigint_wait(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}
