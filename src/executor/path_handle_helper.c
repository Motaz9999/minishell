/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handle_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 23:37:28 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/25 02:59:07 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_on_split(char **split_paths, char *cmd_name, t_shell *shell)
{
	if (split_paths == NULL)
		return (FALSE);
	if (*split_paths == NULL)
	{
		shell->last_exit_status = error_cmd(cmd_name,
				"No such file or directory", 127);
		free(split_paths);
		return (FALSE);
	}
	return (TRUE);
}
