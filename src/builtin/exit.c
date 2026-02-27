/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 06:14:18 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 07:26:29 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_shell(t_ext *ext, t_shell *shell)
{
    int	exit_code;

    (void)shell;
    if (ext->cmd->args[1] == NULL)
        exit_code = 0;
    else if (is_numeric(ext->cmd->args[1]))
        exit_code = ft_atoi(ext->cmd->args[1]) % 256;
    else
    {
        ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
        exit_code = 255;
    }
    ft_putstr_fd("exit\n", 1);
    exit(exit_code);
}
