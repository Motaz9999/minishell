*This project has been created as part of the 42 curriculum by moodeh & aamr.*

# LazyShell

![Minishell Preview](readme%20files/Minishell%20Pipeline%20Architecture.png)

## Description

minishell is a minimal UNIX shell written in C, built as part of the 42 school curriculum. The goal of the project is to reproduce core bash-like behavior: reading a command line, tokenizing/parsing it, expanding variables, and executing commands with pipes, redirections, heredocs, builtins, signals, and environment management.

Visually and functionally, the project bridges the gap between the commands a user types and the complex system calls required to parse, route, and execute those commands flawlessly under the UNIX philosophy.

### 📸 Preview

<img src="readme files/preview1.png" alt="Minishell Terminal Preview 1" />
<img src="readme files/preview2.png" alt="Minishell Terminal Preview 2" />

## Architecture & Diagrams

To help readers understand the inner workings of our shell, we've broken down the architecture into visual representations:

### 1. Overall Pipeline Architecture
The shell utilizes a clear separation of concerns, taking a string of characters and piping it through lexical analysis, parsing, tree building, and finally to the executor.
![Pipeline Architecture](readme%20files/Minishell%20Pipeline%20Architecture.png)

### 2. Execution Flow
From evaluating builtins in the parent process versus launching robust child processes inside pipelines, this is the core loop taking place repeatedly:
![Execution Flow](readme%20files/Execution%20Flow.png)

### 3. Data Structures
Memory mapping and interconnected nodes are crucial for keeping track of environments (`envp`), token clusters, and AST components.
![Data Structures](readme%20files/Minishell%20Data%20Structures.png)

### 4. Pipe and Fork Model
UNIX `pipe()` and `fork()` concepts heavily dictate how commands pass output to the next command's input without breaking the main shell loop.
![Pipe and Fork Model](readme%20files/Pipe%20and%20Fork%20Model.png)

### 5. Heredoc End-To-End Trace
Heredocs (`<<`) act as a separate process interaction intercepting standard input securely until a chosen delimiter is hit, managing its own file descriptors.
![Heredoc End-To-End Trace](readme%20files/Heredoc%20End-To-End%20Trace.png)

---

The project is split between two responsibilities:

- **Parser side**: lexer, syntax checks, heredoc preparation, command-list builder.
- **Executor side**: process forking, pipe wiring, redirections, builtin dispatch, environment handling, signal management, and exit-code propagation.

### Supported features

| Feature | Details |
|---|---|
| Builtins | `echo` (`-n`), `cd`, `pwd`, `env`, `export`, `unset`, `exit` |
| Variable expansion | `$VAR`, `$?`, expansion in unquoted and `"double quoted"` text; suppressed in `'single quotes'` |
| Pipes | `cmd1 \| cmd2 \| cmd3 ...` (arbitrary pipeline length) |
| Redirections | `>` truncate, `>>` append, `<` input, `<<` heredoc |
| Heredoc behavior | Delimiter-aware heredoc reader in child process; expands variables only when delimiter is unquoted |
| Signals | Prompt/execution signal-state handling for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` |
| Exit codes | Standard shell semantics (`0`, `1`, `126`, `127`, `128+signal`) with last status in `$?` |
| Environment | `envp` linked-list init, export/update/unset, and conversion to `char **envp` for `execve` |
| Prompt/UI | Colored readline prompt (`LAZYSHELL$`) with interactive startup banner on TTY |

### Project structure

| Module | Role | Key files |
|---|---|---|
| Entry / shell loop | Startup, readline loop, global shell state lifecycle | `src/main.c`, `src/utils/free_shell.c` |
| Parsing | Lexing, quote-aware word reads, syntax checks, AST-like command chain | `src/parsing/lexer*.c`, `src/parsing/parser*.c`, `src/parsing/syntax_checker.c` |
| Expander | `$VAR` and `$?` replacement with quote-aware cleanup | `src/expander/expander.c`, `src/expander/replace_str.c` |
| Executor | Fork/exec, builtin dispatch, pipes, wait loop, status propagation | `src/executor/executor*.c`, `src/executor/path_handle.c` |
| Redirections / heredoc | `<`, `>`, `>>`, `<<` handling and fd wiring | `src/executor/redirections_handle.c`, `src/parsing/heredoc_handle*.c` |
| Builtins | Core builtin behavior and env updates | `src/builtin/*.c` |
| Signals | Parent/child/heredoc signal setup and handlers | `src/signals/signal_handle*.c` |
| Environment | Linked-list env representation and utilities | `src/env_list/env_list.c`, `src/executor/envp_handle.c` |
| Shared contracts | Types, enums, function prototypes, prompt/color defines | `includes/struct.h`, `includes/minishell.h`, `includes/colors.h` |
| Libft | Custom utility library used by minishell | `libft/` |

### Execution flow

1. `readline()` reads user input in prompt mode.
2. Lexer produces tokens (`WORD`, `PIPE`, redirection tokens, `EOF`).
3. Syntax checker validates pipe/redirection placement.
4. Parser builds a linked list of commands (`t_command`) and redirections (`t_redirect`).
5. Heredocs are collected before execution; quoted delimiter controls expansion.
6. Expander resolves `$VAR` and `$?` across command args.
7. Executor runs either:
	- single builtin in parent (with temporary stdio save/restore), or
	- forked pipeline with redirections, `execve`, and wait loop.
8. Parent restores prompt signal mode and stores final exit status in `shell.last_exit_status`.

---

## Instructions

### Requirements

- `gcc` with `-Wall -Wextra -Werror`
- GNU `readline` development package (`libreadline-dev` on Debian/Ubuntu)
- `make`
- Optional for Python interactive tests: `python3` + `pexpect`

```sh
sudo apt install build-essential libreadline-dev python3 python3-pip
python3 -m pip install --user pexpect
```

### Compilation

```sh
make          # build ./minishell
make re       # clean rebuild
make clean    # remove object files
make fclean   # remove objects + binary
```

### Running

```sh
./minishell
```

The shell starts an interactive prompt (`LAZYSHELL$ `). Press `Ctrl-D` or run `exit` to quit.

### Running the test suite

Project tests:

```sh
bash tests/ultimate_test.sh
bash tests/run_executor_tests.sh
python3 tests/evalsheet_check.py > tests/evalsheet_check_results.txt
python3 tests/evalsheet_interactive.py > tests/evalsheet_interactive_results.txt
```

Generated outputs are saved in the `tests/` folder (for example `tests/ultimate_results.txt`, `tests/results_executor.txt`, and evalsheet result files).

Bundled external testers in this repository:

```sh
./minishell_tester/tester
bash 42_minishell_tester/tester.sh m
```

Useful subsets for `42_minishell_tester`:

```sh
bash 42_minishell_tester/tester.sh m builtins
bash 42_minishell_tester/tester.sh m redirections
bash 42_minishell_tester/tester.sh m pipelines
bash 42_minishell_tester/tester.sh m syntax
```

---

## Resources

### Documentation & references

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) - authoritative reference for shell behavior
- [POSIX Shell & Utilities](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) - standard specification
- [`man 3 readline`](https://tiswww.case.edu/php/chet/readline/rltop.html) - GNU readline library
- [`man 2 execve`](https://man7.org/linux/man-pages/man2/execve.2.html) - process execution
- [`man 2 fork`](https://man7.org/linux/man-pages/man2/fork.2.html) - process creation
- [`man 2 pipe`](https://man7.org/linux/man-pages/man2/pipe.2.html) - inter-process communication
- [`man 2 dup2`](https://man7.org/linux/man-pages/man2/dup2.2.html) - file descriptor redirection
- [`man 2 waitpid`](https://man7.org/linux/man-pages/man2/waitpid.2.html) - waiting for child processes
- [`man 2 sigaction`](https://man7.org/linux/man-pages/man2/sigaction.2.html) - signal handling
- [Writing a Shell in C - Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/) - introductory walkthrough

Project notes and diagrams are available graphically in the `readme files/` folder, which also includes the original `.jam` editable sources for the Jamboard/Figjam diagrams.

### Use of AI

- **Test scripts**: writing `tests/ultimate_test.sh` to systematically verify executor features.
