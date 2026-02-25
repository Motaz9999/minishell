/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 22:34:11 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/25 05:56:35 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_on_args(char **args)
{
	int	count;

	count = 0;
	while (args[count] != NULL)
		count++;
	if (count == 2 || count == 1) // cd or cd dir/..
		return (TRUE);            // here it find home
	else
		return (FALSE); // cd dir1 dir2 // this is false
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
// so i need first to check_on pipes (the in pipe)
// first i want to build the cmd that is execute in parent process
// return true if i change the loc and false if not with error massage
// must be single cmd WITH NO pipe BUT if there are a pipe i should execute in child process
// so just make sure to fork it there are a pipe
// and it must TAKE ONLY ONE PATH ELSE PRINT ERROR
// args[0] => cd  args[1] =>dir  args[2] => error
int	cd(t_ext *ext, t_shell *shell)
{
	char	*path;
	char *cwd;
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
	if (chdir(path) == -1) // there are error in the path so i cant change it
	{
		
		return (error_syscall("cd", 0)); // fail on get into folder
	}//now we change path
	cwd = getcwd(NULL , 0);
	update_env_pwd(shell->env_list , cwd);
	free(cmd);//already get a copy of it inside the update
	return (TRUE);
}
