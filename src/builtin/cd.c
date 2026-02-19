/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:34:11 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/20 01:50:20 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// so i need first to check_on pipes (the in pipe)

// first i want to build the cmd that is execute in parent process
// return true if i change the loc and false if not with error massage
// must be single cmd WITH NO pipe BUT if there are a pipe i should execute in child process
// so just make sure to fork it there are a pipe
int	cd(t_ext *ext, t_shell *shell)
{
	if (shell->commands)
}
int	export(t_shell *shell)
{
}