/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 21:05:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/08 22:40:21 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// here i get the path of the commands and t_env
// soo i must find the command relative or abs
static int	is_relative_or_abs(char *cmd)
{
	int	i;

	i = 0;
	// abs
	if (cmd[0] == '/')
		return (1); // true  (here i dont have to add full path to test)
					// relative
	if (cmd[0] == '.' && (cmd[1] == '/' || (cmd[1] == '.' && cmd[2] == '/')))
		return (0); //  false i have to add full path to test
}

// f_ok r_ok x_ok these flags for access to use bitwise |
// F_OK → Test if the file exists.
//paths it is all after PATH= split between :
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
		full_path_test = ft_strjoin(paths[i], cmd);
		if (full_path_test == NULL)
			break ;
		if (access(full_path_test, F_OK) == 0)
			break ; // found it
		free(full_path_test);
		full_path_test = NULL;
		i++;
	}
	free(cmd);
	ft_free_all2((void **)paths, NULL);
	return (full_path_test);
}

// this fun for find if the cmd is correct and it executable
// dont forgot to free it after use it
char	*resolve_path(char *cmd_name, t_env *env_list)
{
	char	*real_path;
	char	**split_paths;

	while (env_list->key != "PATH")
		env_list = env_list->next;
	if (env_list->key != "PATH")
		return (FALSE);
	split_paths = ft_split(env_list->value, ':');
	if (split_paths == NULL)
		return (FALSE);
	if (*split_paths == NULL)
	{
		free(split_paths);
		return (FALSE);
	}
	if (!is_relative_or_abs(cmd_name))
		real_path = make_it_abs(cmd_name);
	else
		real_path = ft_strdup(cmd_name);
	return (test_cmd_and_free(real_path, split_paths));
}
