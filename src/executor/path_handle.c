/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 21:05:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 11:58:24 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns 1 if cmd is an absolute (/foo) or relative (./foo ../foo) path,
// 0 if it is a bare command name that must be searched in PATH.
static int	is_path(char *cmd)
{
	if (cmd[0] == '/')
		return (1);
	if (cmd[0] == '.' && (cmd[1] == '/' || (cmd[1] == '.' && cmd[2] == '/')))
		return (1);
	return (0);
}

// F_OK → Test if the file exists.
// Paths are all entries after PATH= split by ':'
// here the case 2
static char	*test_cmd_and_free(char *cmd, char **paths)
{
	int		i;
	char	*full_path_test;

	full_path_test = NULL;
	i = 0;
	while (paths[i] != NULL)
	{
		if (cmd == NULL)
			break ;
		full_path_test = ft_merge_join(paths[i], "/", cmd);
		if (full_path_test == NULL)
			break ;
		if (access(full_path_test, F_OK) == 0)
			break ;
		free(full_path_test);
		full_path_test = NULL;
		i++;
	}
	free(cmd);
	ft_free_all2((void **)paths, NULL);
	return (full_path_test);
}

// Find the command's full path. Returns a malloc'd string or NULL.
// Case 1: absolute/relative path → duplicate and return as-is.
// Case 2: bare name → search each directory listed in PATH.
char	*resolve_path(char *cmd_name, t_env *env_list)
{
	t_env	*path_node;
	char	**split_paths;

	if (is_path(cmd_name))//now we know this cmd is start with ./ or ../  or /  so no need to search inside PATH
		return (ft_strdup(cmd_name));//just run it
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
	return (test_cmd_and_free(ft_strdup(cmd_name), split_paths));
}
