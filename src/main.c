/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 01:19:47 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 11:30:21 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(shell));
	shell.env_list = init_env(envp);
	setup_signals_parent();
	while (1)
	{
		line = readline("minishell$ ");
		if (g_sigint_received)
		{
			shell.last_exit_status = 130;
			g_sigint_received = 0;
		}
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			g_sigint_received = 0;
			parse_and_execute(line, &shell);
		}
		free(line);
	}
	return (shell.last_exit_status);
}
