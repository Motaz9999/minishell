/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_execute.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:12:15 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/11 23:07:23 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_eof_only_tokens(t_token *tokens)
{
	if (!tokens)
		return (0);
	return (tokens->type == TOKEN_EOF && tokens->next == NULL);
}

void	parse_and_execute(char *line, t_shell *shell)
{
	t_command *ptr;

	if (!line || !shell)
		return ;
	shell->tokens = lexer(line);
	if (!shell->tokens)
	{
		shell->last_exit_status = 2;
		return ;
	}
	if (validate_syntax(shell->tokens))
	{
		shell->last_exit_status = 2;
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
	if (is_eof_only_tokens(shell->tokens))
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
	shell->commands = parser(shell->tokens, shell);
	if (!shell->commands)
	{
		if (shell->last_exit_status != 130)
			shell->last_exit_status = 2;
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
	ptr = shell->commands;
	while (ptr)
	{
		expand_args_from_cmd(shell, ptr);
		ptr = ptr->next;
	}
	execute(shell);
	free_tokens(shell->tokens);
	free_commands(shell->commands);
	shell->tokens = NULL;
	shell->commands = NULL;
}
