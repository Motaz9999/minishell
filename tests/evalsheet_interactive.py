#!/usr/bin/env python3
import pexpect
import os
import signal
import subprocess
import time

PROMPT = "minishell$ "


def spawn_shell(timeout=4):
    sh = pexpect.spawn("./minishell", encoding="utf-8", timeout=timeout)
    sh.expect_exact(PROMPT)
    return sh


def ok(name, passed, detail=""):
    status = "PASS" if passed else "FAIL"
    if detail:
        print(f"{name}: {status} ({detail})")
    else:
        print(f"{name}: {status}")


def test_ctrl_c_empty_prompt():
    sh = spawn_shell()
    sh.sendcontrol("c")
    sh.expect_exact(PROMPT)
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "returned to prompt"


def test_ctrl_backslash_empty_prompt():
    sh = spawn_shell()
    sh.sendcontrol("\\")
    sh.sendline("echo ok")
    sh.expect_exact("ok")
    sh.expect_exact(PROMPT)
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "ignored at prompt"


def test_ctrl_d_empty_prompt_exits():
    sh = spawn_shell()
    sh.sendcontrol("d")
    sh.expect(pexpect.EOF)
    return True, "shell exited"


def test_ctrl_c_with_typed_buffer():
    sh = spawn_shell()
    sh.send("echo should_not_run")
    sh.sendcontrol("c")
    sh.expect_exact(PROMPT)
    sh.sendline("")
    sh.expect_exact(PROMPT)
    data = sh.before or ""
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return ("should_not_run" not in data), "buffer cleared"


def test_ctrl_d_with_typed_buffer_no_exit():
    sh = spawn_shell()
    sh.send("echo typed")
    sh.sendcontrol("d")
    sh.sendcontrol("c")
    sh.expect_exact(PROMPT)
    sh.sendline("echo alive")
    sh.expect_exact("alive")
    sh.expect_exact(PROMPT)
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "did not exit"


def test_ctrl_backslash_with_typed_buffer_no_effect():
    sh = spawn_shell()
    sh.send("echo typed")
    sh.sendcontrol("\\")
    sh.sendcontrol("c")
    sh.expect_exact(PROMPT)
    sh.sendline("echo alive")
    sh.expect_exact("alive")
    sh.expect_exact(PROMPT)
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "ignored typed buffer"


def test_spaces_and_tabs_only_input():
    sh = spawn_shell(timeout=5)
    sh.sendline("   ")
    sh.expect_exact(PROMPT)

    sh.send("\t\t")
    sh.sendline("")
    sh.expect_exact(PROMPT)

    sh.sendline("echo alive")
    sh.expect_exact("alive")
    sh.expect_exact(PROMPT)
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "no crash on spaces/tabs"


def test_blocking_cat_ctrl_c():
    sh = spawn_shell(timeout=5)
    sh.sendline("cat")
    sh.sendcontrol("c")
    sh.expect_exact(PROMPT)
    sh.sendline("echo ok")
    sh.expect_exact("ok")
    sh.expect_exact(PROMPT)
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "interrupts blocking cmd"


def test_blocking_cat_ctrl_backslash():
    sh = spawn_shell(timeout=5)
    sh.sendline("cat")
    time.sleep(0.2)
    res = subprocess.run(
        ["pgrep", "-P", str(sh.pid), "cat"],
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        check=False,
    )
    pids = [int(x) for x in res.stdout.decode().split() if x.strip()]
    if not pids:
        sh.sendline("exit")
        sh.expect(pexpect.EOF)
        return False, "no blocking cat child found"
    os.kill(pids[0], signal.SIGQUIT)
    sh.expect_exact(PROMPT)
    sh.sendline("echo ok")
    sh.expect_exact("ok")
    sh.expect_exact(PROMPT)
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "returns to prompt"


def test_blocking_cat_ctrl_d():
    sh = spawn_shell(timeout=5)
    sh.sendline("cat")
    sh.sendline("")
    sh.sendeof()
    sh.expect_exact(PROMPT)
    sh.sendline("echo ok")
    sh.expect_exact("ok")
    sh.expect_exact(PROMPT)
    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "EOF ends cat"


def test_history_up_down():
    sh = spawn_shell(timeout=5)
    sh.sendline("echo HISTTEST_42")
    sh.expect_exact("HISTTEST_42")
    sh.expect_exact(PROMPT)

    sh.send("\x1b[A")
    sh.sendline("")
    sh.expect_exact("HISTTEST_42")
    sh.expect_exact(PROMPT)

    sh.send("\x1b[A")
    sh.send("\x1b[B")
    sh.sendline("")
    sh.expect_exact(PROMPT)

    sh.sendline("exit")
    sh.expect(pexpect.EOF)
    return True, "history up/down works"


def main():
    tests = [
        ("ctrl-c empty prompt", test_ctrl_c_empty_prompt),
        ("ctrl-\\ empty prompt", test_ctrl_backslash_empty_prompt),
        ("ctrl-d empty prompt", test_ctrl_d_empty_prompt_exits),
        ("ctrl-c with typed buffer", test_ctrl_c_with_typed_buffer),
        ("ctrl-d with typed buffer", test_ctrl_d_with_typed_buffer_no_exit),
        ("ctrl-\\ with typed buffer", test_ctrl_backslash_with_typed_buffer_no_effect),
        ("spaces/tabs only input", test_spaces_and_tabs_only_input),
        ("blocking cat + ctrl-c", test_blocking_cat_ctrl_c),
        ("blocking cat + ctrl-\\", test_blocking_cat_ctrl_backslash),
        ("blocking cat + ctrl-d", test_blocking_cat_ctrl_d),
        ("history up/down", test_history_up_down),
    ]

    passed = 0
    failed = 0
    for name, fn in tests:
        try:
            good, detail = fn()
            ok(name, good, detail)
            if good:
                passed += 1
            else:
                failed += 1
        except Exception as e:
            ok(name, False, repr(e))
            failed += 1

    total = passed + failed
    print(f"INTERACTIVE_SUMMARY: {passed}/{total} pass")


if __name__ == "__main__":
    main()
