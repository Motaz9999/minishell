/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:34:11 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/23 14:14:53 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// so i need first to check_on pipes (the in pipe)

// first i want to build the cmd that is execute in parent process
// return true if i change the loc and false if not with error massage
// must be single cmd WITH NO pipe BUT if there are a pipe i should execute in child process
// so just make sure to fork it there are a pipe
int	cd(t_ext *ext, t_shell *shell , int casee)
{
	if(chidr(ext->cmd->args[1]) == -1)//there are error in the path so i cant change it
	{
		shell->last_exit_status = 1;
		error_syscall("cd" , 1);
		if (casee == 1)
			exit(shell->last_exit_status);
		else
			return (shell->last_exit_status);
	}
	
}
int	export(t_shell *shell)
{
}