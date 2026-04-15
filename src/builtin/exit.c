/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 06:14:18 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/15 22:38:26 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// close non closed pipe and fds and free pids mallocs
// also clear history of readline fun
// ext->prev_fd_in > STDERR_FILENO this mean
// that must be bigger than stderr 2 > fd
static void	cleanup_exit_resources(t_ext *ext, t_shell *shell)
{
	if (ext)
	{
		if (ext->pids)
			free(ext->pids);
		if (ext->prev_fd_in > STDERR_FILENO)
			close(ext->prev_fd_in);
		if (ext->pipe_fds[0] > STDERR_FILENO
			&& ext->pipe_fds[0] != ext->prev_fd_in)
			close(ext->pipe_fds[0]);
		if (ext->pipe_fds[1] > STDERR_FILENO
			&& ext->pipe_fds[1] != ext->prev_fd_in
			&& ext->pipe_fds[1] != ext->pipe_fds[0])
			close(ext->pipe_fds[1]);
	}
	free_shell(shell);
	rl_clear_history();
}

// check on the str if it digit
static int	ft_str_isdigit(const char *str)
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

// this handle the numbers if it invalid ()
// and that how the bash handle it
static int	handle_invalid_numeric_arg(t_ext *ext, t_shell *shell)
{
	if (ext->cmd->args[1] && ft_str_isdigit(ext->cmd->args[1]) == FALSE)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(ext->cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_exit_resources(ext, shell);
		exit(2);
	}
	return (FALSE);
}

// the exit code must be computed
// btw it takes neg and pos nums
// and the the range of code is from 0 - 255
// Because process exit status is stored as
// 1 byte in Unix wait status conventions
// exit 256 -> 0
// exit -1 -> 255
// waitpid + WEXITSTATUS, which gives
// that 8-bit value
static int	compute_exit_code(t_ext *ext)
{
	int	exit_code;

	exit_code = 0;
	if (ext->cmd->args[1])
		exit_code = ft_atoi(ext->cmd->args[1]);
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	else if (exit_code > 255)
		exit_code = exit_code % 256;
	return (exit_code);
}

// this is the cmd that i use it to exit shell
int	exit_shell(t_ext *ext, t_shell *shell)
{
	int	exit_code;

	handle_invalid_numeric_arg(ext, shell);
	if (find_args_count(ext->cmd->args) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (FALSE);
	}
	exit_code = compute_exit_code(ext);
	ft_putstr_fd("exit\n", 1);
	cleanup_exit_resources(ext, shell);
	exit(exit_code);
}
