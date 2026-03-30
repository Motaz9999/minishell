/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 21:10:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/27 17:37:49 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Create a new command
static t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->quote_types = NULL;
	cmd->next = NULL;
	return (cmd);
}

// Create a new redirection
static t_redirect	*create_redirect(t_redir_type type, char *file)
{
	t_redirect	*redir;

	redir = (t_redirect *)ft_calloc(1, sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	redir->fd = -1;
	redir->heredoc_fd = -1;
	redir->next = NULL;
	return (redir);
}

// Add redirection to a command
static void	add_redirect(t_command *cmd, t_redirect *redir)
{
	t_redirect	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = redir;
		return ;
	}
	current = cmd->redirects;
	while (current->next)
		current = current->next;
	current->next = redir;
}

// Add command to the pipeline
static void	add_command(t_command **head, t_command *new_cmd)
{
	t_command	*current;

	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

// Add an argument to a command
static void	add_arg_to_command(t_command *cmd, char *arg, int quote_type)
{
	int				arg_count;
	char			**new_args;
	t_quote_type	*new_quotes;
	int				i;

	arg_count = 0;
	if (cmd->args)
	{
		while (cmd->args[arg_count])
			arg_count++;
	}
	new_args = (char **)ft_calloc(arg_count + 2, sizeof(char *));
	new_quotes = (t_quote_type *)ft_calloc(arg_count + 2,
			sizeof(t_quote_type));
	if (!new_args || !new_quotes)
		return ;
	i = 0;
	while (i < arg_count)
	{
		new_args[i] = cmd->args[i];
		new_quotes[i] = cmd->quote_types[i];
		i++;
	}
	new_args[arg_count] = arg;
	new_quotes[arg_count] = quote_type;
	new_args[arg_count + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	if (cmd->quote_types)
		free(cmd->quote_types);
	cmd->args = new_args;
	cmd->quote_types = new_quotes;
}

// Main parser function
t_command	*parser(t_token *tokens, t_shell *shell)
{
	t_command	*commands;
	t_command	*current_cmd;
	t_token		*current_token;

	(void)shell;
	if (!tokens)
		return (NULL);
	commands = NULL;
	current_cmd = create_command();
	current_token = tokens;
	while (current_token && current_token->type != TOKEN_EOF)
	{
		if (current_token->type == TOKEN_WORD)
		{
			add_arg_to_command(current_cmd, ft_strdup(current_token->value),
				current_token->quote_type);
		}
		else if (current_token->type == TOKEN_PIPE)
		{
			add_command(&commands, current_cmd);
			current_cmd = create_command();
		}
		else if (current_token->type == TOKEN_REDIR_IN)
		{
			current_token = current_token->next;
			if (current_token && current_token->type == TOKEN_WORD)
				add_redirect(current_cmd,
					create_redirect(REDIR_IN, ft_strdup(current_token->value)));
		}
		else if (current_token->type == TOKEN_REDIR_OUT)
		{
			current_token = current_token->next;
			if (current_token && current_token->type == TOKEN_WORD)
				add_redirect(current_cmd,
					create_redirect(REDIR_OUT, ft_strdup(current_token->value)));
		}
		else if (current_token->type == TOKEN_REDIR_APPEND)
		{
			current_token = current_token->next;
			if (current_token && current_token->type == TOKEN_WORD)
				add_redirect(current_cmd,
					create_redirect(REDIR_APPEND, ft_strdup(current_token->value)));
		}
		else if (current_token->type == TOKEN_REDIR_HEREDOC)
		{
			current_token = current_token->next;
			if (current_token && current_token->type == TOKEN_WORD)
				add_redirect(current_cmd,
					create_redirect(REDIR_HEREDOC, ft_strdup(current_token->value)));
		}
		current_token = current_token->next;
	}
	if (current_cmd)
		add_command(&commands, current_cmd);
	return (commands);
}
