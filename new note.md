# minishell fixes note

Date: 2026-03-30

This note records what was fixed for failing tests around command resolution,
path execution, and empty-variable expansion.

## 1) Path commands were skipped in resolve_path

File: src/executor/path_handle.c
Lines: 131-141

What was wrong:
- Path-like commands such as /bin/ls, ./prog, ../prog, and expanded values like $PWD
	were detected by is_path, then resolve_path returned immediately.
- That skipped all path validations and never set find_path.

Why this broke tests:
- execute_one_cmd returned -1 when find_path stayed NULL.
- No child was forked, so no execution error was printed.
- Exit code often remained old value (commonly 0), causing mismatch with bash.

What was fixed:
- When is_path(cmd_name) is true, resolve_path now calls handle_path_cmd first,
	then returns.

Result:
- Path commands now get proper checks and proper bash-like status/errors.

## 2) access checks were inverted for path execution

File: src/executor/path_handle.c
Lines: 94 and 103

What was wrong:
- Code compared access(...) against TRUE.
- access returns 0 on success, not 1.

Why this broke tests:
- Existing directory/file paths could be treated as missing.
- Example symptom: running $PWD printed No such file or directory instead of
	Is a directory.

What was fixed:
- Changed checks to compare with 0:
	- access(cmd_name, F_OK) != 0
	- access(cmd_name, X_OK) != 0

Result:
- Correct distinction between existing path, directory, and permission errors.

## 3) Empty unquoted expansion blocked valid commands

File: src/expander/expander.c
Lines: 84-132

What was wrong:
- Expansion could produce empty string arguments from unquoted vars.
- For input like: $EMPTY echo hi
	args became ["", "echo", "hi"].
- execute_one_cmd rejects empty args[0], so command was skipped.

What was fixed:
- Added remove_empty_unquoted_args.
- After expansion, unquoted empty args are removed by shifting the argv arrays.
- Called this cleanup at end of expand_args_from_cmd.

Result:
- $EMPTY echo hi now behaves like bash and runs echo hi.

## 4) Behavior after fixes for reported tests

- Test 133 ($PWD): now returns Is a directory, exit 126.
- Test 135 ($EMPTY echo hi): now outputs hi, exit 0.
- Test 136 (./test_files/invalid_permission): now can reach proper path checks
	instead of silently skipping.
- Test 137 (./missing.out): now returns No such file or directory, exit 127.
- Test 141 (./test_files): now returns Is a directory, exit 126.
- Test 142 (/test_files): now returns No such file or directory, exit 127.

## 5) Key lesson

In command execution, small control-flow shortcuts before validation are risky.
For shell behavior compatibility, path-like commands must always pass through
the same validation pipeline that sets both message and exit status.

## 6) Current must-fix list (2026-04-11)

These are still pending and should be fixed next.

### A) Double prompt after Ctrl+C on foreground command

Files:
- src/signals/signal_handle.c
- src/main.c
- src/executor/executor.c

Symptom:
- Running a foreground command like cat, pressing Ctrl+C can show:
	minishell$ minishell$

Cause:
- Parent SIGINT handler always calls readline redraw functions.
- Parent keeps this handler while waiting for children.

Fix direction:
- Use separate signal behavior for prompt mode vs execution/wait mode.
- During wait, do not redraw readline prompt from signal handler.
- Restore prompt-mode handler after wait loop.

### B) Heredoc with quoted delimiter expands variables (should not)

Files:
- src/parsing/parser_redir.c
- src/parsing/lexer_word.c

Symptom:
- cat << 'EOF' then input $X prints expanded value (wrong).

Expected:
- With quoted delimiter, heredoc body must not expand variables.

Cause:
- fill_heredoc always uses expand_cmd on every line.
- Delimiter quote information is not used to control expansion.

Fix direction:
- Carry delimiter quote state from token into heredoc redirection node.
- In fill_heredoc, skip expand_cmd when delimiter was quoted.

### C) Ctrl+C during heredoc should abort command with proper status

Files:
- src/parsing/parser_redir.c
- src/signals/signal_handle.c

Symptom:
- Ctrl+C in heredoc path may not stop command flow cleanly with bash-like status.

Expected:
- Heredoc is canceled, command should not execute, status should be 130.

Fix direction:
- In heredoc loop, detect SIGINT flag and stop immediately.
- Close both heredoc pipe ends on cancel path.
- Return failure to parser so command does not execute.
- Set shell->last_exit_status to 130.

## 7) Remaining issues after latest checks (2026-04-13)

### A) Single-quoted dollar is still lost

Files:
- src/parsing/lexer_quote.c

Symptom:
- echo '$HOME' prints HOME instead of $HOME.
- echo '$?' prints ? instead of $?.

Cause:
- read_single_quoted replaces '$' with byte 1 in protect_dollars.
- No restore path was found before output/execution.

Fix direction:
- Keep '$' literal in single-quoted text (preferred).
- Or restore protected byte back to '$' before expansion/output stages.

### B) waitpid return is unchecked in executor wait loop

Files:
- src/executor/executor.c

Symptom:
- waiting_loop_free_pids calls waitpid but ignores its return value.

Risk:
- If waitpid fails/interrupted, status handling can become unreliable.

Fix direction:
- Check waitpid return value.
- Retry on EINTR, handle other errors safely, and avoid using undefined status.

### C) Redundant env cleanup in heredoc child

Files:
- src/parsing/heredoc_handle.c
- src/utils/free_shell.c

Symptom:
- Heredoc child calls free_shell(shell), then free_env_list(shell->env_list) again.

Risk:
- Currently mostly harmless because pointer is nulled, but redundant and fragile.

Fix direction:
- Remove duplicate free_env_list call in heredoc child cleanup.

### D) Optional compatibility mismatch: command-not-found text casing

Files:
- src/executor/error_handle.c

Symptom:
- Output is "command not found" while some tester expects "Command not found".

Fix direction:
- If matching that tester is required, change exact message casing.