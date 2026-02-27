/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 00:00:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/27 09:32:15 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Writes one line into the heredoc pipe, expanding $VAR and $?
static void	write_heredoc_line(int fd, const char *line, t_shell *shell)
{
	char	buf[4096];
	int		i;
	int		j;
	char	*val;
	t_env	*node;
	char	varname[256];
	int		k;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1]
			&& (ft_isalpha(line[i + 1]) || line[i + 1] == '_'
				|| line[i + 1] == '?'))
		{
			i++;
			if (line[i] == '?')
			{
				val = ft_itoa(shell->last_exit_status);
				k = 0;
				while (val[k])
					buf[j++] = val[k++];
				free(val);
				i++;
			}
			else
			{
				k = 0;
				while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
					varname[k++] = line[i++];
				varname[k] = '\0';
				node = find_node(shell->env_list, varname);
				if (node && node->value)
				{
					k = 0;
					while (node->value[k])
						buf[j++] = node->value[k++];
				}
			}
		}
		else
			buf[j++] = line[i++];
	}
	buf[j++] = '\n';
	buf[j] = '\0';
	write(fd, buf, j);
}

// Creates a pipe, reads lines until delimiter, stores read-end in redir->heredoc_fd
// Returns TRUE on success, FALSE on error
int	fill_heredoc(t_redirect *redir, t_shell *shell)
{
	int		pipe_fds[2];
	char	*line;

	if (pipe(pipe_fds) == -1)
		return (error_syscall("pipe", FALSE));
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: heredoc delimited by EOF\n", 2);
			break ;
		}
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(pipe_fds[1], line, shell);
		free(line);
	}
	close(pipe_fds[1]);
	redir->heredoc_fd = pipe_fds[0];
	return (TRUE);
}
