*This project has been created as part of the 42 curriculum by moodeh & ammr.*

# minishell

## Description

minishell is a minimal UNIX shell written in C, built as part of the 42 school curriculum. The goal of the project is to reproduce the core behaviour of bash: reading a command line, parsing it into tokens, expanding variables, and executing commands — with support for pipes, redirections, heredocs, builtins, signals, and environment management.

The project is split between two responsibilities:

- **Parser side** — lexer, token expansion, heredoc read, command-list builder.
- **Executor side** — process forking, pipe wiring, redirections, builtin dispatch, environment handling, signal management, and exit-code propagation.

### Supported features

| Feature | Details |
|---|---|
| Builtins | `echo` (`-n`), `cd`, `pwd`, `env`, `export`, `unset`, `exit` |
| Variable expansion | `$VAR`, `$?`, inside `"double quotes"`, suppressed inside `'single quotes'` |
| Pipes | `cmd1 \| cmd2 \| cmd3 …` — unlimited depth |
| Redirections | `>` truncate, `>>` append, `<` input, `<<` heredoc |
| Signals | `Ctrl-C` (new prompt), `Ctrl-D` (exit), `Ctrl-\` ignored in parent |
| Exit codes | Correct propagation: `0` success, `1` error, `127` not found, `128+N` signal |
| Environment | Full `envp` init, `export`/`unset` update, passed correctly to child processes |

---

## Instructions

### Requirements

- `gcc` with `-Wall -Wextra -Werror`
- GNU `readline` library (`libreadline-dev` on Debian/Ubuntu)

```sh
sudo apt install libreadline-dev
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

The shell starts an interactive prompt `minishell$ `. Type any command and press Enter. Press `Ctrl-D` or type `exit` to quit.

### Running the test suite

```sh
bash tests/ultimate_test.sh
```

Results are saved to `tests/ultimate_results.txt`.

---

## Resources

### Documentation & references

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — authoritative reference for shell behaviour
- [POSIX Shell & Utilities](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) — standard specification
- [`man 3 readline`](https://tiswww.case.edu/php/chet/readline/rltop.html) — GNU readline library
- [`man 2 execve`](https://man7.org/linux/man-pages/man2/execve.2.html) — process execution
- [`man 2 fork`](https://man7.org/linux/man-pages/man2/fork.2.html) — process creation
- [`man 2 pipe`](https://man7.org/linux/man-pages/man2/pipe.2.html) — inter-process communication
- [`man 2 dup2`](https://man7.org/linux/man-pages/man2/dup2.2.html) — file descriptor redirection
- [`man 2 waitpid`](https://man7.org/linux/man-pages/man2/waitpid.2.html) — waiting for child processes
- [`man 2 sigaction`](https://man7.org/linux/man-pages/man2/sigaction.2.html) — signal handling
- [Writing a Shell in C — Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/) — introductory walkthrough

### Use of AI
- **Test scripts** — writing `tests/ultimate_test.sh` to systematically verify all executor features.
