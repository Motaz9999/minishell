/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 01:19:47 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 00:05:54 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Use one signal state: mark PROMPT before readline
//, then consume INT states after it.
static void	shell_loop(t_shell *shell)
{
	char	*line;

	setup_signals_parent();
	while (1)
	{
		g_sigint_received = SIG_STATE_PROMPT;//we are now inside it mean waiting for line
		line = readline("minishell$ ");
		if (g_sigint_received == SIG_STATE_INT_PROMPT // have sigint but and must change exit code in both states
			|| g_sigint_received == SIG_STATE_INT_OUTSIDE)//but the output depends on SIG_STATE_INT_PROMPT or SIG_STATE_INT_OUTSIDE
		{
			shell->last_exit_status = 130;
			g_sigint_received = SIG_STATE_NONE;//make it non again
		}
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			g_sigint_received = SIG_STATE_NONE;
			parse_and_execute(line, shell);
		}
		free(line);
	}
}

// ft_printf("%s %s", GREEN, BOLD);
// ft_printf("%s", RESET);
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(shell));
	if (envp)
		shell.env_list = init_env(envp);
	shell_loop(&shell);
	free_shell(&shell);
	rl_clear_history();
	return (shell.last_exit_status);
}
