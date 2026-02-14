/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:12 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/15 00:53:30 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int close_pipes(int pipes[] , t_shell *shell)
{
	if (pipes[0] != -1)
		close(pipes[0]);
	if (pipes[1] != -1)
		close(pipes[1]);
	pipes[0] = -1;
	pipes[1] = -1;// so i wont close them again
	shell->last_exit_status = 1;
	return (FALSE);
}
int close_pipes_prev(int pipes[] , int *prev , t_shell *shell)
{
	close_pipes(pipes , shell);
	if (*prev != -1)
		close(*prev);
	*prev = -1;
	shell->last_exit_status = 1;
	return FALSE;
}

// here we put the pipes then over ride it
//return 1 if it success and 0 if it fail (the dub) and it also closes the pipes and anything else
//here it just dup problem handle it inside the caller fun 
int	handle_pipes(int prev_fd_in, int pipes[], int remaining_cmd , t_shell *shell)
{
	if (remaining_cmd > 1) // have more than 1 cmd (middle) or start//but how to know if it the first(easy by prev_fd_in)
	{
		if (prev_fd_in == -1)//first cmd
		{
			if (dup2(pipes[1] , 1) == -1)
					return close_pipes(pipes);
		}
		else// not the first cmd soo there is previous pipe i should take the input from it
		{//but them in if statement
			if (dup2(prev_fd_in,0) == -1 || dup2(pipes[1] , 1) == -1)//now i am taking from the prev pipe and close it in parent bc it live in it
					return close_pipes_prev(pipes , &prev_fd_in);
		}
	}
	else if (remaining_cmd == 1) // the  last cmd  but what if 
	{//how to check if 1 cmd or last cmd //use the same way 
		if (prev_fd_in == -1)//first cmd  but in the same time last (just one cmd)
			return TRUE;//there is nothing to do 
		else // last cmd (dont have new pipe from parent) but have input comes from other cmds
			if (dup2(prev_fd_in , 0) == -1)
				return close_pipes_prev(pipes , &prev_fd_in);
	}
	close_pipes_prev(pipes , &prev_fd_in , shell);
	shell->last_exit_status = 0;//it ok
	return TRUE;
}
