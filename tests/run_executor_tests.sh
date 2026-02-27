#!/bin/bash
# Tests ONLY the executor's domain:
# builtins, pipes, redirections, exit codes, error messages, signals
# Each test sends ONE logical command (no semicolons) to avoid parser issues.

SHELL_BIN=./minishell
OUT=./tests/results_executor.txt
PASS=0
FAIL=0

SEP="────────────────────────────────────────"

run() {
    local label="$1"
    local input="$2"
    local expected="$3"

    actual=$(printf '%s\nexit\n' "$input" | $SHELL_BIN 2>&1 | grep -v '^minishell\$' | grep -v '^exit$')

    printf "\n%s\n" "$SEP" >> "$OUT"
    printf "TEST : %s\n"   "$label"    >> "$OUT"
    printf "INPUT: %s\n"   "$input"    >> "$OUT"
    printf "GOT  : %s\n"   "$actual"   >> "$OUT"
    printf "WANT : %s\n"   "$expected" >> "$OUT"

    if [ "$actual" = "$expected" ]; then
        printf "STATUS: PASS ✓\n" >> "$OUT"
        ((PASS++))
    else
        printf "STATUS: FAIL ✗\n" >> "$OUT"
        ((FAIL++))
    fi
}

# exit code helper: run cmd, then "echo $?" in next shell invocation
run_exit() {
    local label="$1"
    local input="$2"
    local expected_code="$3"

    code=$(printf '%s\n' "exit\n" | $SHELL_BIN 2>&1; echo $?)
    # Better: run cmd and check exit code of minishell itself
    printf '%s\nexit %s\n' "$input" "" | $SHELL_BIN > /dev/null 2>&1
    # Actually pipe exit $? echo approach
    actual=$(printf '%s\necho $?\nexit\n' "$input" | $SHELL_BIN 2>&1 | grep -v '^minishell\$' | grep -v '^exit$' | tail -1)

    printf "\n%s\n" "$SEP" >> "$OUT"
    printf "TEST : %s\n"   "$label"          >> "$OUT"
    printf "INPUT: %s\n"   "$input"          >> "$OUT"
    printf "GOT \$?: %s\n" "$actual"         >> "$OUT"
    printf "WANT\$?: %s\n" "$expected_code"  >> "$OUT"

    if [ "$actual" = "$expected_code" ]; then
        printf "STATUS: PASS ✓\n" >> "$OUT"
        ((PASS++))
    else
        printf "STATUS: FAIL ✗\n" >> "$OUT"
        ((FAIL++))
    fi
}

> "$OUT"
printf "EXECUTOR / BUILTINS TEST RESULTS — $(date)\n" >> "$OUT"

# ── ECHO ──────────────────────────────────────────────────────────────────────
run  "echo hello"           "echo hello"              "hello"
run  "echo -n"              "echo -n hello"           "hello"
run  "echo multi args"      "echo one two   three"    "one two   three"
run  "echo empty"           "echo"                    ""
run  "echo dquotes"         'echo "hello world"'      "hello world"
run  "echo squotes"         "echo 'hello world'"      "hello world"
run  "echo expand HOME"     'echo $HOME'              "$HOME"
run  "echo no expand ''"    "echo '\$HOME'"            '$HOME'
run  "echo expand \"\""     'echo "$HOME"'            "$HOME"

# ── EXIT CODES ────────────────────────────────────────────────────────────────
run_exit "exit code: true"        "true"                    "0"
run_exit "exit code: false"       "false"                   "1"
run_exit "exit code: ls /"        "ls /"                    "0"
run_exit "exit code: bad cmd"     "notacmd_xyz"             "127"
run_exit "exit code: no perm"     "ls /root"                "2"
run_exit "exit code: echo"        "echo hi"                 "0"
run_exit "exit code: pwd"         "pwd"                     "0"

# ── PWD ───────────────────────────────────────────────────────────────────────
run  "pwd"   "pwd"   "$(pwd)"

# ── CD ────────────────────────────────────────────────────────────────────────
run  "cd /tmp"              "cd /tmp"                 ""
run  "cd nonexistent err"   "cd /nonexistent_xyz"     "minishell: cd: /nonexistent_xyz: No such file or directory"
run  "cd too many args"     "cd /tmp /usr"            "minishell: cd: too many arguments"

# ── ENV ───────────────────────────────────────────────────────────────────────
# just check it doesn't crash and has PATH
env_out=$(printf 'env\nexit\n' | $SHELL_BIN 2>&1 | grep -v '^minishell\$' | grep -v '^exit$')
printf "\n%s\n" "$SEP" >> "$OUT"
printf "TEST : env has PATH\n" >> "$OUT"
if echo "$env_out" | grep -q '^PATH='; then
    printf "STATUS: PASS ✓\n" >> "$OUT"; ((PASS++))
else
    printf "STATUS: FAIL ✗ (no PATH in env)\n" >> "$OUT"; ((FAIL++))
fi

# ── EXPORT ────────────────────────────────────────────────────────────────────
run  "export invalid id"    "export 1BAD=x"           "minishell: export: \`1BAD=x': not a valid identifier"
run  "export no args (no crash)" "export" \
     "$(printf 'export\nexit\n' | $SHELL_BIN 2>&1 | grep -v '^minishell\$' | grep -v '^exit$' | head -1)"

# ── UNSET ─────────────────────────────────────────────────────────────────────
run  "unset no crash"   "unset DOESNOTEXIST"   ""

# ── PIPES ─────────────────────────────────────────────────────────────────────
run  "pipe echo|cat"            "echo hello | cat"              "hello"
run  "pipe echo|cat|cat"        "echo hello | cat | cat"        "hello"
run  "pipe ls|grep bin"         "ls / | grep bin"               "bin"
run  "pipe echo|wc -w"          "echo one two three | wc -w"    "3"

# ── REDIRECTIONS ──────────────────────────────────────────────────────────────
# redir out >
printf 'echo redir_out_test > /tmp/ms_redir.txt\nexit\n' | $SHELL_BIN > /dev/null 2>&1
redir_out=$(cat /tmp/ms_redir.txt 2>/dev/null)
printf "\n%s\n" "$SEP" >> "$OUT"
printf "TEST : redir out >\n" >> "$OUT"
printf "GOT  : %s\n" "$redir_out" >> "$OUT"
printf "WANT : redir_out_test\n" >> "$OUT"
if [ "$redir_out" = "redir_out_test" ]; then
    printf "STATUS: PASS ✓\n" >> "$OUT"; ((PASS++))
else
    printf "STATUS: FAIL ✗\n" >> "$OUT"; ((FAIL++))
fi

# redir append >>
printf 'echo line1 > /tmp/ms_app.txt\nexit\n'  | $SHELL_BIN > /dev/null 2>&1
printf 'echo line2 >> /tmp/ms_app.txt\nexit\n' | $SHELL_BIN > /dev/null 2>&1
app_out=$(cat /tmp/ms_app.txt 2>/dev/null)
printf "\n%s\n" "$SEP" >> "$OUT"
printf "TEST : redir append >>\n" >> "$OUT"
printf "GOT  :\n%s\n" "$app_out" >> "$OUT"
printf "WANT :\nline1\nline2\n" >> "$OUT"
if [ "$app_out" = "$(printf 'line1\nline2')" ]; then
    printf "STATUS: PASS ✓\n" >> "$OUT"; ((PASS++))
else
    printf "STATUS: FAIL ✗\n" >> "$OUT"; ((FAIL++))
fi

# redir in <
printf 'echo hello_in > /tmp/ms_in2.txt\nexit\n' | $SHELL_BIN > /dev/null 2>&1
run  "redir in <"   "cat < /tmp/ms_in2.txt"   "hello_in"

# redir out bad path
run "redir out bad dir" \
    "echo x > /nonexistent_dir/file.txt" \
    "minishell: open: No such file or directory"

# redir in nonexistent
run "redir in no file" \
    "cat < /nonexistent_xyz_file.txt" \
    "minishell: open: No such file or directory"

# pipe + redir together
printf 'echo pipe_redir | cat > /tmp/ms_pipe_redir.txt\nexit\n' | $SHELL_BIN > /dev/null 2>&1
pipe_redir=$(cat /tmp/ms_pipe_redir.txt 2>/dev/null)
printf "\n%s\n" "$SEP" >> "$OUT"
printf "TEST : pipe + redir out\n" >> "$OUT"
printf "GOT  : %s\n" "$pipe_redir" >> "$OUT"
printf "WANT : pipe_redir\n" >> "$OUT"
if [ "$pipe_redir" = "pipe_redir" ]; then
    printf "STATUS: PASS ✓\n" >> "$OUT"; ((PASS++))
else
    printf "STATUS: FAIL ✗\n" >> "$OUT"; ((FAIL++))
fi

# ── ERROR MESSAGES ────────────────────────────────────────────────────────────
run "cmd not found msg"     "notacmd_xyz"       "minishell: notacmd_xyz: Command not found"

# ── HEREDOC ───────────────────────────────────────────────────────────────────
heredoc_out=$(printf 'cat << EOF\nhello heredoc\nEOF\nexit\n' | $SHELL_BIN 2>&1 | grep -v '^minishell\$' | grep -v '^exit$' | grep -v '^>')
printf "\n%s\n" "$SEP" >> "$OUT"
printf "TEST : heredoc basic\n" >> "$OUT"
printf "GOT  : %s\n" "$heredoc_out" >> "$OUT"
printf "WANT : hello heredoc\n" >> "$OUT"
if [ "$heredoc_out" = "hello heredoc" ]; then
    printf "STATUS: PASS ✓\n" >> "$OUT"; ((PASS++))
else
    printf "STATUS: FAIL ✗\n" >> "$OUT"; ((FAIL++))
fi

# ── SUMMARY ──────────────────────────────────────────────────────────────────
printf "\n%s\n" "$SEP" >> "$OUT"
printf "PASSED: %d\n" "$PASS" >> "$OUT"
printf "FAILED: %d\n" "$FAIL" >> "$OUT"
TOTAL=$((PASS + FAIL))
printf "TOTAL : %d\n" "$TOTAL" >> "$OUT"

cat "$OUT"
printf "\nResults saved to %s\n" "$OUT"
