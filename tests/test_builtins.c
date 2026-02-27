/* ************************************************************************** */
/*  test_builtins.c — unit tests for all minishell builtins                  */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <string.h>

/* =========================================================================
   Mini test framework
   ========================================================================= */

#define GRN "\033[32m"
#define RED "\033[31m"
#define BLD "\033[1m"
#define RST "\033[0m"

static int	g_pass = 0;
static int	g_fail = 0;

#define SECTION(name) printf(BLD "\n=== %s ===" RST "\n", (name))

#define TEST(label, cond) do { \
	if (cond) { \
		printf("  " GRN "[PASS]" RST " %s\n", (label)); \
		g_pass++; \
	} else { \
		printf("  " RED "[FAIL]" RST " %s  (line %d)\n", (label), __LINE__); \
		g_fail++; \
	} \
} while (0)

/* =========================================================================
   Helpers
   ========================================================================= */

/* Build a single env node (heap-allocated key and value). */
static t_env	*mk_node(char *key, char *val, t_env *next)
{
	t_env	*n;

	n = malloc(sizeof(t_env));
	n->key = ft_strdup(key);
	n->value = val ? ft_strdup(val) : NULL;
	n->next = next;
	return (n);
}

/* Free an entire env linked list. */
static void	free_list(t_env *h)
{
	t_env	*tmp;

	while (h)
	{
		tmp = h->next;
		free(h->key);
		free(h->value);
		free(h);
		h = tmp;
	}
}

/* Count number of nodes in list. */
static int	list_len(t_env *h)
{
	int	c;

	c = 0;
	while (h)
	{
		c++;
		h = h->next;
	}
	return (c);
}

/* =========================================================================
   Stdout capture
   Runs fn(arg) and stores everything printed to stdout into buf[bufsz].
   ========================================================================= */

typedef struct s_echo_arg	t_echo_arg;
struct s_echo_arg { char **args; };

typedef struct s_env_arg	t_env_arg;
struct s_env_arg { t_env *env; };

static void	run_echo(void *arg)
{
	t_echo_arg	*a;

	a = (t_echo_arg *)arg;
	echo(a->args);
}

static void	run_env(void *arg)
{
	t_env_arg	*a;

	a = (t_env_arg *)arg;
	env(a->env);
}

static void	capture(char *buf, size_t sz, void (*fn)(void *), void *arg)
{
	int		pfd[2];
	int		saved;
	ssize_t	n;

	if (pipe(pfd) == -1)
	{
		buf[0] = '\0';
		return ;
	}
	saved = dup(STDOUT_FILENO);
	dup2(pfd[1], STDOUT_FILENO);
	close(pfd[1]);
	fn(arg);
	fflush(stdout);
	dup2(saved, STDOUT_FILENO);
	close(saved);
	n = read(pfd[0], buf, sz - 1);
	close(pfd[0]);
	if (n < 0)
		n = 0;
	buf[n] = '\0';
}

/* =========================================================================
   TEST: valid_arg
   ========================================================================= */

static void	test_valid_arg(void)
{
	int	cut;

	SECTION("valid_arg");

	TEST("simple alpha key",            valid_arg("USER") > 0);
	TEST("underscore start",            valid_arg("_VAR") > 0);
	TEST("alphanumeric mixed",          valid_arg("VAR_123") > 0);
	TEST("key with value (KEY=val)",    valid_arg("USER=bob") > 0);
	TEST("underscore key with value",   valid_arg("_X=y") > 0);

	TEST("starts with digit",           valid_arg("1VAR") == FALSE);
	TEST("special char in key",         valid_arg("VA!R") == FALSE);
	TEST("starts with equals",          valid_arg("=bad") == FALSE);

	cut = valid_arg("KEY=value");
	TEST("cut points to '=' (idx 3)",   cut == 3);

	cut = valid_arg("MYVAR");
	TEST("no '=': cut equals length",   cut == 5);

	cut = valid_arg("A");
	TEST("single char key returns 1",   cut == 1);
}

/* =========================================================================
   TEST: check_key_exist
   ========================================================================= */

static void	test_check_key_exist(void)
{
	t_env	*list;

	SECTION("check_key_exist");

	list = mk_node("HOME", "/root",
		   mk_node("USER", "motaz",
		   mk_node("PATH", "/bin", NULL)));

	TEST("key exists (HOME)",  check_key_exist(list, "HOME") == TRUE);
	TEST("key exists (USER)",  check_key_exist(list, "USER") == TRUE);
	TEST("key exists (PATH)",  check_key_exist(list, "PATH") == TRUE);
	TEST("key missing",        check_key_exist(list, "SHELL") == FALSE);
	TEST("empty list",         check_key_exist(NULL, "HOME") == FALSE);

	free_list(list);
}

/* =========================================================================
   TEST: add_key_env
   ========================================================================= */

static void	test_add_key_env(void)
{
	t_env	*list;
	t_env	*node;
	char	*k;
	char	*v;

	SECTION("add_key_env");

	list = mk_node("HOME", "/root", NULL);

	/* add new key with value */
	k = ft_strdup("USER");
	v = ft_strdup("motaz");
	add_key_env(&list, k, v);
	TEST("new key added",         check_key_exist(list, "USER") == TRUE);
	node = find_node(list, "USER");
	TEST("new key value correct", node && ft_strcmp(node->value, "motaz") == 0);

	/* add new key WITHOUT value (export VAR) */
	k = ft_strdup("NOVALUE");
	add_key_env(&list, k, NULL);
	node = find_node(list, "NOVALUE");
	TEST("key with NULL value added",  node != NULL);
	TEST("NULL value stays NULL",      node && node->value == NULL);

	/* update existing key's value */
	k = ft_strdup("HOME");
	v = ft_strdup("/home/motaz");
	add_key_env(&list, k, v);
	node = find_node(list, "HOME");
	TEST("existing key updated",       node && ft_strcmp(node->value, "/home/motaz") == 0);

	/* export VAR (no =) on existing key — old value must NOT change */
	k = ft_strdup("USER");
	add_key_env(&list, k, NULL);
	node = find_node(list, "USER");
	TEST("export VAR keeps old value", node && ft_strcmp(node->value, "motaz") == 0);

	/* list must grow only for new keys */
	TEST("list has 3 nodes", list_len(list) == 3);

	free_list(list);
}

/* =========================================================================
   TEST: find_node
   ========================================================================= */

static void	test_find_node(void)
{
	t_env	*list;
	t_env	*node;

	SECTION("find_node");

	list = mk_node("A", "1",
		   mk_node("B", "2",
		   mk_node("C", "3", NULL)));

	TEST("find head node",   find_node(list, "A") != NULL);
	TEST("find middle node", find_node(list, "B") != NULL);
	TEST("find tail node",   find_node(list, "C") != NULL);
	TEST("not found",        find_node(list, "Z") == NULL);
	TEST("NULL list",        find_node(NULL, "A") == NULL);

	node = find_node(list, "B");
	TEST("value is correct", node && ft_strcmp(node->value, "2") == 0);

	free_list(list);
}

/* =========================================================================
   TEST: update_env_unset
   ========================================================================= */

static void	test_update_env_unset(void)
{
	t_env	*list;

	SECTION("update_env_unset");

	/* remove head */
	list = mk_node("A", "1",
		   mk_node("B", "2",
		   mk_node("C", "3", NULL)));
	update_env_unset(&list, "A");
	TEST("head removed",    find_node(list, "A") == NULL);
	TEST("B still exists",  find_node(list, "B") != NULL);
	TEST("C still exists",  find_node(list, "C") != NULL);
	free_list(list);

	/* remove middle */
	list = mk_node("A", "1",
		   mk_node("B", "2",
		   mk_node("C", "3", NULL)));
	update_env_unset(&list, "B");
	TEST("middle removed",  find_node(list, "B") == NULL);
	TEST("A still intact",  find_node(list, "A") != NULL);
	TEST("C still intact",  find_node(list, "C") != NULL);
	free_list(list);

	/* remove tail */
	list = mk_node("A", "1",
		   mk_node("B", "2",
		   mk_node("C", "3", NULL)));
	update_env_unset(&list, "C");
	TEST("tail removed",    find_node(list, "C") == NULL);
	TEST("A+B intact",      find_node(list, "A") != NULL && find_node(list, "B") != NULL);
	free_list(list);

	/* remove non-existent — no crash */
	list = mk_node("A", "1", NULL);
	update_env_unset(&list, "X");
	TEST("non-existent: no crash", find_node(list, "A") != NULL);
	free_list(list);

	/* remove from single-node list */
	list = mk_node("ONLY", "1", NULL);
	update_env_unset(&list, "ONLY");
	TEST("single-node: list is NULL", list == NULL);
}

/* =========================================================================
   TEST: merge_sort
   ========================================================================= */

static void	test_merge_sort(void)
{
	t_env	*list;
	t_env	*empty;

	SECTION("merge_sort");

	/* already sorted */
	list = mk_node("APPLE", "1",
		   mk_node("BANANA", "2",
		   mk_node("CHERRY", "3", NULL)));
	merge_sort(&list);
	TEST("sorted: head is APPLE",   ft_strcmp(list->key, "APPLE") == 0);
	TEST("sorted: 2nd is BANANA",   ft_strcmp(list->next->key, "BANANA") == 0);
	TEST("sorted: tail is CHERRY",  ft_strcmp(list->next->next->key, "CHERRY") == 0);
	free_list(list);

	/* reverse order */
	list = mk_node("ZEBRA", "3",
		   mk_node("MANGO", "2",
		   mk_node("APPLE", "1", NULL)));
	merge_sort(&list);
	TEST("reverse: head is APPLE",  ft_strcmp(list->key, "APPLE") == 0);
	TEST("reverse: mid is MANGO",   ft_strcmp(list->next->key, "MANGO") == 0);
	TEST("reverse: tail is ZEBRA",  ft_strcmp(list->next->next->key, "ZEBRA") == 0);
	free_list(list);

	/* single node */
	list = mk_node("SOLO", "1", NULL);
	merge_sort(&list);
	TEST("single node: intact",     ft_strcmp(list->key, "SOLO") == 0);
	free_list(list);

	/* NULL list */
	empty = NULL;
	merge_sort(&empty);
	TEST("NULL list: no crash",     empty == NULL);

	/* lower < upper in ASCII ('a' > 'A') */
	list = mk_node("cat", "1",
		   mk_node("CAT", "2", NULL));
	merge_sort(&list);
	TEST("ASCII: 'CAT' < 'cat'",    ft_strcmp(list->key, "CAT") == 0);
	free_list(list);
}

/* =========================================================================
   TEST: echo
   ========================================================================= */

static void	test_echo(void)
{
	char		buf[512];
	t_echo_arg	a;

	SECTION("echo");

	char *args1[] = {"echo", "hello", "world", NULL};
	a.args = args1;
	capture(buf, sizeof(buf), run_echo, &a);
	TEST("echo hello world",            strcmp(buf, "hello world\n") == 0);

	char *args2[] = {"echo", "-n", "hello", NULL};
	a.args = args2;
	capture(buf, sizeof(buf), run_echo, &a);
	TEST("echo -n suppresses newline",  strcmp(buf, "hello") == 0);

	char *args3[] = {"echo", NULL};
	a.args = args3;
	capture(buf, sizeof(buf), run_echo, &a);
	TEST("echo alone prints newline",   strcmp(buf, "\n") == 0);

	char *args4[] = {"echo", "-n", "-n", "test", NULL};
	a.args = args4;
	capture(buf, sizeof(buf), run_echo, &a);
	TEST("echo -n -n consumes both",    strcmp(buf, "test") == 0);

	char *args5[] = {"echo", "-nnn", "hi", NULL};
	a.args = args5;
	capture(buf, sizeof(buf), run_echo, &a);
	TEST("echo -nnn is valid -n flag",  strcmp(buf, "hi") == 0);

	char *args6[] = {"echo", "-n", NULL};
	a.args = args6;
	capture(buf, sizeof(buf), run_echo, &a);
	TEST("echo -n alone: no output",    strcmp(buf, "") == 0);
}

/* =========================================================================
   TEST: env
   ========================================================================= */

static void	test_env(void)
{
	char		buf[1024];
	t_env_arg	a;
	t_env		*list;

	SECTION("env");

	list = mk_node("USER", "motaz",
		   mk_node("HOME", "/root", NULL));
	a.env = list;
	capture(buf, sizeof(buf), run_env, &a);
	TEST("prints USER=motaz",        strstr(buf, "USER=motaz\n") != NULL);
	TEST("prints HOME=/root",        strstr(buf, "HOME=/root\n") != NULL);
	free_list(list);

	/* var with NULL value must NOT be printed */
	list = mk_node("NOVAL", NULL,
		   mk_node("KEY", "val", NULL));
	a.env = list;
	capture(buf, sizeof(buf), run_env, &a);
	TEST("skips var with NULL value",  strstr(buf, "NOVAL") == NULL);
	TEST("still prints KEY=val",       strstr(buf, "KEY=val\n") != NULL);
	free_list(list);

	/* empty list */
	a.env = NULL;
	capture(buf, sizeof(buf), run_env, &a);
	TEST("NULL env: no output",        strcmp(buf, "") == 0);
}

/* =========================================================================
   TEST: update_env_unset (via unset builtin)
   ========================================================================= */

static void	test_unset_builtin(void)
{
	SECTION("unset builtin");

	t_shell	shell;
	shell.env_list = mk_node("A", "1",
				 mk_node("B", "2",
				 mk_node("C", "3", NULL)));
	shell.last_exit_status = 0;

	char *args1[] = {"unset", "B", NULL};
	TEST("unset returns TRUE",   unset(args1, &shell) == TRUE);
	TEST("B removed",            find_node(shell.env_list, "B") == NULL);
	TEST("A still present",      find_node(shell.env_list, "A") != NULL);

	char *args2[] = {"unset", "A", "C", NULL};
	unset(args2, &shell);
	TEST("unset multiple: A+C gone", shell.env_list == NULL);

	/* unset on empty list — no crash */
	char *args3[] = {"unset", "X", NULL};
	TEST("unset on empty: no crash", unset(args3, &shell) == TRUE);
}

/* =========================================================================
   main
   ========================================================================= */

int	main(void)
{
	printf(BLD "Running minishell builtin unit tests...\n" RST);

	test_valid_arg();
	test_check_key_exist();
	test_add_key_env();
	test_find_node();
	test_update_env_unset();
	test_merge_sort();
	test_echo();
	test_env();
	test_unset_builtin();

	printf("\n" BLD "Results: " RST
		GRN "%d passed" RST ", "
		RED "%d failed" RST "\n",
		g_pass, g_fail);

	return (g_fail > 0 ? 1 : 0);
}
