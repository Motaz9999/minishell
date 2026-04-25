/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 06:14:18 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/25 15:02:18 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	check_ll_bounds(const char *str, int sign)
{
	unsigned long long	num;
	int					i;

	num = 0;
	i = 0;
	while (str[i])
	{
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	if (sign != -1 && num > 9223372036854775807ULL)
		return (FALSE);
	if (sign == -1 && num > 9223372036854775808ULL)
		return (FALSE);
	return (TRUE);
}

static int	ft_str_isdigit(const char *str)
{
	int		i;
	int		s;
	int		st;
	char	r[25];

	i = 0;
	s = 1;
	while (str && (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r')))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			s = -1;
	st = i;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] && str[i] != ' ' && !(str[i] >= '\t' && str[i] <= '\r'))
		return (FALSE);
	while (str[st] == '0' && st < i - 1)
		st++;
	if (i - st > 19)
		return (FALSE);
	ft_strlcpy(r, str + st, i - st + 1);
	return (check_ll_bounds(r, s));
}

static int	calculate_exit_code(char *arg)
{
	unsigned long long	n;
	int					s;
	int					i;

	if (!arg)
		return (0);
	n = 0;
	i = 0;
	s = 1;
	while (arg[i] == ' ' || (arg[i] >= '\t' && arg[i] <= '\r'))
		i++;
	if (arg[i] == '-')
		s = -1;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i] && ft_isdigit(arg[i]))
	{
		n = (n * 10) + (arg[i] - '0');
		i++;
	}
	return ((int)((n * s) % 256));
}

int	exit_shell(t_ext *ext, t_shell *shell)
{
	int	exit_code;

	if (ext->cmd->args[1] && !ft_str_isdigit(ext->cmd->args[1]))
	{
		ft_putstr_fd(RED, 2);
		ft_putstr_fd(SHELL_NAME ": exit: ", 2);
		ft_putstr_fd(ext->cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n" RESET, 2);
		cleanup_exit_resources(ext, shell);
		exit(2);
	}
	if (find_args_count(ext->cmd->args) > 2)
	{
		ft_putstr_fd(RED SHELL_NAME ": exit: too many arguments\n" RESET, 2);
		return (FALSE);
	}
	exit_code = calculate_exit_code(ext->cmd->args[1]);
	ft_putstr_fd("exit\n", 2);
	cleanup_exit_resources(ext, shell);
	exit(exit_code);
}
