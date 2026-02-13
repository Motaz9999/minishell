/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:49:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/13 22:12:09 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// here we put the pipes then over ride it
//return 1 if it success and 0 if it fail (the dub) and it also closes the pipes and anything else
//here it just dup problem handle it inside the caller fun 
int	handle_pipes(int prev_fd_in, int pipes[], int remaining_cmd , t_shell *shell)
{
	if (remaining_cmd > 1) // have more than 1 cmd (middle) or start//but how to know if it the first(easy by prev_fd_in)
	{
		if (prev_fd_in == -1)//first cmd
		{
			close(pipes[0]);
			if (dup2(pipes[1] , 1) == -1)
				return FALSE;
		}
		else// not the first cmd soo there is previous pipe i should take the input from it
		{//but them in if statement
			if (dup2(prev_fd_in,0) == -1 || dup2(pipes[1] , 1) == -1)//now i am taking from the prev pipe and close it in parent bc it live in it
				return FALSE;
		}
	}
	else if (remaining_cmd == 1) // the  last cmd  but what if 
	{//how to check if 1 cmd or last cmd //use the same way 
		if (prev_fd_in == -1)//first cmd  but in the same time last (just one cmd)
			return TRUE;//there is nothing to do 
		else // last cmd (dont have new pipe from parent) but have input comes from other cmds
			dup2(prev_fd_in , 0);
	}
	return TRUE;
}






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
		error_syscall("fork", 127);
		return (-1);
	}
	if (pid == 0)
	{	// bc the env_list is always updated like envp and redir if it exist and pipes
		envp = make_envp(shell->env_list);////but these 2 handles in if and check for errors and what the exit code should be bc we inside a child process
		if (envp == NULL)
		{
			free(find_path);
			close(pipes[0]);
			close(pipes[1]);
			exit(error_syscall("envp" , 1));
		}
		if (!handle_pipes(prev_fd_in, pipes, remaining_cmds , shell) || !handle_redir(prev_fd_in, pipes,cmd->redirects,shell))
		{
			free(find_path);
			close(pipes[0]);
			close(pipes[1]);//
			exit(error_syscall("dup2" , 1));
		}//here pver ride the pipes and i see about prev_fd
		if (!handle_signal())//this is also have exit inside it soo it must send pipes to it // make last thing dont forget the exit code not the same so u must send the shell to it// here we control where the input and output
		{
			free(find_path);
			close(pipes[0]);
			close(pipes[1]);
			error_signal();
		}
		execve(find_path, cmd->args, envp);
		free(find_path);
		close(pipes[0]);
		close(pipes[1]);//there is no need for them now 
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
