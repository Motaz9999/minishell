/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:00:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/24 21:16:50 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// counter for cmd num
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

// error already printed inside handle_redir
static void	fork_cmd_helper(char **envp, t_ext *ext, t_shell *shell,
		char *find_path)
{
	int	remaining_cmds;

	remaining_cmds = count_commands(ext->cmd);
	if (envp == NULL)
	{
		free(find_path);
		exit(error_syscall("envp", 1));
	}
	if (!handle_pipes(ext->prev_fd_in, ext->pipe_fds, remaining_cmds, shell))
	{
		free(find_path);
		ft_free_all2((void **)envp, NULL);
		exit(error_syscall("dup2", shell->last_exit_status));
	}
	if (!handle_redir(ext->cmd->redirects, shell))
	{
		free(find_path);
		ft_free_all2((void **)envp, NULL);
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
		return (-1);
	}
	if (pid == 0)
	{
		setup_signals_child();
		envp = make_envp(shell->env_list);
		fork_cmd_helper(envp, ext, shell, find_path);
		execve(find_path, ext->cmd->args, envp);
		error_execve(ext->cmd->args[0]);
		free(find_path);
		ft_free_all2((void **)envp, NULL);
	}
	free(find_path);
	return (pid);
}

// i have 2 line here it enough for the if statement
// and dont forget the signal
// first thing i must do is execute just one command
// F_OK → Test if the file exists.
// R_OK → Test if the file is readable.
// X_OK → Test if the file is executable.
// also in this fun
// dont need the others the 1 is for fork and must return pid
// here is the continue for this fun
pid_t	execute_one_cmd(t_ext *ext, t_shell *shell)
{
	char	*find_path;

	if (!ext || !ext->cmd || !ext->cmd->args || !ext->cmd->args[0])
		return (-1);
	if (get_builtin(ext->cmd) != FALSE)
		return (execute_builtin(ext, shell, 1));
	find_path = resolve_path(ext->cmd->args[0], shell->env_list);
	if (!find_path)
	{
		shell->last_exit_status = 127;
		error_cmd(ext->cmd->args[0], "Command not found", 127);
		return (-1);
	}
	if (access(find_path, X_OK) == -1)
	{
		shell->last_exit_status = 126;
		error_cmd(find_path, "Permission denied", 126);
		free(find_path);
		return (-1);
	}
	return (fork_cmd(shell, ext, find_path));
}
