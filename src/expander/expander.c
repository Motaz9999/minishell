/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 17:50:55 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 18:01:29 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//first what i well deal with is shell and cmds
//this fun is used once for each cmd 
int	expand_args_from_cmd(t_shell *shell , t_command *cmd)
{
    int i = 0;
    while (cmd->args[i])
    {
        if (cmd->quote_types[i])
    }
    
	return (TRUE);
}
