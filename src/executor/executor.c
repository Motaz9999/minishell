/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:49:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/15 00:51:27 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"








void clean_up_v1();
void fork_cmd_helper();
// here we fork the process
// and make a new fun to deal with the dir of input and output
pid_t	fork_cmd(t_shell *shell, t_command *cmd, char **find_path,
		int prev_fd_in, int pipes[])
{
	pid_t	pid;
	char	**envp;
	int		remaining_cmds;

	remaining_cmds = count_commands(cmd);
	pid = fork();                         // this is for the process
	if (pid == -1)
	{
		shell->last_exit_status = 127; // we are now inside child soo the exit
		exit (error_syscall("fork", 127));
	}
	if (pid == 0)
	{	// bc the env_list is always updated like envp and redir if it exist and pipes
		envp = make_envp(shell->env_list);////but these 2 handles in if and check for errors and what the exit code should be bc we inside a child process
		if (envp == NULL)
		{
			ft_free_all((char*)find_path ,(char*) envp , NULL);
			exit(error_syscall("envp" , 1));
		}
		if (!handle_pipes(prev_fd_in, pipes, remaining_cmds , shell) ||
		 !handle_redir(cmd->redirects,shell))
		{
			ft_free_all((char*)find_path ,(char*) envp , NULL);
			exit(error_syscall("dup2" , shell->last_exit_status));
		}//here pver ride the pipes and i see about prev_fd
		if (!handle_signal())//this is also have exit inside it soo it must send pipes to it // make last thing dont forget the exit code not the same so u must send the shell to it// here we control where the input and output
		{
			ft_free_all((char*)find_path ,(char*) envp , NULL);
			exit (error_signal());
		}
		execve(find_path, cmd->args, envp);
		free(find_path);
		error_execve(cmd->args[0]); // it also have exit code bc it child
	}
	free(find_path);
	return (pid);//save it in parent
}// i have 2 line here it enough for the if statement and dont forget the signal 
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
		return -1;
	}
	if (access(find_path, R_OK) == -1)
	{
		shell->last_exit_status = 126; // i just update here
		error_cmd(find_path, "permission denied", 126);
		free(find_path);
		return -1;
	}
	if (access(find_path, X_OK) == -1)
	{
		shell->last_exit_status = 126; // i just update here
		error_cmd(find_path, "Not executable", 126);
		free(find_path);
		return -1;
	}
	return (fork_cmd(shell, cmd, find_path, prev_fd_in, pipes));
	// here is the continue for this fun
}












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









void execute_helper2(t_ext *ext , t_shell *shell)
{
			if (is_builtin(ext->cmd))
			execute_builtin();// this type i totally work different with it bc the 2 types
		else
			ext->pids[ext->i] = execute_one_cmd(ext->cmd, shell, ext->prev_fd_in,
												ext->pipe_fds); // the normal cmd
		if (ext->prev_fd_in != -1)
			close(ext->prev_fd_in);
		if (ext->cmd->next)
		{
			close(ext->pipe_fds[1]);           // always close it in parent 
			ext->prev_fd_in = ext->pipe_fds[0]; // close in the next time 
		}
}
void execute_helper(t_ext *ext , t_shell *shell)
{
	while (ext->cmd)
	{
		if (ext->cmd->next != NULL)
			if (pipe(ext->pipe_fds) == -1)// here i create pipe (each 2 process have 2 types )
			{
				shell->last_exit_status = -1;
				error_syscall("pipe", -1);
				free(ext->pids);// i want for each 2 cmd to make them a pipe in first cmd
				return;
			}
		else // if next cmd is null there is no  need for pipe
			{
				ext->pipe_fds[0] = -1;
				ext->pipe_fds[1] = -1;
			}
			execute_helper2(ext , shell);
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
	ext.prev_fd_in = -1;       // bc i dont have a previous pipe
	ext.cmd = shell->commands; // head
	ext.i = 0;
	// but the whole loop in another fun
	execute_helper(&ext , shell);
	waiting_loop_free_pids(); // here we waiting and also update the status
}
