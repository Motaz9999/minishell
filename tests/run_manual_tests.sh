#!/bin/bash
# Feeds commands to minishell and compares output with bash where possible

SHELL_BIN=./minishell
OUT=./tests/results.txt
SEP="────────────────────────────────────────"

run() {
    local label="$1"
    local input="$2"
    local expected="$3"

    printf "\n%s\n" "$SEP" >> "$OUT"
    printf "TEST : %s\n" "$label" >> "$OUT"
    printf "INPUT: %s\n" "$input" >> "$OUT"

    actual=$(printf '%s\nexit\n' "$input" | $SHELL_BIN 2>&1 | grep -v '^minishell\$')

    printf "GOT  : %s\n" "$actual" >> "$OUT"
    if [ -n "$expected" ]; then
        printf "WANT : %s\n" "$expected" >> "$OUT"
        if [ "$actual" = "$expected" ]; then
            printf "STATUS: PASS ✓\n" >> "$OUT"
        else
            printf "STATUS: FAIL ✗\n" >> "$OUT"
        fi
    fi
}

> "$OUT"
printf "MINISHELL TEST RESULTS — $(date)\n" >> "$OUT"

########## ECHO ##########
run "echo hello"            "echo hello"                    "hello"
run "echo -n hello"         "echo -n hello"                 "hello"
run "echo multiple args"    "echo one two three"            "one two three"
run "echo empty"            "echo"                          ""
run "echo with quotes"      'echo "hello world"'            "hello world"
run "echo single quotes"    "echo 'hello world'"            "hello world"
run "echo \$? after true"   "true; echo \$?"                "0"
run "echo \$? after false"  "false; echo \$?"               "1"

########## PWD ##########
run "pwd"   "pwd"   "$(pwd)"

########## CD ##########
run "cd /tmp then pwd"      "cd /tmp; pwd"          "/tmp"
run "cd .."                 "cd /tmp; cd ..; pwd"   "/"
run "cd ~ "                 "cd ~; pwd"             "$HOME"
run "cd no args"            "cd; pwd"               "$HOME"
run "cd nonexistent"        "cd /nonexistent_dir_xyz"  "minishell: cd: /nonexistent_dir_xyz: No such file or directory"

########## ENV ##########
run "env contains PATH"     "env | grep ^PATH"      "$(env | grep ^PATH)"

########## EXPORT ##########
run "export new var"        'export TESTVAR=hello; echo $TESTVAR'          "hello"
run "export no value"       'export BAREVAR; env | grep ^BAREVAR'          "BAREVAR"
run "export no args"        'export' ""   # just check no crash, output too long
run "export invalid"        'export 1INVALID=x'     "minishell: export: \`1INVALID=x': not a valid identifier"
run "export update"         'export X=1; export X=2; echo $X'              "2"

########## UNSET ##########
run "unset var"             'export DEL=yes; unset DEL; echo $DEL'         ""
run "unset nonexistent"     'unset DOESNOTEXIST; echo ok'                  "ok"

########## EXIT ##########
run "exit 0"    "exit 0"    ""
run "exit 42"   "exit 42"   ""

########## REDIRECTIONS ##########
run "redir out >"       'echo hello > /tmp/ms_test.txt; cat /tmp/ms_test.txt'   "hello"
run "redir append >>"   'echo a > /tmp/ms_test.txt; echo b >> /tmp/ms_test.txt; cat /tmp/ms_test.txt' "$(printf 'a\nb')"
run "redir in <"        'echo world > /tmp/ms_in.txt; cat < /tmp/ms_in.txt'     "world"

########## PIPES ##########
run "simple pipe"           'echo hello | cat'                  "hello"
run "pipe with grep"        'echo -e "a\nb\nc" | grep b'        "b"
run "multi pipe"            'echo hello | cat | cat'            "hello"

########## HEREDOC ##########
run "heredoc basic"   $'cat << EOF\nhello\nEOF'   "hello"

########## EXPANDER ##########
run "expand \$HOME"     'echo $HOME'        "$HOME"
run "expand \$?"        'true; echo $?'     "0"
run "no expand in ''"   "echo '\$HOME'"     '$HOME'
run "expand in \"\""    'echo "$HOME"'      "$HOME"

########## EXTERNAL CMDS ##########
run "ls /tmp"           "ls /tmp"           "$(ls /tmp)"
run "cat /etc/hostname" "cat /etc/hostname" "$(cat /etc/hostname)"
run "cmd not found"     "notacmd_xyz"       "minishell: notacmd_xyz: Command not found"

printf "\n%s\n" "$SEP" >> "$OUT"
printf "DONE. Results saved to $OUT\n"
cat "$OUT"
