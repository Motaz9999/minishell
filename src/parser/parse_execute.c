/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 09:32:15 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Entry point: tokenize → expand → parse → execute → cleanup
void	parse_and_execute(char *line, t_shell *shell)
{
	shell->tokens = lexer(line);
	if (!shell->tokens)
		return ;
	expand_tokens(shell->tokens, shell);
	shell->commands = parser(shell->tokens, shell);
	if (!shell->commands)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
	execute(shell);
	free_tokens(shell->tokens);
	free_commands(shell->commands);
	shell->tokens = NULL;
	shell->commands = NULL;
}
