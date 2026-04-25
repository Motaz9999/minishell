/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 21:05:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/24 23:42:36 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_path(char *cmd)
{
	if (cmd[0] == '/')
		return (TRUE);
	if (cmd[0] == '.' && (cmd[1] == '/' || (cmd[1] == '.' && cmd[2] == '/')))
		return (TRUE);
	return (FALSE);
}

static void	handle_bare_cmd(char **find_path, char *cmd_name, char **paths,
		t_shell *shell)
{
	int		i;
	char	*full_path_test;

	full_path_test = NULL;
	i = 0;
	while (paths[i] != NULL)
	{
		if (cmd_name == NULL)
			break ;
		full_path_test = ft_merge_join(paths[i], "/", cmd_name);
		if (full_path_test == NULL)
			break ;
		if (access(full_path_test, F_OK) == 0)
			break ;
		free(full_path_test);
		full_path_test = NULL;
		i++;
	}
	*find_path = full_path_test;
	if (*find_path == NULL)
		shell->last_exit_status = error_cmd(cmd_name, "command not found", 127);
}

static int	check_on_dir(char *cmd, t_shell *shell)
{
	struct stat	path_stat;

	if (stat(cmd, &path_stat) == -1)
	{
		shell->last_exit_status = error_syscall("stat", 1);
		return (FALSE);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		shell->last_exit_status = error_cmd(cmd, "Is a directory", 126);
		return (FALSE);
	}
	return (TRUE);
}

static void	handle_path_cmd(char **find_path, char *cmd_name, t_shell *shell)
{
	if (access(cmd_name, F_OK) != 0)
	{
		shell->last_exit_status = error_cmd(cmd_name,
				"No such file or directory", 127);
		return ;
	}
	if (!check_on_dir(cmd_name, shell))
		return ;
	if (access(cmd_name, X_OK) != 0)
	{
		shell->last_exit_status = error_cmd(cmd_name, "Permission denied", 126);
		return ;
	}
	*find_path = ft_strdup(cmd_name);
	if (*find_path == NULL)
		shell->last_exit_status = error_syscall("malloc", 1);
}

void	resolve_path(char **find_path, char *cmd_name, t_env *env_list,
		t_shell *shell)
{
	t_env	*path_node;
	char	**split_paths;

	if (is_path(cmd_name))
	{
		handle_path_cmd(find_path, cmd_name, shell);
		return ;
	}
	path_node = find_node(env_list, "PATH");
	if (!path_node || !path_node->value)
	{
		shell->last_exit_status = error_cmd(cmd_name, "command not found", 127);
		return ;
	}
	split_paths = ft_split(path_node->value, ':');
	if (!check_on_split(split_paths, cmd_name, shell))
		return ;
	handle_bare_cmd(find_path, cmd_name, split_paths, shell);
	ft_free_all2((void *)split_paths, NULL);
}
