/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 21:10:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 22:20:21 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Free a single redirection
static void	free_redirect(t_redirect *redir)
{
	if (!redir)
		return ;
	if (redir->file)
		free(redir->file);
	free(redir);
}

// Free all redirections of a command
void	free_redirects(t_redirect *redirects)
{
	t_redirect	*current;
	t_redirect	*tmp;

	if (!redirects)
		return ;
	current = redirects;
	while (current)
	{
		tmp = current->next;
		free_redirect(current);
		current = tmp;
	}
}

// Free a single command
static void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->quote_types)
		free(cmd->quote_types);
	if (cmd->redirects)
		free_redirects(cmd->redirects);
	free(cmd);
}

// Free all commands
void	free_commands(t_command *commands)
{
	t_command	*current;
	t_command	*tmp;

	if (!commands)
		return ;
	current = commands;
	while (current)
	{
		tmp = current->next;
		free_command(current);
		current = tmp;
	}
}

// Count the number of commands
int	count_commands_parser(t_command *commands)
{
	int			count;
	t_command	*current;

	if (!commands)
		return (0);
	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

// Print commands for debugging
void	print_commands(t_command *commands)
{
	t_command	*cmd;
	int			cmd_idx;
	int			arg_idx;
	t_redirect	*redir;

	if (!commands)
	{
		printf("No commands\n");
		return ;
	}
	cmd = commands;
	cmd_idx = 0;
	while (cmd)
	{
		printf("Command %d:\n", cmd_idx);
		if (cmd->args)
		{
			printf("  Args: ");
			arg_idx = 0;
			while (cmd->args[arg_idx])
			{
				printf("[%s] ", cmd->args[arg_idx]);
				arg_idx++;
			}
			printf("\n");
		}
		if (cmd->redirects)
		{
			printf("  Redirections:\n");
			redir = cmd->redirects;
			while (redir)
			{
				printf("    Type: %d, File: %s\n", redir->type, redir->file);
				redir = redir->next;
			}
		}
		cmd = cmd->next;
		cmd_idx++;
	}
}
