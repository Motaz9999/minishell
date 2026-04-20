/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:00:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/19 03:56:25 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Free pid array allocated for pipeline children.
*/
void	free_pids(t_ext *ext)
{
	if (ext && ext->pids)
		free(ext->pids);
}

/*
** Count commands in pipeline list.
*/
int	count_commands(t_command *cmds)
{
	int	count;

	count = 0;
	while (cmds != NULL)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

/*
** Child-side pre-exec setup for env, pipes, and redirections.
*/
static void	fork_cmd_helper(char **envp, t_ext *ext, t_shell *shell,
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

// here we fork the process
// and make a new fun to deal with the dir of input and output
// this is for the process
// bc the env_list is always updated like envp and redir if it exist and pipes
// but these 2 handles in if and check for errors and what
// the exit code should be bc we inside a child process
// helper to make it short
// it also have exit code bc it child
// save it in parent
static pid_t	fork_cmd(t_shell *shell, t_ext *ext, char *find_path)
{
	pid_t	pid;
	char	**envp;

	pid = fork();
	if (pid == -1)
	{
		shell->last_exit_status = error_syscall("fork", 127);
		free(find_path);
		return (-1);
	}
	if (pid == 0)
	{
		setup_signals_child();
		close_all_heredoc_fds_except(shell->commands, ext->cmd);
		envp = make_envp(shell->env_list);
		fork_cmd_helper(envp, ext, shell, find_path);
		execve(find_path, ext->cmd->args, envp);
		free(find_path);
		ft_free_all2((void **)envp, NULL);
		free_pids(ext);
		free_shell(shell);
		error_execve(ext->cmd->args[0]);
	}
	free(find_path);
	return (pid);
}

/*
** Execute a single pipeline command node.
** Dispatch builtins or fork/exec external commands.
*/
pid_t	execute_one_cmd(t_ext *ext, t_shell *shell)
{
	char	*find_path;

	find_path = NULL;
	if (!ext || !ext->cmd || !ext->cmd->args || !ext->cmd->args[0]
		|| ext->cmd->args[0][0] == '\0')
		return (-1);
	if (get_builtin(ext->cmd) != FALSE)
		return (execute_builtin(ext, shell, 1));
	resolve_path(&find_path, ext->cmd->args[0], shell->env_list, shell);
	if (find_path == NULL)
		return (-1);
	return (fork_cmd(shell, ext, find_path));
}
