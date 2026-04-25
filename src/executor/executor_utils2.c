/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:37:36 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 01:38:18 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Child-side pre-exec setup for env, pipes, and redirections.
*/
static void	fork_cmd_helper2(char **envp, t_ext *ext, t_shell *shell,
		char *find_path)
{
	int	remaining_cmds;

	remaining_cmds = count_commands(ext->cmd);
	if (envp == NULL)
	{
		free(find_path);
		free_pids(ext);
		exit(error_syscall("envp", 1));
	}
	if (!handle_pipes(ext->prev_fd_in, ext->pipe_fds, remaining_cmds, shell))
	{
		free(find_path);
		ft_free_all2((void **)envp, NULL);
		free_pids(ext);
		free_shell(shell);
		exit(error_syscall("dup2", shell->last_exit_status));
	}
	if (!handle_redir(ext->cmd->redirects, shell))
	{
		free(find_path);
		ft_free_all2((void **)envp, NULL);
		free_pids(ext);
		free_shell(shell);
		exit(shell->last_exit_status);
	}
}

void	fork_cmd_helper1(char **envp, t_ext *ext, t_shell *shell,
		char *find_path)
{
	setup_signals_child();
	close_all_heredoc_fds_except(shell->commands, ext->cmd);
	envp = make_envp(shell->env_list);
	fork_cmd_helper2(envp, ext, shell, find_path);
	if (find_path == NULL)
	{
		ft_free_all2((void **)envp, NULL);
		free_pids(ext);
		free_shell(shell);
		exit(0);
	}
	execve(find_path, ext->cmd->args, envp);
	free(find_path);
	ft_free_all2((void **)envp, NULL);
	free_pids(ext);
	free_shell(shell);
	error_execve(ext->cmd->args[0]);
}

static void	waiting_loop_update_last_status(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		shell->last_exit_status = status + 128;
		if (status == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
}

// this fun for handle the waiting error EINTR
// this when i resize the window may be cuse error
// with waiting and this signal must wait again for it
// if error happen and not from EINTR then we return false
// and thats mean something happen with child process
static int	waiting_loop_free_pids_helper(pid_t pid, int *status,
		t_shell *shell)
{
	pid_t	ret;

	ret = waitpid(pid, status, 0);
	while (ret == -1 && errno == EINTR)
		ret = waitpid(pid, status, 0);
	if (ret == -1)
	{
		shell->last_exit_status = error_syscall("waitpid", 1);
		return (FALSE);
	}
	return (TRUE);
}

// WIFEXITED(status)  -> Did the child exit normally?
// WEXITSTATUS(status) -> Extracts the exit code passed to exit()
// WIFSIGNALED(status) -> Did the process die because of a signal?
// User pressed Ctrl+C → SIGINT
// User pressed Ctrl+\ → SIGQUIT
// Program crashed (segfault) → SIGSEGV
// WTERMSIG(status) -> Returns the signal number that killed the process.
// last cmd: update exit status from child
// pids[i] == -1: cmd not found/no perm, last_exit_status already set
// Check WTERMSIG dynamically updating Exit statues matching correct norms!
void	waiting_loop_free_pids(pid_t pids[], t_shell *shell, int cmd_count)
{
	int	i;
	int	status;

	status = 0;
	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] != -1)
		{
			if (!waiting_loop_free_pids_helper(pids[i], &status, shell))
			{
				i++;
				continue ;
			}
			if (i == cmd_count - 1)
				waiting_loop_update_last_status(status, shell);
		}
		i++;
	}
	free(pids);
}
