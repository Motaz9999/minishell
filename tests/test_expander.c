/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 20:20:00 by moodeh            #+#    #+#             */
/*   Updated: 2026/03/23 20:20:00 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

#define GRN "\033[32m"
#define RED "\033[31m"
#define BLD "\033[1m"
#define RST "\033[0m"

#define TEST(label, cond) do { \
	if (cond) { \
		printf("  " GRN "[PASS]" RST " %s\n", (label)); \
		g_pass++; \
	} else { \
		printf("  " RED "[FAIL]" RST " %s  (line %d)\n", (label), __LINE__); \
		g_fail++; \
	} \
} while (0)

static int	g_pass = 0;
static int	g_fail = 0;

char	*extract_key_value(char *key, t_env *env_list, int last_exit_status);
int		check_valid_key_char(char c);
char	*expand_cmd(char *expand_it, t_shell *shell);

/* expander depends on find_node(), so provide a tiny test implementation. */
t_env	*find_node(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

static t_env	*mk_node(char *key, char *val, t_env *next)
{
	t_env	*n;

	n = malloc(sizeof(t_env));
	if (!n)
		return (NULL);
	n->key = ft_strdup(key);
	if (!n->key)
		return (free(n), NULL);
	n->value = NULL;
	if (val)
	{
		n->value = ft_strdup(val);
		if (!n->value)
			return (free(n->key), free(n), NULL);
	}
	n->next = next;
	return (n);
}

static void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

static void	test_check_valid_key_char(void)
{
	printf(BLD "\n=== check_valid_key_char ===" RST "\n");
	TEST("alpha", check_valid_key_char('A') == TRUE);
	TEST("digit", check_valid_key_char('7') == TRUE);
	TEST("underscore", check_valid_key_char('_') == TRUE);
	TEST("dash invalid", check_valid_key_char('-') == FALSE);
	TEST("space invalid", check_valid_key_char(' ') == FALSE);
	TEST("question invalid", check_valid_key_char('?') == FALSE);
}

static void	test_extract_key_value(t_shell *shell)
{
	char	*value;

	printf(BLD "\n=== extract_key_value ===" RST "\n");
	value = extract_key_value("HOME", shell->env_list, shell->last_exit_status);
	TEST("HOME found", value && ft_strcmp(value, "/home/fake") == 0);
	free(value);
	value = extract_key_value("NOT_SET", shell->env_list,
			shell->last_exit_status);
	TEST("missing key returns empty", value && ft_strcmp(value, "") == 0);
	free(value);
	value = extract_key_value("?", shell->env_list, shell->last_exit_status);
	TEST("special $? value", value && ft_strcmp(value, "42") == 0);
	free(value);
}

static void	test_expand_cmd(t_shell *shell)
{
	char	*s;

	printf(BLD "\n=== expand_cmd ===" RST "\n");
	s = ft_strdup("$HOME");
	s = expand_cmd(s, shell);
	TEST("$HOME", s && ft_strcmp(s, "/home/fake") == 0);
	free(s);
	s = ft_strdup("hello$USER");
	s = expand_cmd(s, shell);
	TEST("prefix + $USER", s && ft_strcmp(s, "hellomotaz") == 0);
	free(s);
	s = ft_strdup("$USER-$HOME-$?");
	s = expand_cmd(s, shell);
	TEST("multi expansion", s && ft_strcmp(s, "motaz-/home/fake-42") == 0);
	free(s);
	s = ft_strdup("$NOT_SET");
	s = expand_cmd(s, shell);
	TEST("missing key in command", s && ft_strcmp(s, "") == 0);
	free(s);
	s = ft_strdup("abc$");
	s = expand_cmd(s, shell);
	TEST("trailing dollar stays", s && ft_strcmp(s, "abc$") == 0);
	free(s);
}

int	main(void)
{
	t_shell	shell;
	t_env		*env;

	env = NULL;
	env = mk_node("HOME", "/home/fake", env);
	env = mk_node("USER", "motaz", env);
	if (!env)
		return (1);
	shell.env_list = env;
	shell.last_exit_status = 42;
	test_check_valid_key_char();
	test_extract_key_value(&shell);
	test_expand_cmd(&shell);
	printf(BLD "\nSummary:" RST " %d pass, %d fail\n", g_pass, g_fail);
	free_env(env);
	return (g_fail != 0);
}
