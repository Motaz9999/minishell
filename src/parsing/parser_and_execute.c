/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_execute.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:12:15 by moodeh            #+#    #+#             */
/*   Updated: 2026/04/16 01:56:08 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_eof_only_tokens(t_token *tokens)
{
	if (!tokens)
		return (FALSE);
	return (tokens->type == TOKEN_EOF && tokens->next == NULL);
}

static int	tokenize_and_validate(char *line, t_shell *shell)
{
	shell->tokens = lexer(line);
	if (!shell->tokens)
	{
		shell->last_exit_status = 2;
		return (TRUE);
	}
	if (validate_syntax(shell->tokens))
	{
		shell->last_exit_status = 2;
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (TRUE);
	}
	if (!is_eof_only_tokens(shell->tokens))
		return (FALSE);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	return (TRUE);
}

static int	build_and_expand_commands(t_shell *shell)
{
	t_command	*ptr;

	shell->commands = parser(shell->tokens, shell);
	if (!shell->commands)
	{
		if (shell->last_exit_status != 130)
			shell->last_exit_status = 2;
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (TRUE);
	}
	ptr = shell->commands;
	while (ptr)
	{
		expand_args_from_cmd(shell, ptr);
		ptr = ptr->next;
	}
	return (FALSE);
}

void	parse_and_execute(char *line, t_shell *shell)
{
	if (!line || !shell)
		return ;
	if (tokenize_and_validate(line, shell))
		return ;
	if (build_and_expand_commands(shell))
		return ;
	setup_signals_waits();
	execute(shell);
	free_tokens(shell->tokens);
	free_commands(shell->commands);
	shell->tokens = NULL;
	shell->commands = NULL;
}
