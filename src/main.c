/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 01:19:47 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/24 20:34:42 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_welcome_logo(void)
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return ;
	ft_printf("\033[2J\033[H\n\n");
	ft_printf("%s%s", BOLD, MAGENTA);
	ft_printf("%s", LINE1);
	ft_printf("%s", LINE2);
	ft_printf("%s", LINE3);
	ft_printf("%s", LINE4);
	ft_printf("%s", LINE5);
	ft_printf("%s%s", LINE6, RESET);
	ft_printf("%sMade by moodeh & aamr @ 42 Amman%s\n", YELLOW, RESET);
	ft_printf("%s%s==============================================%s\n", BOLD,
		MAGENTA, RESET);
	ft_printf("%s%s%s - interactive minishell%s\n", BOLD, RED, SHELL_NAME,
		RESET);
	ft_printf("%sBuiltins: cd echo pwd export unset env exit%s\n", MAGENTA,
		RESET);
	ft_printf("%sSignals : Ctrl-C clears line | Ctrl-D exits%s\n", MAGENTA,
		RESET);
	ft_printf("%sTip     : use $? to view last exit status%s\n", MAGENTA,
		RESET);
	ft_printf("%s%s==============================================%s\n\n", BOLD,
		MAGENTA, RESET);
}

static void	shell_loop(t_shell *shell)
{
	char	*line;

	setup_signals_parent();
	while (1)
	{
		g_sigint_received = SIG_STATE_PROMPT;
		line = readline(PROMPT_FULL);
		if (g_sigint_received == SIG_STATE_INT_PROMPT
			|| g_sigint_received == SIG_STATE_INT_OUTSIDE)
		{
			shell->last_exit_status = 130;
			g_sigint_received = SIG_STATE_NONE;
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

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	(void)argc;
	ft_memset(&shell, 0, sizeof(shell));
	print_welcome_logo();
	if (envp)
		shell.env_list = init_env(envp);
	shell_loop(&shell);
	free_shell(&shell);
	rl_clear_history();
	return (shell.last_exit_status);
}
