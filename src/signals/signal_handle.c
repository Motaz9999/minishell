/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:05 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/18 21:53:02 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Single definition of the global signal flag */
volatile sig_atomic_t	g_sigint_received = 0;
static void	handle_sigint(int sig)
{
	(void)sig;
	g_sigint_received = 1;         // we cant do anything  with it now
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay(); //
}

// here we setup the signals for parent
// what dose this mean ? it mean we change what the signals can do
// like ctrl + c it mean here just preint \n and discard any input (the read)
// and like ctrl+\ it dosent do anything  soo we but it SIG_IGN
// here the global var g_in_cmd = 0 (interactive mode)
void	setup_signals_parent(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask); // bc i want to see if it sent any signals
	sa.sa_flags = SA_RESTART; // dont fails read and write
	sigaction(SIGINT, &sa, NULL);
	// now to works as signal fun but is more advanced
	// dont forgot to skip SIGQUIT ctrl+ (\)
	sa.sa_handler = SIG_IGN; // ignore it
	sigaction(SIGQUIT, &sa, NULL);
}

// ok now before we send the cmd to execute part we change g_in_cmd = 1
// here child have same values as parent and we must change this
// bc if the kernal send signal its send to the partent and child
// FOR NOW
// g_in_cmd =1    sigint -> handle_sigint(the one print new line)  and sigquit
//	-> sigIGN
// so now i want to change how they work
// just return them into default mode inside child
//│ SIGQUIT → SIG_DFL (default: die)      │
//│ SIGINT  → SIG_DFL (default: die)      │
void	setup_signals_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

// now the parent is waiting
// dont forgot the parent g_in_cmd = 1 and signals still custom
// after we waiting the child process to be terminate
// we return into interactive mode g_in_cmd = 0
// and now we print new prompt