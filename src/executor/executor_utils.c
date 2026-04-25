/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:00:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/24 22:26:30 by moodeh           ###   ########.fr       */
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

	envp = NULL;
	pid = fork();
	if (pid == -1)
	{
		shell->last_exit_status = error_syscall("fork", 127);
		free(find_path);
		return (-1);
	}
	if (pid == 0)
		fork_cmd_helper1(envp, ext, shell, find_path);
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
	if (!ext || !ext->cmd)
		return (-1);
	if (!ext->cmd->args || !ext->cmd->args[0] || ext->cmd->args[0][0] == '\0')
	{
		if (ext->cmd->redirects)
			return (fork_cmd(shell, ext, NULL));
		return (-1);
	}
	if (get_builtin(ext->cmd) != FALSE)
		return (execute_builtin(ext, shell, 1));
	resolve_path(&find_path, ext->cmd->args[0], shell->env_list, shell);
	if (find_path == NULL)
		return (-1);
	return (fork_cmd(shell, ext, find_path));
}
