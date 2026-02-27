/* ************************************************************************** */
/*  stubs.c — minimal stubs for functions not yet implemented                 */
/*  Only used by the test binary; never linked into the real minishell.       */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <string.h>

/* Define the global signal flag (normally lives in signal_handle.c) */
volatile sig_atomic_t	g_sigint_received = 0;

/* ---------- export helpers ---------- */

/* Extract the key from "KEY" or "KEY=value" (chars 0..cut-1) */
char	*cut_key(char *arg, int cut)
{
	return (ft_substr(arg, 0, cut));
}

/* Extract the value from "KEY=value" (chars after '=').
   Returns NULL if there is no '=' at position `cut`. */
char	*cut_value(char *arg, int cut)
{
	if (arg[cut] != '=')
		return (NULL);
	return (ft_strdup(arg + cut + 1));
}

/* ---------- error helpers ---------- */

int	error_cmd(char *cmd, char *msg, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (exit_code);
}

int	error_syscall(char *context, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(context, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (exit_code);
}
