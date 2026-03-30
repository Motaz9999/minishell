/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 21:05:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/30 19:04:02 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns 1 if cmd is an absolute (/../cmd) or relative (./cmd ../cmd) path,
// 0 if it is a bare command name that must be searched in PATH.
static int	is_path(char *cmd)
{
	if (cmd[0] == '/')
		return (TRUE);
	if (cmd[0] == '.' && (cmd[1] == '/' || (cmd[1] == '.' && cmd[2] == '/')))
		return (TRUE);
	return (FALSE);
}

// this fun for if u give it a bare cmd like ls or cat
void	handle_bare_cmd(char **find_path, char *cmd_name, char **paths,
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
		shell->last_exit_status = error_cmd(cmd_name, "Command not found", 127);
}
// this stat fun is used to give me info about the file/folder
// like size it is a dir or a file and last update
// all this stored in struct of type stat
// struct stat {
//   dev_t   st_dev;  // device
//   ino_t   st_ino;  // inode number
//   mode_t  st_mode; // file type + permissions (**)
//   nlink_t st_nlink;// number of hard links
//   uid_t   st_uid;  // user ID
//   gid_t   st_gid;  // group ID
//   off_t   st_size; // file size (bytes)
//   time_t  st_atime;// last access
//   time_t  st_mtime;// last modification
//   time_t  st_ctime;// last status change
// };
// and i will be using different fun to check
int	check_on_dir(char *cmd, t_shell *shell)
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

// this cmd is for doing anything that help FULL PATH CMD
// and there is 2 types of it 1. relative cmd that start with ./ or ../
//								2. is FULL PATH CMD  start with /
// F_OK → Test if the file exists.
// R_OK → Test if the file is readable.
// X_OK → Test if the file is executable.
//	if (access(cmd_name, F_OK) != TRUE) // dose the file exist
void	handle_path_cmd(char **find_path, char *cmd_name, t_shell *shell)
{
	if (access(cmd_name, F_OK) != TRUE)
	{
		shell->last_exit_status = error_cmd(ext->cmd->args[0],
				"No such file or directory", 127);
		return ;
	}
	// but what if it a dir ?
	if (!check_on_dir(cmd_name, shell))
		return ;
	if (access(cmd_name, X_OK) != TRUE)
	{
		shell->last_exit_status = error_cmd(ext->cmd->args[0],
				"Permission denied", 126);
		return ;
	}
	*find_path = cmd_name;
}

// ok this fun is for resolving what path and it have everything
// first i have 2 types of paths one start with '/'
//	if (is_path(cmd_name)) // bare cmd or path cmd

void	resolve_path_helper(char **find_path, char *cmd_name, char **paths,
		t_shell *shell)
{
	if (is_path(cmd_name))
		handle_path_cmd(find_path, cmd_name, shell);
	else
		handle_bare_cmd(find_path, cmd_name, paths, shell);
}

// Find the command's full path. Returns a malloc'd string or NULL.
// Case 1: absolute/relative path → duplicate and return as-is.
// Case 2: bare name → search each directory listed in PATH.
// now we know this cmd is start with ./ or ../  or
//  so no need to search inside PATH
char	*resolve_path(char **find_path, char *cmd_name, t_env *env_list,
		t_shell *shell)
{
	t_env	*path_node;
	char	**split_paths;

	if (is_path(cmd_name))
		return (ft_strdup(cmd_name));
	path_node = find_node(env_list, "PATH");
	if (!path_node || !path_node->value)
		return (NULL);
	split_paths = ft_split(path_node->value, ':');
	if (split_paths == NULL)
		return (NULL);
	if (*split_paths == NULL)
	{
		free(split_paths);
		return (NULL);
	}
	resolve_path_helper(find_path, cmd_name, split_paths, shell);
	ft_free_all2((void *)split_paths, NULL);
}
