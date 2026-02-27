#!/bin/bash
# ══════════════════════════════════════════════════════════════════════════════
#  ULTIMATE EXECUTOR TEST — tests everything in the executor's domain:
#  builtins, pipes, redirections, heredoc, exit codes, error messages
#  Does NOT test semicolons (parser responsibility)
# ══════════════════════════════════════════════════════════════════════════════

SHELL_BIN=./minishell
OUT=./tests/ultimate_results.txt
PASS=0
FAIL=0
SECTION=""

> "$OUT"

# ── helpers ──────────────────────────────────────────────────────────────────

section() {
    SECTION="$1"
    printf "\n\n══ %s ══\n" "$SECTION" >> "$OUT"
    printf "══ %s ══\n" "$SECTION"
}

run() {
    local label="$1"
    local input="$2"
    local expected="$3"

    actual=$(printf '%s\nexit\n' "$input" | $SHELL_BIN 2>&1 \
        | grep -v '^minishell\$' | grep -v '^exit$')

    if [ "$actual" = "$expected" ]; then
        printf "  ✓ %s\n" "$label" | tee -a "$OUT"
        ((PASS++))
    else
        printf "  ✗ %s\n" "$label" | tee -a "$OUT"
        printf "      INPUT : %s\n"    "$input"    >> "$OUT"
        printf "      WANTED: %s\n"    "$expected" >> "$OUT"
        printf "      GOT   : %s\n"    "$actual"   >> "$OUT"
        ((FAIL++))
    fi
}

# run a cmd, then echo $? — checks exit code
run_code() {
    local label="$1"
    local input="$2"
    local expected_code="$3"

    actual=$(printf '%s\necho $?\nexit\n' "$input" | $SHELL_BIN 2>&1 \
        | grep -v '^minishell\$' | grep -v '^exit$' | tail -1)

    if [ "$actual" = "$expected_code" ]; then
        printf "  ✓ %s  [\$?=%s]\n" "$label" "$expected_code" | tee -a "$OUT"
        ((PASS++))
    else
        printf "  ✗ %s  [wanted \$?=%s  got \$?=%s]\n" \
               "$label" "$expected_code" "$actual" | tee -a "$OUT"
        printf "      INPUT : %s\n" "$input" >> "$OUT"
        ((FAIL++))
    fi
}

# write a file via minishell then read it back in bash
run_file() {
    local label="$1"
    local ms_cmd="$2"
    local filepath="$3"
    local expected="$4"

    printf '%s\nexit\n' "$ms_cmd" | $SHELL_BIN > /dev/null 2>&1
    actual=$(cat "$filepath" 2>/dev/null)

    if [ "$actual" = "$expected" ]; then
        printf "  ✓ %s\n" "$label" | tee -a "$OUT"
        ((PASS++))
    else
        printf "  ✗ %s\n" "$label" | tee -a "$OUT"
        printf "      WANTED: %s\n" "$expected" >> "$OUT"
        printf "      GOT   : %s\n" "$actual"   >> "$OUT"
        ((FAIL++))
    fi
}

printf "ULTIMATE MINISHELL TEST — $(date)\n" >> "$OUT"

# ══════════════════════════════════════════════════════════════════════════════
section "ECHO"
# ══════════════════════════════════════════════════════════════════════════════
run  "echo hello"                "echo hello"              "hello"
run  "echo -n (no newline)"      "echo -n hello"           "hello"
run  "echo multiple args"        "echo a b c"              "a b c"
run  "echo empty"                "echo"                    ""
run  "echo double quotes"        'echo "hello world"'      "hello world"
run  "echo single quotes"        "echo 'hello world'"      "hello world"
run  "echo expand \$HOME"        'echo $HOME'              "$HOME"
run  "echo no-expand in ''"      "echo '\$HOME'"            '$HOME'
run  "echo expand in \"\""       'echo "$HOME"'            "$HOME"
run  "echo -n -n -n"             "echo -n -n -n hello"     "hello"

# ══════════════════════════════════════════════════════════════════════════════
section "EXIT CODES"
# ══════════════════════════════════════════════════════════════════════════════
run_code "echo exits 0"          "echo hi"                 "0"
run_code "pwd exits 0"           "pwd"                     "0"
run_code "ls / exits 0"          "ls /"                    "0"
run_code "true exits 0"          "true"                    "0"
run_code "false exits 1"         "false"                   "1"
run_code "cmd not found → 127"   "notacmd_xyz_abc"         "127"
run_code "bad redir → non-zero"  "cat < /no_such_file_xyz" "1"
run_code "\$? after echo"        "echo hi"                 "0"

# ══════════════════════════════════════════════════════════════════════════════
section "PWD"
# ══════════════════════════════════════════════════════════════════════════════
run  "pwd output"   "pwd"   "$(pwd)"
run_code "pwd exits 0"   "pwd"   "0"

# ══════════════════════════════════════════════════════════════════════════════
section "CD"
# ══════════════════════════════════════════════════════════════════════════════
run      "cd /tmp (no output)"       "cd /tmp"              ""
run      "cd too many args error"    "cd /tmp /usr"         "minishell: cd: too many arguments"
run      "cd nonexistent error"      "cd /nonexistent_xyz"  "minishell: cd: /nonexistent_xyz: No such file or directory"
run_code "cd /tmp exits 0"           "cd /tmp"              "0"
run_code "cd nonexistent exits 1"    "cd /nonexistent_xyz"  "1"

# ══════════════════════════════════════════════════════════════════════════════
section "ENV"
# ══════════════════════════════════════════════════════════════════════════════
env_out=$(printf 'env\nexit\n' | $SHELL_BIN 2>&1 | grep -v '^minishell\$' | grep -v '^exit$')
has_path=$(echo "$env_out" | grep -c '^PATH=')
has_home=$(echo "$env_out" | grep -c '^HOME=')
has_user=$(echo "$env_out" | grep -c '^USER=')
[ "$has_path" -ge 1 ] && { printf "  ✓ env has PATH\n" | tee -a "$OUT"; ((PASS++)); } \
                       || { printf "  ✗ env missing PATH\n" | tee -a "$OUT"; ((FAIL++)); }
[ "$has_home" -ge 1 ] && { printf "  ✓ env has HOME\n"  | tee -a "$OUT"; ((PASS++)); } \
                       || { printf "  ✗ env missing HOME\n"  | tee -a "$OUT"; ((FAIL++)); }
[ "$has_user" -ge 1 ] && { printf "  ✓ env has USER\n"  | tee -a "$OUT"; ((PASS++)); } \
                       || { printf "  ✗ env missing USER\n"  | tee -a "$OUT"; ((FAIL++)); }
run_code "env exits 0"  "env"  "0"

# ══════════════════════════════════════════════════════════════════════════════
section "EXPORT"
# ══════════════════════════════════════════════════════════════════════════════
run  "export invalid (number start)"   "export 1BAD=x"          "minishell: export: \`1BAD=x': not a valid identifier"
run  "export invalid (special char)"   "export BA-D=x"          "minishell: export: \`BA-D=x': not a valid identifier"
run_code "export invalid → exits 1"    "export 1BAD=x"          "1"

# export no-args produces declare -x lines
exp_out=$(printf 'export\nexit\n' | $SHELL_BIN 2>&1 | grep -v '^minishell\$' | grep -v '^exit$')
has_decl=$(echo "$exp_out" | grep -c '^declare -x ')
[ "$has_decl" -ge 5 ] && { printf "  ✓ export no-args prints declare -x lines\n" | tee -a "$OUT"; ((PASS++)); } \
                       || { printf "  ✗ export no-args missing declare -x lines\n" | tee -a "$OUT"; ((FAIL++)); }

# ══════════════════════════════════════════════════════════════════════════════
section "UNSET"
# ══════════════════════════════════════════════════════════════════════════════
run      "unset nonexistent (no crash)"   "unset DOESNOTEXIST_XYZ"   ""
run_code "unset exits 0"                  "unset DOESNOTEXIST_XYZ"   "0"

# ══════════════════════════════════════════════════════════════════════════════
section "EXTERNAL COMMANDS"
# ══════════════════════════════════════════════════════════════════════════════
run      "ls /"                    "ls /"                         "$(ls /)"
run      "cat /etc/hostname"       "cat /etc/hostname"            "$(cat /etc/hostname)"
run      "cmd not found message"   "notacmd_xyz_abc"              "minishell: notacmd_xyz_abc: Command not found"
run_code "successful external → 0" "ls /"                        "0"
run_code "cmd not found → 127"     "notacmd_xyz_abc"              "127"

# ══════════════════════════════════════════════════════════════════════════════
section "PIPES"
# ══════════════════════════════════════════════════════════════════════════════
run  "echo | cat"               "echo hello | cat"               "hello"
run  "echo | cat | cat"         "echo hello | cat | cat"         "hello"
run  "echo | wc -w"             "echo one two three | wc -w"     "3"
run  "ls / | grep usr"          "ls / | grep usr"                "usr"
run  "cat /etc/hostname | cat"  "cat /etc/hostname | cat"        "$(cat /etc/hostname)"
run  "echo | tr"                "echo hello | tr a-z A-Z"        "HELLO"
run_code "pipe exits with last cmd code"  "echo hi | true"  "0"
run_code "pipe last cmd fails → non-0"   "echo hi | false" "1"

# ══════════════════════════════════════════════════════════════════════════════
section "REDIRECTIONS — OUTPUT >"
# ══════════════════════════════════════════════════════════════════════════════
run_file "echo > file"  \
    "echo redir_works > /tmp/ms_u1.txt"  \
    "/tmp/ms_u1.txt"  "redir_works"

run_file "overwrites existing file"  \
    "echo first > /tmp/ms_u2.txt"    "/tmp/ms_u2.txt" "first"
run_file "overwrite with second cmd" \
    "echo second > /tmp/ms_u2.txt"   "/tmp/ms_u2.txt" "second"

run      "bad redir path error"  \
    "echo x > /no_such_dir/f.txt"  \
    "minishell: open: No such file or directory"

run_code "redir out exits 0"  "echo x > /tmp/ms_u_exit.txt"  "0"

# ══════════════════════════════════════════════════════════════════════════════
section "REDIRECTIONS — APPEND >>"
# ══════════════════════════════════════════════════════════════════════════════
printf 'echo line1 > /tmp/ms_app.txt\nexit\n'  | $SHELL_BIN > /dev/null 2>&1
printf 'echo line2 >> /tmp/ms_app.txt\nexit\n' | $SHELL_BIN > /dev/null 2>&1
printf 'echo line3 >> /tmp/ms_app.txt\nexit\n' | $SHELL_BIN > /dev/null 2>&1
app_got=$(cat /tmp/ms_app.txt 2>/dev/null)
app_want=$(printf 'line1\nline2\nline3')
if [ "$app_got" = "$app_want" ]; then
    printf "  ✓ append >> builds up file\n" | tee -a "$OUT"; ((PASS++))
else
    printf "  ✗ append >> builds up file\n" | tee -a "$OUT"
    printf "      WANTED: %s\n" "$app_want" >> "$OUT"
    printf "      GOT   : %s\n" "$app_got"  >> "$OUT"
    ((FAIL++))
fi

# ══════════════════════════════════════════════════════════════════════════════
section "REDIRECTIONS — INPUT <"
# ══════════════════════════════════════════════════════════════════════════════
printf 'echo input_test > /tmp/ms_in.txt\nexit\n' | $SHELL_BIN > /dev/null 2>&1
run  "cat < file"           "cat < /tmp/ms_in.txt"           "input_test"
run  "redir in no file err" "cat < /no_such_file_xyz.txt"    "minishell: open: No such file or directory"
run_code "redir in exits 0" "cat < /tmp/ms_in.txt"           "0"
run_code "redir in bad file → 1" "cat < /no_such_file_xyz.txt" "1"

# ══════════════════════════════════════════════════════════════════════════════
section "PIPE + REDIR COMBINED"
# ══════════════════════════════════════════════════════════════════════════════
run_file "echo | cat > file"  \
    "echo pipe_then_redir | cat > /tmp/ms_pr.txt"  \
    "/tmp/ms_pr.txt"  "pipe_then_redir"

printf 'echo hello > /tmp/ms_combo_in.txt\nexit\n' | $SHELL_BIN > /dev/null 2>&1
run  "cat < infile | cat"  \
    "cat < /tmp/ms_combo_in.txt | cat"  "hello"

# ══════════════════════════════════════════════════════════════════════════════
section "HEREDOC <<"
# ══════════════════════════════════════════════════════════════════════════════
heredoc_out=$(printf 'cat << EOF\nhello heredoc\nEOF\nexit\n' | $SHELL_BIN 2>&1 \
    | grep -v '^minishell\$' | grep -v '^exit$' | grep -v '^>')
if [ "$heredoc_out" = "hello heredoc" ]; then
    printf "  ✓ heredoc basic output\n" | tee -a "$OUT"; ((PASS++))
else
    printf "  ✗ heredoc basic output\n" | tee -a "$OUT"
    printf "      GOT: %s\n" "$heredoc_out" >> "$OUT"
    ((FAIL++))
fi

heredoc_multi=$(printf 'cat << STOP\nline1\nline2\nline3\nSTOP\nexit\n' | $SHELL_BIN 2>&1 \
    | grep -v '^minishell\$' | grep -v '^exit$' | grep -v '^>')
if [ "$heredoc_multi" = "$(printf 'line1\nline2\nline3')" ]; then
    printf "  ✓ heredoc multiple lines\n" | tee -a "$OUT"; ((PASS++))
else
    printf "  ✗ heredoc multiple lines\n" | tee -a "$OUT"
    printf "      GOT: %s\n" "$heredoc_multi" >> "$OUT"
    ((FAIL++))
fi

# ══════════════════════════════════════════════════════════════════════════════
section "EXPANSION"
# ══════════════════════════════════════════════════════════════════════════════
run  "expand \$HOME"          'echo $HOME'           "$HOME"
run  "expand \$USER"          'echo $USER'           "$USER"
run  "expand in double quote" 'echo "$HOME"'         "$HOME"
run  "no expand in single ''" "echo '\$HOME'"         '$HOME'
run  "expand \$? = 0"         'true; echo $?'        ""   # semicolons = parser; skip with note
# Direct $? test (no semicolons needed — use two-line input)
dollar_q=$(printf 'echo $?\nexit\n' | $SHELL_BIN 2>&1 | grep -v '^minishell\$' | grep -v '^exit$' | head -1)
if [ "$dollar_q" = "0" ]; then
    printf "  ✓ \$? is 0 at shell start\n" | tee -a "$OUT"; ((PASS++))
else
    printf "  ✗ \$? is 0 at shell start (got %s)\n" "$dollar_q" | tee -a "$OUT"; ((FAIL++))
fi

# ══════════════════════════════════════════════════════════════════════════════
# SUMMARY
# ══════════════════════════════════════════════════════════════════════════════
TOTAL=$((PASS + FAIL))
printf "\n══════════════════════════════════════════\n" | tee -a "$OUT"
printf "  PASSED : %d / %d\n" "$PASS" "$TOTAL" | tee -a "$OUT"
printf "  FAILED : %d / %d\n" "$FAIL" "$TOTAL" | tee -a "$OUT"
printf "══════════════════════════════════════════\n" | tee -a "$OUT"
printf "\nFull results → %s\n" "$OUT"
