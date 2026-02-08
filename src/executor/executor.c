/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:49:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/09 01:11:54 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// here we fork the process
// and make a new fun to deal with the dir of input and output
pid_t	fork_cmd(t_shell *shell, t_command *cmd, char **find_path, int pipes[])
{
	pid_t	pid;
	char	**envp;

	pid = fork(); // this is for the process
	if (pid == -1)
	{
		shell->last_exit_status = 127; // we are now inside chiled soo the exit
		error_syscall("fork", 127);
		return (-1);
	}
	if (pid == 0)
	{
		// bc the env_list is always updated like envp and redir if it exisit and pipes
		envp = make_envp(shell->env_list);
		execve(find_path, cmd->args, envp);
		free(find_path);
		error_execve(cmd->args[0]); // it also have exit code bc it child
	}
	free(find_path);
	return (pid);
}
// first thing i must do is execute just one command
// F_OK → Test if the file exists.
// R_OK → Test if the file is readable.
// X_OK → Test if the file is executable.
// also in this fun
pid_t	execute_one_cmd(t_command *cmd, t_shell *shell, int prev_fd_in,
		int pipes[])
{
	char	*find_path;

	find_path = resolve_path(cmd->args[0], shell->env_list);
	// handle and free all in fun make it later name it checking for file and
	if (!find_path)
	{
		shell->last_exit_status = 127; // i just update here
		error_cmd(cmd->args[0], "Command not found", 127);
		return ;
	}
	if (access(find_path, R_OK) == -1)
	{
		shell->last_exit_status = 126; // i just update here
		error_cmd(find_path, "permission denied", 126);
		free(find_path);
		return ;
	}
	if (access(find_path, X_OK) == -1)
	{
		shell->last_exit_status = 126; // i just update here
		error_cmd(find_path, "Not executable", 126);
		free(find_path);
		return ;
	}
	return (fork_cmd(shell, cmd, find_path, pipes));
	// here is the continue for this fun
}
// this is the parent process
void	execute(t_shell *shell)
{
	int			pipe_fds[2];
	int			prev_fd_in;
	pid_t		*pids;
	int			i;
	t_command	*cmd;

	// but them all one struct this best
	if (!shell->commands)
		return ;
	pids = malloc(sizeof(pid_t) * count_commands(shell->commands));
	prev_fd_in = -1;       // bc i dont have a previous pipe
	cmd = shell->commands; // head
	i = 0;
	// but the whole loop in another fun
	while (cmd)
	{
		if (cmd->next != NULL)
			if (pipe(pipe_fds) == -1)
			// here i create pipe (each 2 process have 2 types )
			{
				shell->last_exit_status = -1;
				error_syscall("pipe", -1);
				free(pids);
				return ;
				// i want for each 2 cmd to make them a pipe in first cmd
			}
		if (is_bulitin(cmd))
			execute_builtin();
		// this type i totally work different with it bc the 2 types
		else
			pids[i] = execute_one_cmd(cmd, shell, prev_fd_in, pipe_fds);
		if (prev_fd_in != -1)
			close(prev_fd_in);
		if (cmd->next)
		{
			close(pipe_fds[1]);
			prev_fd_in = pipe_fds[0];//so i can send it to the new child 
		}
		cmd = cmd->next;
		i++;
	}
	waiting_loop_free_pids();
}
