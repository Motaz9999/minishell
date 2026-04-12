/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:49:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/13 02:28:51 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// WIFEXITED(status)  -> Did the child exit normally?
// WEXITSTATUS(status) -> Extracts the exit code passed to exit()
// WIFSIGNALED(status) -> Did the process die because of a signal?
// User pressed Ctrl+C → SIGINT
// User pressed Ctrl+\ → SIGQUIT
// Program crashed (segfault) → SIGSEGV
// WTERMSIG(status) -> Returns the signal number that killed the process.
// last cmd: update exit status from child
// pids[i] == -1: cmd not found/no perm, last_exit_status already set
static void	waiting_loop_free_pids(pid_t pids[], t_shell *shell, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] != -1)
		{
			waitpid(pids[i], &status, 0);
			if (i == cmd_count - 1)
			{
				if (WIFEXITED(status))
					shell->last_exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
				{
					status = WTERMSIG(status);
					shell->last_exit_status = status + 128;
					if (status == SIGQUIT)
						write(1, "Quit (core dumped)\n", 19);
				}
			}
		}
		i++;
	}
	free(pids);
}

// just move to before the execve fun
// hmm but if NOT FOR FORKING
// if (is_builtin(ext->cmd))
// 	execute_builtin();
// this type i totally work different with it bc the 2 types
// else  // the normal cmd
// always close it in parent
// close in the next time
static void	execute_helper3(t_ext *ext, t_shell *shell)
{
	ext->pids[ext->i] = execute_one_cmd(ext, shell);
	if (ext->prev_fd_in != -1)
		close(ext->prev_fd_in);
	if (ext->cmd->next)
	{
		close(ext->pipe_fds[1]);
		ext->prev_fd_in = ext->pipe_fds[0];
	}
}

// this fun to make a pipe for cmds
static int	execute_helper2(t_ext *ext, t_shell *shell)
{
	if (!ext->cmd->next)
	{
		ext->pipe_fds[0] = -1;
		ext->pipe_fds[1] = -1;
		return (FALSE);
	}
	if (pipe(ext->pipe_fds) == -1)
	{
		shell->last_exit_status = error_syscall("pipe", -1);
		return (TRUE);
	}
	return (FALSE);
}

// this mean that i have just 1 cmd
// check if it from the builtins
// here i create pipe (each 2 process have 2 types )
// i want for each 2 cmd to make them a pipe in first cmd
// if next cmd is null there is no  need for pipe
static void	execute_helper(t_ext *ext, t_shell *shell)
{
	while (ext->cmd)
	{
		if (execute_helper2(ext, shell))
			return ;
		execute_helper3(ext, shell);
		ext->cmd = ext->cmd->next;
		ext->i++;
	}
}

// this is the parent process
// make a custom struct
// but them all one struct this best
// init value
// bc i dont have a previous pipe
// head
// but the whole loop in another fun
// fork all child here
// here we waiting and also update the status
void	execute(t_shell *shell)
{
	t_ext	ext;
	int		count_cmd;

	if (!shell->commands)
		return ;
	count_cmd = count_commands(shell->commands);
	ft_memset(&ext, 0, sizeof(t_ext));
	ext.cmd = shell->commands;
	ext.prev_fd_in = -1;
	if (count_cmd == 1 && get_builtin(shell->commands) != BI_NONE)
	{
		ext.pipe_fds[0] = -1;
		ext.pipe_fds[1] = -1;
		execute_builtin(&ext, shell, 0);
		return ;
	}
	ext.pids = malloc(sizeof(pid_t) * count_cmd);
	if (ext.pids == NULL)
		return ;
	ft_memset(ext.pids, -1, count_cmd * sizeof(pid_t));
	execute_helper(&ext, shell);
	setup_signals_waits();
	waiting_loop_free_pids(ext.pids, shell, count_cmd);
	setup_signals_parent(); // return to parent mode
}
