/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 06:14:18 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 07:53:32 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_str_isdigit(const char *str)
{
	if (!str || *str == '\0')
		return (FALSE);
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (FALSE);
		str++;
	}
	return (TRUE);
}

int	exit_shell(t_ext *ext)
{
	int	exit_code;

	if (ext->cmd->args[1] && ft_str_isdigit(ext->cmd->args[1]) == FALSE)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(ext->cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	else if (find_args_count(ext->cmd->args) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (FALSE);
	}
	exit_code = ext->cmd->args[1] ? ft_atoi(ext->cmd->args[1]) : 0;
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	else if (exit_code > 255)
		exit_code = exit_code % 256;
	ft_putstr_fd("exit\n", 1);
	exit(exit_code);
}
