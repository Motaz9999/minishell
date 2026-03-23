/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 01:19:47 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 21:06:06 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (g_sigint_received)
		{
			shell->last_exit_status = 130;
			g_sigint_received = 0;
		}
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			g_sigint_received = 0;
			parse_and_execute(line, shell);
		}
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(shell));
	if (!envp)
		shell.env_list = init_env(envp);
	setup_signals_parent();
	shell_loop(&shell);
	return (shell.last_exit_status);
}
