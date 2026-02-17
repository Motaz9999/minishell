/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 01:19:47 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/17 02:14:31 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void execute_cmd(char **args)
{
    pid_t pid;
    int status;
    
    g_in_cmd = 1;  // We're running a command
    
    pid = fork();
    
    if (pid == 0)  // Child
    {
        setup_signals_child();  // Restore defaults
        execvp(args[0], args);
        perror(args[0]);
        exit(127);
    }
    else if (pid > 0)  // Parent
    {
        waitpid(pid, &status, 0);
        
        if (WIFSIGNALED(status))
        {
            int sig = WTERMSIG(status);
            if (sig == SIGQUIT)
                write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        }
        
        g_in_cmd = 0;  // Back to interactive
    }
}


int	main(void)
{
	char	*line;
    t_shell shell;
	// maybe we can here print something cool before anything
	setup_signals_parent();
	// dont forgot to handle inside heredoc (it is a different case)
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (line && *line)
		{
			add_history(line);
		}
        //t_shell = pareser();
        //execute_cmd(&shell);
		// parser take over here and send to execute
		// execute_cmd(shell);
		free(line);
	}
	return (0)
}
