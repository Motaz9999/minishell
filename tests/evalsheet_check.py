#!/usr/bin/env python3
import subprocess
from dataclasses import dataclass
from collections import defaultdict

MS = "./minishell"

@dataclass
class Case:
    section: str
    name: str
    script: str
    compare_bash: bool = True


def run_ms(script: str) -> str:
    script_lines = [ln for ln in script.splitlines()]
    try:
        p = subprocess.run(
            [MS],
            input=(script + "\nexit\n").encode(),
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            check=False,
            timeout=8,
            start_new_session=True,
        )
    except subprocess.TimeoutExpired:
        return "__TIMEOUT__"
    out = p.stdout.decode(errors="replace")
    out = out.replace("minishell$ ", "")
    out_lines = []
    for ln in out.splitlines():
        if ln.strip() == "exit":
            continue
        if ln.startswith("> "):
            continue
        if ln in script_lines:
            continue
        out_lines.append(ln)
    return "\n".join(out_lines).strip("\n")


def run_bash(script: str) -> str:
    try:
        p = subprocess.run(
            ["bash", "--noprofile", "--norc"],
            input=(script + "\n").encode(),
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            check=False,
            timeout=8,
            start_new_session=True,
        )
    except subprocess.TimeoutExpired:
        return "__TIMEOUT__"
    return p.stdout.decode(errors="replace").strip("\n")


cases = [
    Case("Simple/Globals", "Absolute command", "/bin/ls"),
    Case("Simple/Globals", "Empty command", ""),
    Case("Simple/Globals", "Spaces only", "    "),
    Case("Simple/Globals", "Tabs only", "\t\t\t"),
    Case("Arguments", "Args no quotes", "/bin/echo one two three"),
    Case("Arguments", "Wrong arg command", "/bin/ls filethatdoesntexist"),
    Case("echo", "echo no args", "echo"),
    Case("echo", "echo text", "echo hello"),
    Case("echo", "echo -n", "echo -n hello"),
    Case("echo", "echo -n -n -n", "echo -n -n -n hello"),
    Case("exit", "exit behavior smoke", "echo before"),
    Case("Return value", "$? after ls ok", "/bin/ls\necho $?"),
    Case("Return value", "$? after ls bad", "/bin/ls filethatdoesntexist\necho $?"),
    Case("Return value", "expr on $? values", "false\nexpr $? + $?"),
    Case("Double Quotes", "quoted pipe symbols literal", 'echo "cat lol.c | cat > lol.c"'),
    Case("Double Quotes", "spaces in quotes", 'echo "hello     world"'),
    Case("Single Quotes", "env not expanded", "echo '$USER'"),
    Case("Single Quotes", "pipes literal in single quotes", "echo 'a | b > c < d'"),
    Case("Single Quotes", "empty single quotes", "echo ''"),
    Case("env", "env prints vars", "env"),
    Case("export", "create var", "export TST_A=42\necho $TST_A"),
    Case("export", "replace var", "export TST_A=1\nexport TST_A=2\necho $TST_A"),
    Case("unset", "unset var", "export TST_B=99\nunset TST_B\necho $TST_B"),
    Case("cd", "cd and pwd", "cd /tmp\npwd"),
    Case("cd", "cd . and ..", "cd /tmp\ncd .\ncd ..\npwd"),
    Case("pwd", "pwd in cwd", "pwd"),
    Case("Relative Path", "complex relative execution", "cd /tmp\n/bin/pwd"),
    Case("Environment Path", "command without path", "ls | head -n 1"),
    Case("Environment Path", "unset PATH then ls", "unset PATH\nls"),
    Case("Environment Path", "PATH left-to-right", "export PATH=/no/such/dir:/bin\nls | head -n 1"),
    Case("Redirection", "output >", "echo hi > /tmp/eval_redir1\ncat /tmp/eval_redir1"),
    Case("Redirection", "append >>", "echo a > /tmp/eval_redir2\necho b >> /tmp/eval_redir2\ncat /tmp/eval_redir2"),
    Case("Redirection", "input <", "cat < /tmp/eval_redir2"),
    Case("Redirection", "heredoc <<", "cat << EOF\nhello\nEOF"),
    Case("Pipes", "simple pipe", "echo hello | cat"),
    Case("Pipes", "multi pipe", "cat /etc/hostname | cat | cat"),
    Case("Pipes", "wrong cmd in pipe", "ls filethatdoesntexist | grep bla | cat"),
    Case("Pipes", "mix pipe and redir", "echo hello | cat > /tmp/eval_mix\ncat /tmp/eval_mix"),
    Case("Go Crazy", "invalid command no crash", "dsbksdgbksdghsd"),
    Case("Go Crazy", "cat | cat | ls", "cat | cat | ls"),
    Case("Go Crazy", "long command args", "echo " + " ".join(["x"] * 400)),
    Case("Environment Variables", "echo $USER", "echo $USER"),
    Case("Environment Variables", 'echo "$USER"', 'echo "$USER"'),
]

bonus_cmds = [
    ("AND operator", "echo a && echo b"),
    ("OR operator", "false || echo recovered"),
    ("Parentheses", "(echo a)"),
    ("Wildcard", "echo tests/* | head -n 1"),
    ("Surprise1", "export USER=ZZ\necho \"'$USER'\""),
    ("Surprise2", "export USER=ZZ\necho '\"$USER\"'"),
]

results = []
for c in cases:
    ms = run_ms(c.script)
    bh = run_bash(c.script) if c.compare_bash else ""
    status = "PASS" if (not c.compare_bash or ms == bh) else "FAIL"
    results.append((c.section, c.name, status, ms, bh))

bonus = []
for name, script in bonus_cmds:
    ms = run_ms(script)
    bh = run_bash(script)
    bonus.append((name, "PASS" if ms == bh else "FAIL", ms, bh))

sec = defaultdict(lambda: [0, 0])
for section, _, status, _, _ in results:
    if status in ("PASS", "FAIL"):
        sec[section][0] += int(status == "PASS")
        sec[section][1] += 1

print("EVALSHEET_MANDATORY_SUMMARY")
for section in sorted(sec):
    passed, total = sec[section]
    print(f"{section}: {passed}/{total} pass")

print("\nMANDATORY_FAILURES")
fails = [row for row in results if row[2] == "FAIL"]
if not fails:
    print("none")
else:
    for section, name, _, ms, bh in fails:
        print(f"[{section}] {name}")
        print("  minishell:", repr(ms[:240]))
        print("  bash     :", repr(bh[:240]))

print("\nBONUS_SUMMARY")
for name, status, _, _ in bonus:
    print(f"{name}: {status}")

print("\nBONUS_FAILURE_DETAILS")
for name, status, ms, bh in bonus:
    if status == "FAIL":
        print(name)
        print("  minishell:", repr(ms[:240]))
        print("  bash     :", repr(bh[:240]))
