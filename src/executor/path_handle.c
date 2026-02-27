/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 21:05:26 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 09:19:45 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// here i get the path of the commands and t_env
// soo i must find the command relative or abs
static int	is_relative_or_abs(char *cmd)
{
	// abs
	if (cmd[0] == '/')
		return (TRUE);
			// true  (here i dont have to add full path to test)// relative
	else if (cmd[0] == '.' && (cmd[1] == '/' || (cmd[1] == '.'
				&& cmd[2] == '/')))
		return (FALSE); //  false i have to add full path to test
	else if (ft_isalnum(cmd[0]) == 0)// if the first char is not alphanumeric and not . or// so it is not relative or abs
		return (FALSE);
	return (FALSE); // plain name like "ls" -> needs PATH search
}

//this fun for make the cmd relative to abs
static char	*make_full_path(char *path, char *cmd)
{
	char	*merged;
	char	*temp;

	temp = ft_strjoin(path, "/");
	if (temp == NULL)
		return (NULL);
	merged = ft_strjoin(temp, cmd);
	free(temp);
	if (merged == NULL)
		return (NULL);
	return (merged);
}


// f_ok r_ok x_ok these flags for access to use bitwise |
// F_OK → Test if the file exists.
// paths it is all after PATH= split between :
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
		full_path_test = make_full_path(paths[i], cmd);
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
char *make_it_abs(char *cmd_name)
{
	char	*cwd;
	char	*full_path;

	cwd = getcwd(NULL, 0);//this fun return the current working directory as a string, and it allocates memory for it. 
	if (cwd == NULL)
		return (NULL);
	full_path = make_full_path(cwd, cmd_name);
	free(cwd);
	return (full_path);
}

char	*resolve_path(char *cmd_name, t_env *env_list)
{
	char	*real_path;
	char	**split_paths;

	while (env_list && ft_strcmp(env_list->key, "PATH") != 0)
		env_list = env_list->next;
	if (!env_list)
		return (NULL); // PATH not found in env
	split_paths = ft_split(env_list->value, ':');
	if (split_paths == NULL)
		return (NULL);
	if (*split_paths == NULL)
	{
		free(split_paths);
		return (NULL);
	}
	if (!is_relative_or_abs(cmd_name))
		real_path = make_it_abs(cmd_name);
	else
		real_path = ft_strdup(cmd_name);
	return (test_cmd_and_free(real_path, split_paths));
}
