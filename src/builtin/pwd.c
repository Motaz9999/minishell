/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:33:35 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/23 19:44:45 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// print the current dir
int	pwd(void)
{
	char *pwd;

	pwd = getcwd(NULL, 0); // here is the path
	if (pwd == NULL)
	{
		shell->last_exit_status = error_syscall("getcwd", 1);
		return (shell->last_exit_status);
	}
	ft_printf("%s\n", pwd);
	free(pwd);
	return (0);
}