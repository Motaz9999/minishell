/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:49:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 10:12:00 by moodeh           ###   ########.fr       */
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
void	clean_up_v1(void);
void	fork_cmd_helper(char **envp, t_ext *ext, t_shell *shell,
		char *find_path)
{
	int	remaining_cmds;

	remaining_cmds = count_commands(ext->cmd);
	if (envp == NULL)
	{
		free(find_path);
		exit(error_syscall("envp", 1));
	}
	if (!handle_pipes(ext->prev_fd_in, ext->pipe_fds, remaining_cmds, shell)
		|| !handle_redir(ext->cmd->redirects, shell))
	{
		free(find_path);
		ft_free_all2((void **)envp, NULL);
		exit(error_syscall("dup2", shell->last_exit_status));
	}
}
// here we fork the process
// and make a new fun to deal with the dir of input and output
pid_t	fork_cmd(t_shell *shell, t_ext *ext, char *find_path)
{
	pid_t	pid;
	char	**envp;

	pid = fork(); // this is for the process
	if (pid == -1)
	{
		shell->last_exit_status = 127; // we are now inside child soo the exit
		error_syscall("fork", 127); // code is for child and we just print the error
		return -1;// we just return it and handle it in the parent
	}
	if (pid == 0)
	{
		// bc the env_list is always updated like envp and redir if it exist and pipes
		setup_signals_child();
		envp = make_envp(shell->env_list);// but these 2 handles in if and check for errors and what the exit code should be bc we inside a child process
		fork_cmd_helper(envp, ext, shell, find_path); // helper to make it short
		execve(find_path, ext->cmd->args, envp);
		free(find_path);
		ft_free_all2((void **)envp, NULL);
		error_execve(ext->cmd->args[0]); // it also have exit code bc it child
	}
	free(find_path);
	return (pid); // save it in parent
}

// i have 2 line here it enough for the if statement and dont forget the signal
// first thing i must do is execute just one command
// F_OK → Test if the file exists.
// R_OK → Test if the file is readable.
// X_OK → Test if the file is executable.
// also in this fun
pid_t	execute_one_cmd(t_ext *ext, t_shell *shell)
{
	char	*find_path;

	if (get_builtin(ext->cmd) != FALSE)
		return (execute_builtin(ext, shell, 1));                 
			// dont need the others the 1 is for fork and must return pid
	find_path = resolve_path(ext->cmd->args[0], shell->env_list);
		// handle and free all in fun make it later name it checking for file and
	if (!find_path)
	{
		shell->last_exit_status = 127; // i just update here
		error_cmd(ext->cmd->args[0], "Command not found", 127);
		return (-1);
	}
	if (access(find_path, R_OK) == -1)
	{
		shell->last_exit_status = 126; // i just update here
		error_cmd(find_path, "permission denied", 126);
		free(find_path);
		return (-1);
	}
	if (access(find_path, X_OK) == -1)
	{
		shell->last_exit_status = 126; // i just update here
		error_cmd(find_path, "Not executable", 126);
		free(find_path);
		return (-1);
	}
	return (fork_cmd(shell, ext, find_path));
		// here is the continue for this fun
}

void	execute_helper2(t_ext *ext, t_shell *shell)
{
	// just move to before the execve fun
	// hmm but if NOT FOR FORKING
	// if (is_builtin(ext->cmd))
	// 	execute_builtin();
	// this type i totally work different with it bc the 2 types
	// else
	ext->pids[ext->i] = execute_one_cmd(ext, shell); // the normal cmd
	if (ext->prev_fd_in != -1)
		close(ext->prev_fd_in);
	if (ext->cmd->next)
	{
		close(ext->pipe_fds[1]);            // always close it in parent
		ext->prev_fd_in = ext->pipe_fds[0]; // close in the next time
	}
}

void	execute_helper(t_ext *ext, t_shell *shell)
{
	if (count_commands(ext->cmd) == 1) // this mean that i have just 1 cmd
	{
		if (get_builtin(ext->cmd) != FALSE) // check if it from the builtins
		{
			execute_builtin(ext, shell, 0);
				// dont have to fork and dont need pid here
			return ;
		}
	}
	while (ext->cmd)
	{
		if (ext->cmd->next != NULL)
		{
			if (pipe(ext->pipe_fds) == -1)
			{
				shell->last_exit_status = -1;
				error_syscall("pipe", -1);
				free(ext->pids);
				return ;
			}
		}
		else // next cmd is null, no pipe needed
		{
			ext->pipe_fds[0] = -1;
			ext->pipe_fds[1] = -1;
		}
		execute_helper2(ext, shell);
		ext->cmd = ext->cmd->next;
		ext->i++;
	}
}

// this is the parent process
// make a custom struct
void	execute(t_shell *shell)
{
	t_ext	ext;

	// but them all one struct this best
	if (!shell->commands)
		return ;
	ext.pids = malloc(sizeof(pid_t) * count_commands(shell->commands));
	if (ext.pids == NULL)
	{
		shell->last_exit_status = 1;
		error_syscall("malloc", 1);
		return ;
	}
	ft_memset(ext.pids, -1,count_commands(shell->commands)); // just to be safe
	ext.prev_fd_in = -1;                                                     
		// bc i dont have a previous pipe
	ext.cmd = shell->commands;                                               
		// head
	ext.i = 0;                                                               
		// but the whole loop in another fun
	execute_helper(&ext, shell);                                             
		// fork all child here
	waiting_loop_free_pids(ext.pids, shell, count_commands(shell->commands));
		// here we waiting and also update the status
}

// WIFEXITED(status)  -> Did the child exit normally?
// WEXITSTATUS(status) -> Extracts the exit code passed to exit()
// WIFSIGNALED(status) -> Did the process die because of a signal?
// User pressed Ctrl+C → SIGINT
// User pressed Ctrl+\ → SIGQUIT
// Program crashed (segfault) → SIGSEGV
// WTERMSIG(status) -> Returns the signal number that killed the process.
void	waiting_loop_free_pids(pid_t pids[], t_shell *shell, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
			waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1) // last cmd
		{
			if (WIFEXITED(status)) // normal exit so i can now get the exit code
				shell->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status)) // sig exit
			{
				status = WTERMSIG(status);             
					// we want to save the exit code
				shell->last_exit_status = status + 128;
					// save it as signal type error (this for child)
				if (status == SIGQUIT)
					write(1, "Quit (core dumped)\n", 19);
			}
		}
		i++;
	}
	free(pids);
}
