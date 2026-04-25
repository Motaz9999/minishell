/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handle_helper2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 23:37:28 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/25 02:59:07 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	resolve_path_helper(char *cmd_name, t_shell *shell, char **split_paths)
{
	shell->last_exit_status = error_cmd(cmd_name, "No such file or directory",
			127);
	ft_free_all2((void *)split_paths, NULL);
}
