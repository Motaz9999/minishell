# Minishell Fix Todo

## High Priority

- [ ] Heredoc: check delimiter before expansion.
- [ ] Heredoc: preserve empty input lines as newline.
- [ ] Heredoc: fix `expand_cmd` ownership and free paths.
- [ ] Heredoc: handle fork failure and close both pipe ends.
- [ ] Heredoc: close remaining fds on signal/error return.
- [ ] Heredoc: skip expansion when delimiter is quoted.

## Medium Priority

- [ ] Heredoc child: close unused read end after fork.
- [ ] Expander: define empty-string contract for `expand_cmd`.
- [ ] Signals: set explicit `SIGQUIT` policy for heredoc mode.

## Low Priority

- [ ] Cleanup: remove duplicate env free between `free_shell` and `exit` cleanup path.
