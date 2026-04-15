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

## 6) Active Priority TODO Board (2026-04-15)

Promotion rule:
- Only OPEN items stay in this section.
- If current P1 is solved, the next OPEN item is promoted automatically.

Status legend:
- OPEN = not fixed yet
- IN PROGRESS = partially fixed, needs final pass
- SOLVED = moved to section 7
- SKIPPED = deferred by request

### P1) Unsupported operators are parsed as plain text (SKIPPED, Medium)

Reason:
- Deferred by request on 2026-04-15.

Current behavior:
- && and || are treated as normal words.
- Example: echo hi && echo there prints literal && echo there.

Expected behavior:
- Since this minishell does not implement &&/||, return syntax error instead.

Root cause:
- Lexer only recognizes |, <, > as operators.

Files to update:
- src/parsing/lexer_utils.c
- src/parsing/lexer.c
- src/parsing/syntax_checker.c

Fix:
- Detect unsupported operators (&&, ||, ;, optionally parentheses) and reject.
- Emit consistent syntax error + status 2.

Validation after fix:
- echo hi && echo there -> syntax error.
- echo hi || echo there -> syntax error.

### P2) Double prompt after Ctrl+C on foreground job (OPEN, Medium)

Current behavior:
- Foreground command interrupted by Ctrl+C can print duplicated prompt line.

Root cause:
- Parent SIGINT handler redraws readline prompt while shell is waiting on child.

Files to update:
- src/signals/signal_handle.c
- src/executor/executor.c
- src/main.c

Fix:
- Use wait-mode signal handler without readline redraw.
- Restore interactive prompt handler after wait finishes.
- Gate readline redraw to prompt mode only.

Validation after fix:
- Run cat, press Ctrl+C once, exactly one clean prompt appears.

### P3) Escaped quotes in double quotes are not handled (OPEN, Low)

Current behavior:
- Backslash-escaped quote inside double quotes is not parsed bash-like.

Expected behavior:
- \" inside double quotes should be treated as literal ".

Files to update:
- src/parsing/lexer_quote.c

Fix:
- Implement escape-aware scan for double-quoted content.
- Respect shell rules for escapes inside double quotes.

Validation after fix:
- echo "a\"b" outputs a"b.

### P4) Placeholder quote file should be implemented or removed (OPEN, Low)

Current behavior:
- src/parsing/handle_quote.c exists as a placeholder with no real logic.
- It can cause confusion about the real quote-handling path.

Expected behavior:
- Quote handling code should live in one clear place with no dead placeholder file.

Root cause:
- Partial refactor started but not completed.

Files to update:
- src/parsing/handle_quote.c
- Makefile

Fix:
- Either implement and wire this file into parsing flow, or remove it.
- Keep one authoritative quote pipeline to avoid duplicated/unused logic.

Validation after fix:
- Build succeeds with no unused/dead parser file confusion.
- Quote behavior tests still pass.

### P5) Optional tester compatibility: message casing (OPEN, Low)

Current behavior:
- Some external testers expect "Command not found" exact casing.

Files to update:
- src/executor/error_handle.c

Fix:
- Only if required by your chosen tester, align exact message text casing.

Validation after fix:
- Re-run that tester case and compare exact stderr string.

## 7) Solved Archive (updated 2026-04-15)

### SOLVED) Quoted heredoc delimiter expansion control

Files updated:
- src/parsing/heredoc_handle.c

Result:
- cat << '' now terminates on the first empty line (correct empty delimiter match).
- cat << 'EOF' keeps $VAR literal in heredoc body.
- Non-empty delimiters still behave correctly.

### SOLVED) Single-quoted dollar preservation

Files updated:
- src/expander/expander.c

Result:
- echo '$HOME' and echo '$?' remain literal.
- Mixed tokens now behave correctly: echo 'abc'$HOME expands only unquoted part.

### SOLVED) Redundant env cleanup in heredoc child

Result:
- Duplicate env cleanup path is no longer present in current heredoc child code.

### SOLVED) Ctrl+C during heredoc keeps status 130

Files updated:
- src/parsing/parser_and_execute.c

Result:
- Parser failure path now preserves status 130 instead of overwriting it with 2.
- Manual check in minishell session: start heredoc, send Ctrl+C, then echo $? prints 130.

### SOLVED) Unmatched quote reports syntax error

Files updated:
- src/parsing/lexer_quote.c

Result:
- Unmatched single quote and unmatched double quote now print:
	- minishell: syntax error near unexpected token `newline'
- Status is preserved as 2 after the failed parse.

### SOLVED) Whitespace-only input preserves previous status

Files updated:
- src/parsing/parser_and_execute.c

Result:
- EOF-only token streams now return early without forcing status 2.
- Validation: `false`, then whitespace-only line, then `echo $?` prints `1`.

### SOLVED) Heredoc key with single-quoted '$' matches correctly

Files updated:
- src/parsing/parser_redir.c

Result:
- Heredoc delimiter now normalizes protected dollar markers before key storage.
- Validation:
	- `cat << '$END'` stops on `$END`.
	- `cat << "$END"` stops on `$END`.
	- Existing behavior preserved for `<< EOF` (expands body) and `<< 'EOF'` (no expansion).

### SOLVED) waitpid error handling in executor wait loop

Files updated:
- src/executor/executor.c

Result:
- Added EINTR-safe wait loop helper and explicit waitpid failure handling.
- Last-command status extraction remains correct after successful waits.

### SOLVED) Heredoc EOF before delimiter warning

Files updated:
- src/parsing/heredoc_handle.c

Result:
- EOF (Ctrl+D) before delimiter now prints:
	- minishell: warning: here-document at line 1 delimited by end-of-file (wanted `DELIM')
- Normal delimiter-terminated heredoc flow remains unchanged.