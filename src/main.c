/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 01:19:47 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/19 21:35:43 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*line;
    t_shell shell;

    shell.last_exit_code = 0;// maybe we can here print something cool before anything
	setup_signals_parent();// dont forgot to handle inside heredoc (it is a different case)
	while (1)
	{
		line = readline("minishell$ ");
        if (g_sigint_received)
        {
            shell.last_exit_status = 130;
            g_sigint_received = 0;//reset
        }
		if (!line)//ctrl+D
			break ;
		if (*line)
		{
			add_history(line);
            g_sigint_received = 0 ; //reset before give it to child
            parse_and_execute(line , &shell);//it have cleanup after execute
		}
		free(line);
	}
	return (shell.last_exit_status);//end of the shell
}
