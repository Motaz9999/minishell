# new note

Main failing-test sources

1. PATH lookup accepts non-executable entries
- File: [src/executor/path_handle.c](src/executor/path_handle.c#L43)
- Current behavior: uses access with F_OK when searching PATH.
- Problem: directories can be treated like valid command paths, especially when command is empty.

2. Command error mapping is too generic
- File: [src/executor/executor_utils.c](src/executor/executor_utils.c#L122)
- Current behavior: any access X_OK failure becomes Permission denied with exit 126.
- Problem: missing files should be No such file or directory with exit 127.

How this maps to your failing tests

1. Test 134 ($EMPTY)
2. Test 135 ($EMPTY echo hi)
- Root cause: empty first token reaches PATH search and matches wrong candidate.
- Related lines: [src/executor/path_handle.c](src/executor/path_handle.c#L43), [src/executor/executor_utils.c](src/executor/executor_utils.c#L115)

1. Test 137 (./missing.out)
2. Test 142 (/test_files)
- Root cause: execution failure branch always maps to Permission denied/126.
- Related line: [src/executor/executor_utils.c](src/executor/executor_utils.c#L122)

1. Test 133 ($PWD) warning
2. Test 141 (./test_files) warning
- Root cause: directory-execution message does not match bash wording.
- Related line: [src/executor/executor_utils.c](src/executor/executor_utils.c#L122)

Fix order (highest impact first)

1. In PATH search, check executability (X_OK), not just existence (F_OK), and reject directories.
2. In execute path handling, map errors by errno:
- ENOENT -> No such file or directory, exit 127
- EACCES -> Permission denied, exit 126
- directory case -> Is a directory, exit 126
3. Before resolve_path, handle empty command after expansion (args[0] == "").

After these changes, rerun tester cases 133 to 142 first.