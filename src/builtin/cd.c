/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:34:11 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/08 16:44:49 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_on_args(char **args)
{
	int	count;

	if (!args)
		return (FALSE);
	count = 0;
	while (args[count] != NULL)
		count++;
	if (count == 1 || count == 2)
		return (TRUE);
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(SHELL_NAME ": cd: too many arguments", 2);
	ft_putstr_fd(RESET, 2);
	ft_putstr_fd("\n", 2);
	return (FALSE);
}

char	*get_home_path(t_env *env_list)
{
	char	*path;
	t_env	*node;

	node = find_node(env_list, "HOME");
	if (node == NULL)
		return (NULL);
	path = node->value;
	if (path == NULL)
		return (NULL);
	return (path);
}

static int	cd_helper_error(char *path)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(SHELL_NAME ": cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(RESET, 2);
	ft_putstr_fd("\n", 2);
	return (FALSE);
}

// so i need first to check_on pipes (the in pipe)
// first i want to build the cmd that is execute in parent process
// return true if i change the loc and false if not with error massage
// must be single cmd WITH NO pipe BUT 
//if there are a pipe i should execute in child process
// so just make sure to fork it there are a pipe
// and it must TAKE ONLY ONE PATH ELSE PRINT ERROR
// args[0] => cd  args[1] =>dir  args[2] => error
// free(cwd); // already copied inside update_env_pwd
int	cd(t_ext *ext, t_shell *shell)
{
	char	*path;
	char	*cwd;

	if (!check_on_args(ext->cmd->args))
		return (FALSE);
	if (!ext->cmd->args[1])
	{
		path = get_home_path(shell->env_list);
		if (path == NULL)
			return (error_cmd("cd", "HOME not set", 1));
	}
	else
		path = ext->cmd->args[1];
	if (chdir(path) == -1)
		return (cd_helper_error(path));
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (error_syscall("getcwd", 0));
	update_env_pwd(shell->env_list, cwd);
	free(cwd);
	return (TRUE);
}
