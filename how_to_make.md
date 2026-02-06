# MINISHELL - COMPLETE CONCEPTUAL IMPLEMENTATION GUIDE
## Project Duration: 6-8 Weeks | Partners: 2

---

# PART 1: COMPLETE DATA STRUCTURES EXPLANATION

## 1. TOKEN STRUCTURES (For Lexical Analysis)

### t_token_type (Enumeration)
**Purpose**: Classify each piece of user input into categories

**Values**:
- `TOKEN_WORD` - Regular text like commands, arguments, filenames
- `TOKEN_PIPE` - The `|` symbol
- `TOKEN_REDIR_IN` - The `<` symbol  
- `TOKEN_REDIR_OUT` - The `>` symbol
- `TOKEN_REDIR_APPEND` - The `>>` symbols
- `TOKEN_REDIR_HEREDOC` - The `<<` symbols
- `TOKEN_AND` - The `&&` operator (BONUS)
- `TOKEN_OR` - The `||` operator (BONUS)
- `TOKEN_LPAREN` - The `(` character (BONUS)
- `TOKEN_RPAREN` - The `)` character (BONUS)
- `TOKEN_EOF` - End of input marker

**Why needed?** 
The parser needs to know what each piece of input represents to build commands correctly.

### t_token (Linked List Node)
**Purpose**: Store individual pieces (tokens) of the parsed input

**Fields**:
- `type` - Which kind of token this is (from t_token_type)
- `value` - The actual text string (e.g., "echo", "|", "file.txt")
- `quote_type` - How this token was quoted: 0=none, 1=single quotes, 2=double quotes
- `next` - Pointer to the next token in the sequence

**Data Structure Choice**: Linked list
**Why?** 
- Unknown number of tokens until we parse
- Easy to traverse sequentially during parsing
- Simple to free memory

**Example Data Flow**:
```
Input: echo "hello world" | grep hello

Tokenization creates:
Token 1: [type=WORD, value="echo", quote_type=0] →
Token 2: [type=WORD, value="hello world", quote_type=2] →
Token 3: [type=PIPE, value="|", quote_type=0] →
Token 4: [type=WORD, value="grep", quote_type=0] →
Token 5: [type=WORD, value="hello", quote_type=0] →
NULL
```

---

## 2. REDIRECTION STRUCTURES

### t_redir_type (Enumeration)
**Purpose**: Identify different redirection operations

**Values**:
- `REDIR_IN` - Input redirection `<` (read from file)
- `REDIR_OUT` - Output redirection `>` (write to file, truncate)
- `REDIR_APPEND` - Append redirection `>>` (write to file, append)
- `REDIR_HEREDOC` - Here-document `<<` (read input until delimiter)

### t_redirect (Linked List Node)
**Purpose**: Store redirection information for a command

**Fields**:
- `type` - Which kind of redirection (from t_redir_type)
- `file` - Filename to redirect to/from, or delimiter for heredoc
- `fd` - File descriptor (initially -1, filled during execution)
- `next` - Pointer to next redirection

**Data Structure Choice**: Linked list
**Why?** A single command can have multiple redirections

**Example Data Flow**:
```
Input: cat < input.txt > output.txt 2> error.txt

Redirection List:
Redir 1: [type=REDIR_IN, file="input.txt", fd=-1] →
Redir 2: [type=REDIR_OUT, file="output.txt", fd=-1] →  
Redir 3: [type=REDIR_OUT, file="error.txt", fd=-1] →
NULL
```

---

## 3. COMMAND STRUCTURES

### t_command (Linked List Node)
**Purpose**: Represent one command in a pipeline

**Fields**:
- `args` - NULL-terminated array of strings (command and arguments)
- `redirects` - Pointer to first redirection (linked list)
- `next` - Pointer to next command in pipeline

**Data Structure Choices**:
- `args` is an **array** because `execve()` system call requires it
- `redirects` is a **linked list** because number varies
- `next` creates a **linked list** of commands for pipelines

**Example Data Flow**:
```
Input: ls -la /home | grep user | wc -l

Command Chain:
┌─────────────────────┐
│ Command 1           │
│ args: ["ls","-la",  │
│        "/home",NULL]│
│ redirects: NULL     │
└──────┬──────────────┘
       │ next
       ▼
┌─────────────────────┐
│ Command 2           │
│ args: ["grep",      │
│        "user",NULL] │
│ redirects: NULL     │
└──────┬──────────────┘
       │ next
       ▼
┌─────────────────────┐
│ Command 3           │
│ args: ["wc",        │
│        "-l",NULL]   │
│ redirects: NULL     │
└──────┬──────────────┘
       │ next
       ▼
      NULL
```

---

## 4. ABSTRACT SYNTAX TREE (AST) - BONUS ONLY

### t_node_type (Enumeration)
**Purpose**: Identify node types in command trees

**Values**:
- `NODE_COMMAND` - Leaf node containing actual command
- `NODE_PIPE` - Pipeline operator node
- `NODE_AND` - AND operator `&&` node
- `NODE_OR` - OR operator `||` node

### t_ast_node (Tree Node)
**Purpose**: Build hierarchical command structures for complex expressions

**Fields**:
- `type` - Node type (from t_node_type)
- `command` - Pointer to t_command (if leaf node)
- `left` - Left child node
- `right` - Right child node

**Data Structure Choice**: Binary tree
**Why?** Represents operator precedence and grouping with parentheses

**Example Data Flow**:
```
Input: (ls | grep x) && echo "found" || echo "not found"

AST Structure:
              OR
            /    \
          AND    COMMAND
         /   \   (echo not found)
      PIPE  COMMAND
     /   \  (echo found)
   CMD  CMD
   (ls) (grep)
```

---

## 5. ENVIRONMENT STRUCTURES

### t_env (Linked List Node)
**Purpose**: Store environment variables

**Fields**:
- `key` - Variable name (e.g., "PATH", "HOME", "USER")
- `value` - Variable value (e.g., "/usr/bin:/bin", "/home/user")
- `next` - Pointer to next variable

**Data Structure Choice**: Linked list
**Why?**
- Easy insertion and deletion for export/unset
- Dynamic size (don't know how many variables)
- Can convert to array when needed for `execve()`

**Example Data Flow**:
```
Environment Variables:
[key="PATH", value="/usr/bin:/bin"] →
[key="HOME", value="/home/user"] →
[key="USER", value="student"] →
[key="SHELL", value="/bin/bash"] →
NULL

Conversion to Array for execve():
envp[0] = "PATH=/usr/bin:/bin"
envp[1] = "HOME=/home/user"
envp[2] = "USER=student"
envp[3] = "SHELL=/bin/bash"
envp[4] = NULL
```

---

## 6. MAIN SHELL CONTEXT

### t_shell (Central State Structure)
**Purpose**: Hold ALL shell state and context

**Fields Breakdown**:

**Environment Management**:
- `env_list` - Linked list of environment variables (for manipulation)
- `envp` - Array format of environment (for execve(), rebuilt when env changes)

**Execution State**:
- `last_exit_status` - Exit code of last executed command (for `$?` expansion)
- `child_pids` - Array tracking all child process IDs
- `child_count` - Number of active children

**Parsing State** (temporary, cleared after each command):
- `tokens` - Token list from current input
- `commands` - Command list from current parsing
- `ast` - AST root (for bonus features)

**Terminal State**:
- `original_term` - Terminal settings before shell started
- `shell_term` - Modified terminal settings for shell

**Other**:
- `history_count` - Number of commands in history

**Why this design?**
- Single source of truth for shell state
- Easy to pass context to functions
- Clear separation: persistent vs temporary data
- Organized cleanup (free tokens/commands after each command)

---

## 7. GLOBAL VARIABLE (Only One Allowed!)

### g_signal
**Type**: `volatile sig_atomic_t`
**Purpose**: Store signal numbers received by signal handlers
**Value**: Signal number (SIGINT=2, SIGQUIT=3, etc.) or 0 for no signal

**CRITICAL CONSTRAINTS**:
- ❌ CANNOT be a struct
- ❌ CANNOT point to other data
- ❌ CANNOT store anything except signal numbers
- ✅ ONLY for communication between signal handler and main program
- ✅ All other data MUST be in t_shell

**Why these restrictions?**
- Signal handlers can only safely access very simple variables
- Subject explicitly forbids storing other data here
- Prevents race conditions and undefined behavior

**Example Usage**:
```
Signal handler (Ctrl-C):
  g_signal = SIGINT;  // Store signal number
  
Main loop:
  if (g_signal == SIGINT)
    // Handle interrupt
    g_signal = 0;  // Reset
```

---

## 8. PARSER STATE

### t_parser
**Purpose**: Track parsing progress (temporary object)

**Fields**:
- `current` - Pointer to current token being examined
- `shell` - Reference to shell context (for accessing environment)
- `error` - Error flag (0=ok, 1=error detected)
- `error_msg` - Description of syntax error

**Why separate from t_shell?**
- Temporary structure only used during parsing
- Keeps parsing logic isolated and testable
- Can be freed immediately after parsing
- Cleaner error handling

**Example Flow**:
```
1. Create parser pointing to first token
2. Advance through tokens
3. Build command structures
4. If error: set flag and message
5. Return commands or NULL
6. Free parser
```

---

## 9. EXECUTION CONTEXT

### t_pipe_info
**Purpose**: Manage pipe file descriptors during pipeline execution

**Fields**:
- `pipe_fds[2]` - Current pipe: [read_end, write_end]
- `prev_fd` - Previous pipe's read end (for chaining)
- `is_last` - Boolean: is this the last command?

**Why needed?**
Each command in a pipeline needs to know:
- Where to read input from (previous pipe or stdin)
- Where to write output to (next pipe or stdout)
- Whether to create a new pipe

**Example Flow** (`ls | grep | wc`):
```
Command 1 (ls):
  stdin: 0 (original)
  stdout: pipe1[write] = 3
  Create pipe1[read=4, write=3]
  prev_fd: -1 (no previous pipe)
  is_last: false

Command 2 (grep):
  stdin: pipe1[read] = 4 (from ls)
  stdout: pipe2[write] = 5
  Create pipe2[read=6, write=5]
  prev_fd: 4
  is_last: false

Command 3 (wc):
  stdin: pipe2[read] = 6 (from grep)
  stdout: 1 (original)
  No new pipe created
  prev_fd: 6
  is_last: true
```

### t_exec_context
**Purpose**: Package all information needed to execute one command

**Fields**:
- `cmd` - The command structure to execute
- `shell` - Shell context (for environment, exit status)
- `pipe_info` - Pipe management info
- `stdin_backup` - Saved stdin (for builtins in parent process)
- `stdout_backup` - Saved stdout (for builtins in parent process)

**Why backups?**
Builtins run in the parent process, so we need to:
1. Apply redirections
2. Execute builtin
3. Restore original file descriptors

External commands run in child processes, so they can modify FDs freely.

---

# PART 2: DETAILED IMPLEMENTATION PHASES

---

## PHASE 1: PROJECT SETUP (Days 1-4)

### Goals:
- Create project structure
- Define all data structures
- Set up build system
- Create basic shell loop

### BOTH PARTNERS - Days 1-2

#### Task 1.1: Directory Structure
Create organized folder hierarchy:
```
minishell/
├── Makefile           (Build rules)
├── README.md          (Project documentation)
├── includes/
│   ├── minishell.h    (Function prototypes, includes)
│   └── structs.h      (All data structure definitions)
├── srcs/
│   ├── main.c         (Shell main loop)
│   ├── parsing/       (Tokenizer, parser, expander)
│   ├── execution/     (Executor, pipes, path resolution)
│   ├── builtins/      (Built-in commands)
│   ├── environment/   (Environment management)
│   ├── signals/       (Signal handlers)
│   └── utils/         (Memory, errors, cleanup)
├── libft/             (Your library)
└── tests/             (Test programs - not submitted)
```

**Why this organization?**
- Clear separation of concerns
- Easy to find and modify code
- Scales well as project grows
- Matches common software architecture

#### Task 1.2: Makefile Rules
Create comprehensive build system:

**Required Rules**:
- `all` - Build minishell executable
- `clean` - Remove object files
- `fclean` - Remove objects and executable
- `re` - Rebuild everything
- `bonus` - Build with bonus features (if applicable)

**Important Compilation Steps**:
1. Compile libft first
2. Compile all .c files to .o objects
3. Link everything together with readline library
4. Use flags: `-Wall -Wextra -Werror`

**Why Makefile matters?**
- Automatic dependency tracking
- Only recompiles changed files
- Consistent build process
- Required for submission

#### Task 1.3: Define All Structures
In `includes/structs.h`, write out ALL data structures (shown in Part 1)

**Critical**: Both partners MUST use identical structures!

**Why do this first?**
- Prevents incompatibilities later
- Forces you to think through design
- Makes coding easier (know what data is available)
- Easier to spot design flaws early

#### Task 1.4: Create Function Prototypes
In `includes/minishell.h`, declare all functions you'll implement

**Categories**:
- Parsing functions
- Execution functions
- Builtin functions
- Environment functions
- Signal functions
- Utility functions
- Cleanup functions

**Note**: You'll add more as you code, but start with basics

#### Task 1.5: Basic Main Loop
Create `srcs/main.c` with shell loop structure:

**Main Loop Concept**:
```
1. Initialize shell context
   - Create environment from system envp
   - Set up signal handlers
   - Initialize all fields

2. Infinite loop:
   a. Display prompt ("minishell$ ")
   b. Read line from user (readline)
   c. Check for EOF (Ctrl-D) → exit
   d. Add to history (if not empty)
   e. Parse input → tokens → commands
   f. Execute commands
   g. Update exit status
   h. Free temporary data (tokens, commands)

3. Exit cleanup:
   - Free all allocated memory
   - Return last exit status
```

**Test Goal**: Program compiles and shows prompt (even if it doesn't do anything yet)

---

### PARTNER A - Days 3-4: TOKENIZER

#### Understanding Tokenization

**What is tokenization?**
Breaking input string into meaningful pieces (tokens)

**Example**:
```
Input String: "cat < file.txt | grep hello > output.txt"

Step-by-Step Tokenization:
1. Read "cat" → WORD token
2. Skip space
3. Read "<" → REDIR_IN token
4. Skip space
5. Read "file.txt" → WORD token
6. Skip space
7. Read "|" → PIPE token
8. Skip space
9. Read "grep" → WORD token
10. Skip space
11. Read "hello" → WORD token
12. Skip space
13. Read ">" → REDIR_OUT token
14. Skip space
15. Read "output.txt" → WORD token

Result: 8 tokens in a linked list
```

#### Task A1: Research Tokenization Rules

**Study Questions**:
1. What are metacharacters?
   - Special characters: `|`, `<`, `>`, `;`, `&`, `(`, `)`
   - Have special meaning, not treated as regular text
   
2. How do quotes work?
   - Single quotes `'...'`: Everything inside is literal
   - Double quotes `"..."`: Allow variable expansion `$VAR`
   - Quotes are NOT included in the token value
   
3. What about whitespace?
   - Spaces, tabs, newlines separate tokens
   - Multiple whitespace = one separator
   - Whitespace inside quotes is preserved

**Testing in Bash**:
```bash
$ echo hello world
# Output: hello world (2 separate args)

$ echo "hello    world"
# Output: hello    world (1 arg, spaces preserved)

$ echo 'test$USER'
# Output: test$USER (no expansion in single quotes)

$ echo "test$USER"
# Output: testjohn (expansion in double quotes)

$ ls|grep test
# Works without spaces (tokenizer must handle this)
```

#### Task A2: Design Token Extraction

**Character Classification Functions**:

**is_whitespace(char c)**:
- Purpose: Identify separators
- Return: 1 if space/tab/newline, 0 otherwise

**is_metachar(char c)**:
- Purpose: Identify special characters
- Return: 1 if `|<>&;()`, 0 otherwise

**Skip Functions**:

**skip_whitespace(char **ptr)**:
- Purpose: Move pointer past all whitespace
- Algorithm: While current char is whitespace, advance pointer

**Token Creation**:

**create_token(type, value, quote_type)**:
- Purpose: Allocate and initialize token node
- Steps:
  1. Allocate memory for t_token
  2. Set type field
  3. Duplicate value string
  4. Set quote_type
  5. Set next to NULL
  6. Return pointer

**List Management**:

**add_token(list, new_token)**:
- Purpose: Append token to end of list
- Algorithm:
  1. If list empty: new_token becomes head
  2. Otherwise: traverse to end, link new_token

#### Task A3: Word Extraction Logic

**extract_word(input_ptr, quote_type_out)**:
- Purpose: Extract one word token, handling quotes
- Returns: String value of word
- Side effects: Moves input_ptr past extracted word

**Algorithm**:
```
1. Save starting position

2. Check first character:
   
   A. If ' or ":
      - Save quote character
      - Set quote_type (1=single, 2=double)
      - Advance past opening quote
      - Find matching closing quote:
        * Scan until same quote char or end of string
        * If end without closing: SYNTAX ERROR
      - Extract text between quotes (NOT including quotes)
      - Advance past closing quote
   
   B. If regular character:
      - Set quote_type to 0
      - Scan until whitespace or metachar
      - Extract all scanned characters

3. Create string from extracted portion

4. Return string
```

**Examples**:
```
Input: 'hello world' test
Position: ^
Step 1: Detect opening '
Step 2: Find closing ' at position 12
Step 3: Extract "hello world" (11 chars, no quotes)
Step 4: Move position past closing '
Return: "hello world", quote_type=1

Input: test123
Position: ^
Step 1: Not a quote
Step 2: Scan until space/metachar
Step 3: Extract "test123"
Return: "test123", quote_type=0
```

#### Task A4: Operator Extraction Logic

**extract_operator(input_ptr)**:
- Purpose: Extract operator tokens (|, <, >, <<, >>)
- Returns: Token with operator type

**Algorithm**:
```
1. Look at current character:

   If '|':
     - Type: TOKEN_PIPE
     - Value: "|"
     - Advance 1 position
   
   If '<':
     - Look ahead one character:
       * If next is also '<':
         Type: TOKEN_REDIR_HEREDOC
         Value: "<<"
         Advance 2 positions
       * Otherwise:
         Type: TOKEN_REDIR_IN
         Value: "<"
         Advance 1 position
   
   If '>':
     - Look ahead one character:
       * If next is also '>':
         Type: TOKEN_REDIR_APPEND
         Value: ">>"
         Advance 2 positions
       * Otherwise:
         Type: TOKEN_REDIR_OUT
         Value: ">"
         Advance 1 position

2. Create and return token
```

#### Task A5: Main Tokenizer Function

**tokenize(input_string)**:
- Purpose: Convert entire input into token list
- Returns: Pointer to first token (or NULL if empty)

**Algorithm**:
```
1. Initialize empty token list
2. Point to start of input string

3. Loop while not at end of string:
   
   a. Skip any whitespace
   b. If at end of string: break
   
   c. Check current character:
      
      If metacharacter:
        - Extract operator token
        - Add to list
      
      Otherwise (regular character or quote):
        - Extract word token
        - Add to list

4. Return token list
```

**Example Execution**:
```
Input: "ls -la | grep test"

Iteration 1:
  - Skip no whitespace
  - Extract word "ls"
  - Add [WORD, "ls", 0]

Iteration 2:
  - Skip 1 space
  - Extract word "-la"
  - Add [WORD, "-la", 0]

Iteration 3:
  - Skip 1 space
  - Extract operator "|"
  - Add [PIPE, "|", 0]

Iteration 4:
  - Skip 1 space
  - Extract word "grep"
  - Add [WORD, "grep", 0]

Iteration 5:
  - Skip 1 space
  - Extract word "test"
  - Add [WORD, "test", 0]

Result: 5 tokens linked together
```

#### Task A6: Testing Strategy

**Create test program** (`tests/test_tokenizer.c`):

**Test Cases**:
1. Simple command: `echo hello world`
2. With pipes: `ls | grep test`
3. With redirections: `cat < in > out`
4. With quotes: `echo 'single' "double"`
5. Mixed: `cat file1 file2 | grep -i "pattern" >> output.txt`
6. Edge cases:
   - Empty string
   - Only whitespace
   - Unclosed quotes (should error)
   - Multiple spaces: `echo    hello`

**Test Output Format**:
```
For each token, print:
  - Type name (WORD, PIPE, etc.)
  - Value string
  - Quote type
  
Example:
Token 1: WORD "echo" (no quote)
Token 2: WORD "hello world" (double quote)
Token 3: PIPE "|" (no quote)
```

**Verification**:
- Compare with expected output
- Test with valgrind for memory leaks
- Test cleanup (free_tokens function)

---

### PARTNER B - Days 3-4: ENVIRONMENT & BASIC BUILTINS

#### Understanding Environment Variables

**What are environment variables?**
- Key-value pairs passed to programs
- Inherited from parent process
- Can be modified by shell
- Passed to child processes

**Format**: `KEY=VALUE`
- Example: `PATH=/usr/bin:/bin`
- Keys: typically uppercase, alphanumeric + underscore
- Values: any string

**How they work**:
```
Parent Process (bash):
  env = ["PATH=/bin", "HOME=/home/user", ...]
  
Starts Child Process (minishell):
  Child inherits env
  Child can modify its copy
  Changes don't affect parent
  
Child starts its own child (ls):
  ls inherits modified env
```

#### Task B1: Environment List Creation

**create_env_list(envp_array)**:
- Purpose: Convert envp array to linked list
- Input: Array of "KEY=VALUE" strings
- Output: Linked list of t_env nodes

**Why convert to linked list?**
- Easy to add/remove variables
- Easy to search
- Can rebuild array when needed

**Algorithm**:
```
1. Start with empty list

2. For each string in envp:
   a. Find '=' character position
   b. Split string:
      - Everything before '=' is key
      - Everything after '=' is value
   c. Create new t_env node:
      - Allocate memory
      - Copy key
      - Copy value
      - Set next = NULL
   d. Add node to list

3. Return head of list
```

**Example**:
```
Input envp:
envp[0] = "PATH=/usr/bin:/bin"
envp[1] = "HOME=/home/student"
envp[2] = "USER=student"
envp[3] = NULL

Result:
node1: [key="PATH", value="/usr/bin:/bin"] →
node2: [key="HOME", value="/home/student"] →
node3: [key="USER", value="student"] →
NULL
```

#### Task B2: Environment Accessors

**get_env_value(env_list, key)**:
- Purpose: Find value for given key
- Returns: Value string or NULL if not found

**Algorithm**:
```
1. Start at head of list

2. For each node:
   - Compare node->key with search key
   - If match: return node->value
   - Otherwise: move to next node

3. If end of list reached: return NULL
```

**Example**:
```
Search for "HOME":
  Check node1: "PATH" ≠ "HOME" → continue
  Check node2: "HOME" = "HOME" → return "/home/student"
```

**set_env_value(env_list, key, value)**:
- Purpose: Create or update environment variable
- Returns: Success/failure code

**Algorithm**:
```
1. Search for existing key:
   - Traverse list comparing keys
   
2. If key found:
   - Free old value
   - Duplicate new value
   - Store in node
   - Return success
   
3. If key not found:
   - Create new node
   - Set key and value
   - Add to end of list
   - Return success
```

**unset_env_value(env_list, key)**:
- Purpose: Remove environment variable
- Returns: Success/failure code

**Algorithm**:
```
1. Traverse list, tracking previous node

2. For each node:
   - If key matches:
     a. Update previous->next to skip this node
        (or update head if this is first node)
     b. Free node->key
     c. Free node->value
     d. Free node itself
     e. Return success

3. If not found: return failure
```

#### Task B3: Environment Array Conversion

**env_list_to_array(env_list)**:
- Purpose: Convert linked list back to array format for execve()
- Returns: NULL-terminated array of "KEY=VALUE" strings

**Why needed?**
`execve()` system call requires environment as array, not linked list!

**Algorithm**:
```
1. Count nodes in list:
   - Traverse list
   - Increment counter for each node

2. Allocate array:
   - Size: (count + 1) pointers
   - Extra slot for NULL terminator

3. For each node in list:
   a. Create "KEY=VALUE" string:
      - Concatenate key + "=" + value
   b. Store in array
   c. Move to next node

4. Set last array element to NULL

5. Return array
```

**Example**:
```
Input List:
[PATH=/bin] → [HOME=/home/user] → NULL

Step 1: Count = 2
Step 2: Allocate array[3]
Step 3: 
  array[0] = "PATH=/bin"
  array[1] = "HOME=/home/user"
Step 4:
  array[2] = NULL

Return: array
```

#### Task B4: PWD Builtin

**Purpose**: Print current working directory

**Algorithm**:
```
1. Call getcwd() system function:
   - Provide buffer for path
   - Size: PATH_MAX (usually 4096)

2. If getcwd succeeds:
   - Print directory path
   - Print newline
   - Return 0 (success)

3. If getcwd fails:
   - Print error message
   - Return 1 (failure)
```

**System Function**: `getcwd(buffer, size)`
- Returns: Pointer to buffer on success, NULL on error
- Fills buffer with current directory path

**Test Cases**:
```bash
$ pwd
/home/user/project

$ cd /tmp
$ pwd
/tmp

$ cd nonexistent
cd: nonexistent: No such file or directory
$ pwd
/tmp  (still in /tmp, cd failed)
```

#### Task B5: ENV Builtin

**Purpose**: Print all environment variables

**Algorithm**:
```
1. Start at head of environment list

2. For each node:
   - Print key
   - Print "="
   - Print value
   - Print newline
   - Move to next node

3. Return 0 (success)
```

**Output Format**: `KEY=VALUE`

**Test**:
```bash
$ env
PATH=/usr/bin:/bin
HOME=/home/user
USER=student
SHELL=/bin/bash
...
```

#### Task B6: EXIT Builtin

**Purpose**: Exit shell with specified code

**Algorithm**:
```
1. Print "exit" message

2. Check number of arguments:
   
   No arguments:
     - Exit with last_exit_status from shell

   One argument:
     a. Check if numeric:
        - If not: Print error, exit with 2
        - If yes: Convert to number, exit with that code
   
   Two or more arguments:
     - Print "too many arguments" error
     - Return 1 (DON'T exit, just return to prompt)

3. Call cleanup_shell() to free memory

4. Call exit(code)
```

**Numeric Check**:
```
Valid: "0", "123", "-42", "+5"
Invalid: "abc", "12abc", ""
```

**Test Cases**:
```bash
$ exit
exit
(shell closes with code 0)

$ exit 42
exit
(shell closes with code 42)

$ echo $?  # in parent shell
42

$ exit abc
exit
minishell: exit: abc: numeric argument required
(shell closes with code 2)

$ exit 1 2 3
exit
minishell: exit: too many arguments
minishell$  (shell stays open)
```

#### Task B7: Environment Testing

**Test Program Requirements**:
1. Create env list from system envp
2. Test get: retrieve PATH, HOME, USER
3. Test set: create new variable MY_VAR
4. Test update: change MY_VAR value
5. Test unset: remove MY_VAR
6. Test array conversion
7. Verify no memory leaks with valgrind

**Example Test Output**:
```
=== Test 1: Create Environment ===
Created list with 50 variables

=== Test 2: Get Values ===
PATH=/usr/bin:/bin
HOME=/home/user

=== Test 3: Set New Variable ===
MY_VAR=initial_value

=== Test 4: Update Variable ===
MY_VAR=updated_value

=== Test 5: Unset Variable ===
MY_VAR not found (correct)

=== Test 6: Array Conversion ===
envp[0]=PATH=/usr/bin:/bin
envp[1]=HOME=/home/user
...
envp[49]=NULL

Memory check: PASSED (no leaks)
```

---

## PHASE 2: PARSING (Days 5-10)

### Goals:
- Convert tokens into command structures
- Implement remaining builtins
- Handle syntax errors

### PARTNER A - Days 5-7: PARSER

#### Understanding Parsing

**What is parsing?**
Converting flat token list into hierarchical command structure

**Example**:
```
Tokens:
[cat] [file1] [file2] [|] [grep] [pattern] [>] [output.txt]

Parsed Commands:
Command 1:
  args: ["cat", "file1", "file2", NULL]
  redirects: NULL
  next → Command 2

Command 2:
  args: ["grep", "pattern", NULL]
  redirects: [REDIR_OUT, "output.txt"]
  next → NULL
```

#### Task A7: Parser Infrastructure

**Parser State Machine**:
- Tracks current position in token list
- Detects syntax errors
- Builds command structures

**init_parser(tokens, shell)**:
- Create t_parser structure
- Set current to first token
- Initialize error state
- Return parser

**Navigation Functions**:

**peek()**:
- Return type of current token
- Don't advance
- Used for lookahead decisions

**advance()**:
- Move to next token
- Used after processing current token

**expect(type)**:
- Check if current token matches expected type
- If not: set error flag
- Used for validation

**Example Usage**:
```
Parser at: [>] [file.txt] [|] [grep]
            ^

peek() → TOKEN_REDIR_OUT
advance()

Parser at: [>] [file.txt] [|] [grep]
                ^

peek() → TOKEN_WORD
expect(TOKEN_WORD) → success
value = "file.txt"
advance()

Parser at: [>] [file.txt] [|] [grep]
                            ^
```

#### Task A8: Redirection Parsing

**parse_redirection()**:
- Purpose: Extract one redirection from tokens
- Returns: t_redirect node

**Algorithm**:
```
1. Check current token type:
   - TOKEN_REDIR_IN → REDIR_IN
   - TOKEN_REDIR_OUT → REDIR_OUT
   - TOKEN_REDIR_APPEND → REDIR_APPEND
   - TOKEN_REDIR_HEREDOC → REDIR_HEREDOC
   - Other → return NULL (not a redirection)

2. Save redirection type

3. Advance to next token

4. Expect TOKEN_WORD (filename/delimiter):
   - If not WORD: SYNTAX ERROR "expected filename"
   - Extract filename from token value

5. Create t_redirect node:
   - Set type field
   - Copy filename
   - Set fd = -1 (not opened yet)
   - Set next = NULL

6. Advance past filename

7. Return redirection node
```

**Example**:
```
Tokens: [>] [output.txt] [more tokens...]
        ^

Step 1: Type = TOKEN_REDIR_OUT → REDIR_OUT
Step 2: Saved
Step 3: Move to [output.txt]
Step 4: Is WORD ✓, extract "output.txt"
Step 5: Create node [type=REDIR_OUT, file="output.txt", fd=-1]
Step 6: Move past "output.txt"
Step 7: Return node
```

#### Task A9: Simple Command Parsing

**parse_simple_command()**:
- Purpose: Parse one command with args and redirections
- Returns: t_command node

**Algorithm**:
```
1. Create t_command structure
   - Initialize all fields to NULL/0

2. Loop while tokens are WORD or REDIRECTION:
   
   A. If current token is WORD:
      - First time: Count total words, allocate args array
      - Add word to args array
      - Increment arg counter
      - Advance token
   
   B. If current token is REDIRECTION:
      - Parse redirection (get t_redirect node)
      - Add to redirections list
      - Advance handled by parse_redirection

   C. Otherwise (pipe or end):
      - Break loop

3. Validate:
   - If no arguments: SYNTAX ERROR "empty command"

4. NULL-terminate args array

5. Return command
```

**Complexity**: Commands can have redirections and arguments in any order!

**Example**:
```
Tokens: [cat] [<] [input.txt] [file1] [file2] [>] [output.txt]

Iteration 1: cat is WORD
  - Allocate args array (size from scanning ahead)
  - args[0] = "cat"

Iteration 2: < is REDIRECTION
  - Parse: [REDIR_IN, "input.txt"]
  - Add to redirections list

Iteration 3: file1 is WORD
  - args[1] = "file1"

Iteration 4: file2 is WORD
  - args[2] = "file2"

Iteration 5: > is REDIRECTION
  - Parse: [REDIR_OUT, "output.txt"]
  - Add to redirections list

Iteration 6: No more tokens or hit pipe
  - Break loop

Result:
  args: ["cat", "file1", "file2", NULL]
  redirects: [IN, "input.txt"] → [OUT, "output.txt"] → NULL
```

#### Task A10: Pipeline Parsing

**parse_pipeline()**:
- Purpose: Parse commands separated by pipes
- Returns: Head of command linked list

**Algorithm**:
```
1. Parse first simple command
   - If fails: return NULL

2. Save as head and current

3. Loop while current token is PIPE:
   a. Advance past pipe token
   b. Parse next simple command
      - If fails: free all commands, return NULL
   c. Link: current->next = new_command
   d. Update current = new_command

4. Return head of command list
```

**Example**:
```
Tokens: [ls] [-la] [|] [grep] [test] [|] [wc] [-l]

Step 1: Parse "ls -la"
  cmd1 = [args: ["ls", "-la"], next: NULL]
  head = cmd1
  current = cmd1

Step 2: Token is PIPE
  Step 2a: Advance past |
  Step 2b: Parse "grep test"
    cmd2 = [args: ["grep", "test"], next: NULL]
  Step 2c: cmd1->next = cmd2
  Step 2d: current = cmd2

Step 3: Token is PIPE
  Step 3a: Advance past |
  Step 3b: Parse "wc -l"
    cmd3 = [args: ["wc", "-l"], next: NULL]
  Step 3c: cmd2->next = cmd3
  Step 3d: current = cmd3

Step 4: No more pipes
  Return head (cmd1)

Result: cmd1 → cmd2 → cmd3 → NULL
```

#### Task A11: Main Parser Entry

**parse_tokens(tokens, shell)**:
- Purpose: Main parsing function called from main loop
- Returns: Command list or NULL on error

**Algorithm**:
```
1. Check if tokens is NULL:
   - If yes: return NULL

2. Initialize parser

3. Parse pipeline (gets command list)

4. Check parser error flag:
   - If error:
     * Print error message
     * Free any partial commands
     * Set commands to NULL

5. Free parser structure

6. Return commands (or NULL if error)
```

**Error Handling Examples**:
```
Input: "cat <"
Error: "expected filename after <"

Input: "|grep"
Error: "unexpected token |"

Input: "cat < < file"
Error: "syntax error near <"
```

#### Task A12: Parser Testing

**Test Strategy**:
Create test program that:
1. Tokenizes input
2. Parses tokens
3. Prints resulting command structures
4. Checks for errors
5. Frees all memory

**Test Cases**:

1. **Simple command**:
   Input: `echo hello world`
   Expected: 1 command, 3 args, no redirects

2. **Command with redirections**:
   Input: `cat < in.txt > out.txt`
   Expected: 1 command, 1 arg, 2 redirects

3. **Pipeline**:
   Input: `ls -la | grep test`
   Expected: 2 commands, 2 args each, no redirects

4. **Complex**:
   Input: `cat file1 file2 | grep -i pattern >> output.txt`
   Expected: 2 commands, command2 has 1 redirect

5. **Error cases**:
   - `cat <` (missing filename)
   - `| grep` (pipe at start)
   - `` (empty command)

**Verification**:
- Print each command's args array
- Print each command's redirections
- Confirm proper linking (next pointers)
- Run valgrind to check leaks

---

### PARTNER B - Days 5-7: REMAINING BUILTINS

#### Task B8: CD Builtin

**Purpose**: Change current working directory

**Behavior Requirements**:
```
cd            → Go to HOME directory
cd <path>     → Go to specified path
cd path1 path2→ Error: too many arguments
```

**Algorithm**:
```
1. Check argument count:
   - If > 1: Print error, return 1
   
2. Determine target path:
   - If no arg: Get HOME from environment
     * If HOME not set: Print error, return 1
   - If one arg: Use that path

3. Get current directory (for OLDPWD):
   - Call getcwd()
   - Save result

4. Attempt to change directory:
   - Call chdir(target_path)
   - If fails: Print error with reason, return 1

5. Update environment variables:
   - Set OLDPWD = old directory
   - Get new current directory (getcwd)
   - Set PWD = new directory

6. Rebuild envp array (for execve)

7. Return 0 (success)
```

**System Functions**:
- `getcwd(buffer, size)` - Get current directory
- `chdir(path)` - Change directory

**Edge Cases**:
- Path doesn't exist
- No permission to enter directory
- HOME not set
- Path is actually a file
- Symlinks (follow them)

**Test Cases**:
```bash
$ pwd
/home/user

$ cd /tmp
$ pwd
/tmp

$ cd
$ pwd
/home/user  # Back to HOME

$ cd project minishell
minishell: cd: too many arguments

$ cd /root
minishell: cd: /root: Permission denied

$ cd nonexistent
minishell: cd: nonexistent: No such file or directory
```

#### Task B9: ECHO Builtin

**Purpose**: Print arguments to stdout

**Special Features**:
- `-n` flag: Don't print trailing newline
- Handles quotes (already processed by tokenizer)

**Algorithm**:
```
1. Initialize:
   - newline = true
   - start_index = 1 (first argument)

2. Check for -n flag:
   - If args[1] equals "-n":
     * Set newline = false
     * Set start_index = 2

3. Print arguments:
   - For each arg from start_index to end:
     * Print the argument
     * If not last arg: Print space

4. If newline is true:
   - Print newline character

5. Return 0 (success)
```

**Important**: Tokenizer already removed quotes, so echo just prints values!

**Test Cases**:
```bash
$ echo hello world
hello world

$ echo -n test
test$ (no newline)

$ echo "hello    world"
hello    world  (spaces from original preserved)

$ echo '$USER'
$USER  (single quotes prevent expansion)

$ echo "$USER"
student  (double quotes allow expansion)

$ echo
 (just newline)

$ echo -n
$ (no output at all)
```

#### Task B10: EXPORT Builtin

**Purpose**: Set or display environment variables

**Behaviors**:
```
export           → Display all variables
export VAR=value → Set/update variable
export VAR       → Declare variable (bash compatibility)
export VAR=val1 VAR2=val2 → Multiple sets
```

**Algorithm**:
```
1. Check if any arguments:
   - If no args:
     * Display all env variables
     * Format: declare -x KEY="VALUE"
     * Return 0

2. For each argument:
   
   a. Validate identifier:
      - Must start with letter or underscore
      - Can contain letters, digits, underscore
      - If invalid: Print error, set return code to 1, continue
   
   b. Find '=' sign:
      - If found:
        * Split into key (before =) and value (after =)
        * Call set_env_value(key, value)
      - If not found:
        * Just declare variable (no value set)
        * Bash compatibility feature

3. Rebuild envp array (environment changed!)

4. Return exit code (0 if all valid, 1 if any errors)
```

**Validation Examples**:
```
Valid identifiers:
  PATH, HOME, USER, MY_VAR, _test, VAR123

Invalid identifiers:
  123VAR (starts with digit)
  MY-VAR (contains hyphen)
  "" (empty)
```

**Test Cases**:
```bash
$ export
declare -x HOME="/home/user"
declare -x PATH="/usr/bin:/bin"
...

$ export MY_VAR=hello
$ env | grep MY_VAR
MY_VAR=hello

$ export MY_VAR=world
$ env | grep MY_VAR
MY_VAR=world

$ export TEST1=val1 TEST2=val2
$ env | grep TEST
TEST1=val1
TEST2=val2

$ export 123=invalid
minishell: export: `123=invalid': not a valid identifier

$ export PATH=$PATH:/new/path
# (Note: $PATH expansion happens before export sees it)
```

#### Task B11: UNSET Builtin

**Purpose**: Remove environment variables

**Algorithm**:
```
1. Check if any arguments:
   - If none: Return 0 (nothing to do)

2. For each argument:
   
   a. Validate identifier:
      - Same rules as export
      - If invalid: Print error, set return code to 1, continue
   
   b. Call unset_env_value(key):
      - Remove from environment list
      - Doesn't error if variable doesn't exist

3. Rebuild envp array

4. Return exit code
```

**Test Cases**:
```bash
$ export TEST=value
$ env | grep TEST
TEST=value

$ unset TEST
$ env | grep TEST
(nothing - variable removed)

$ unset TEST
(no error, even though TEST doesn't exist)

$ unset PATH HOME USER
(removes multiple variables)

$ unset 123
minishell: unset: `123': not a valid identifier

$ unset _valid VAR123
(works fine)
```

#### Task B12: Comprehensive Builtin Testing

**Test Script** (`tests/test_builtins.sh`):
```bash
#!/bin/bash

echo "=== Testing PWD ==="
pwd
cd /tmp
pwd
cd -  # back to previous (uses OLDPWD)
pwd

echo "=== Testing CD ==="
cd /
pwd
cd ~
pwd
cd project
pwd
cd ..
pwd

echo "=== Testing ECHO ==="
echo hello world
echo -n test
echo
echo "quoted string"
echo '$USER is' $USER

echo "=== Testing ENV ==="
env | head -5

echo "=== Testing EXPORT ==="
export TEST=value
env | grep TEST
export TEST=new_value
env | grep TEST
export

echo "=== Testing UNSET ==="
unset TEST
env | grep TEST

echo "=== Testing EXIT ==="
exit 42
```

Run in both bash and minishell, compare outputs!

---

## PHASE 3: EXECUTION (Days 8-14)

### PARTNER A - Days 8-10: SIGNALS & BASIC EXECUTION

#### Understanding Signals

**What are signals?**
- Asynchronous notifications sent to processes
- Triggered by keyboard or system events
- Process can handle, ignore, or use default behavior

**Key Signals for Minishell**:
- `SIGINT` (signal 2): Ctrl-C, interrupt
- `SIGQUIT` (signal 3): Ctrl-\, quit with core dump
- EOF (Ctrl-D): Not a signal, handled by readline

**Signal Handling Modes**:

1. **Interactive Mode** (waiting for input):
   - Ctrl-C: Display new prompt on new line
   - Ctrl-\: Do nothing (ignore)
   - Ctrl-D: Exit shell

2. **Executing Command**:
   - Ctrl-C: Terminate child process, return to prompt
   - Ctrl-\: Terminate child with quit message

#### Task A13: Signal Handler Implementation

**setup_signals()**:
- Purpose: Configure signal handlers for interactive mode
- Called once during shell initialization

**Algorithm**:
```
1. Create sigaction structure for SIGINT:
   - Set handler function to handle_sigint
   - Empty signal mask (don't block other signals)
   - Set SA_RESTART flag (restart system calls)
   
2. Register SIGINT handler:
   - Call sigaction(SIGINT, ...)

3. Create sigaction structure for SIGQUIT:
   - Set handler to SIG_IGN (ignore signal)
   - Empty signal mask
   - No special flags

4. Register SIGQUIT handler:
   - Call sigaction(SIGQUIT, ...)
```

**handle_sigint(sig)**:
- Purpose: Handle Ctrl-C in interactive mode
- Called automatically when SIGINT received

**Algorithm**:
```
1. Set global: g_signal = SIGINT

2. Print newline (move to new line)

3. Update readline state:
   - Call rl_on_new_line() - notify readline of new line
   - Call rl_replace_line("", 0) - clear current input
   - Call rl_redisplay() - redraw prompt

Purpose: Appears to user as if they pressed Enter, got new prompt
```

**setup_signals_child()**:
- Purpose: Reset signals to default in child processes
- Called after fork, before execve

**Algorithm**:
```
1. Create sigaction structure:
   - Set handler to SIG_DFL (default)
   - Empty signal mask
   - No flags

2. Apply to both SIGINT and SIGQUIT
```

**Why reset in child?**
Child processes should respond to Ctrl-C normally (terminate), not show new prompt!

**Signal Flow Example**:
```
User types: cat
Shell forks
Child: Resets signals to default
Child: Executes cat (waiting for input)
User presses Ctrl-C
SIGINT sent to child
Child terminates (default behavior)
Parent: Detects child terminated
Parent: Returns to prompt
```

#### Task A14: PATH Resolution

**Understanding PATH**:
```
PATH=/usr/bin:/bin:/usr/local/bin

Means: Search these directories in order
1. /usr/bin
2. /bin
3. /usr/local/bin
```

**When command is `ls`**:
```
Not a path (no /) → search PATH
1. Try /usr/bin/ls → Check if exists and executable
2. If found → use /usr/bin/ls
3. If not → try /bin/ls
4. If found → use /bin/ls
5. If none found → command not found error
```

**When command is `/bin/ls`**:
```
Contains / → treat as path
Don't search PATH
Just check if /bin/ls exists and is executable
```

**resolve_path(cmd, shell)**:
- Purpose: Find full path to executable
- Returns: Full path string or NULL if not found

**Algorithm**:
```
1. Check if cmd contains '/':
   YES (is a path):
     - Check if file exists and is executable
     - Return path if yes, NULL if no
   
   NO (is a command name):
     - Continue to PATH search

2. Get PATH from environment:
   - Call get_env_value(env_list, "PATH")
   - If PATH not set: return NULL

3. Split PATH by ':' delimiter:
   - Use ft_split(path_value, ':')
   - Result: array of directory paths

4. For each directory in PATH:
   a. Construct full path: directory + "/" + cmd
      Example: "/usr/bin" + "/" + "ls" = "/usr/bin/ls"
   b. Check if exists and executable:
      - Use access(path, X_OK)
   c. If yes:
      - Free directory array
      - Return full path
   d. If no:
      - Free this path
      - Continue to next directory

5. If not found in any directory:
   - Free directory array
   - Return NULL
```

**System Function**:
`access(path, X_OK)` - Check if file exists and is executable
- Returns: 0 if yes, -1 if no

**Example Execution**:
```
cmd = "grep"
PATH = "/usr/bin:/bin"

Step 1: No '/' in "grep" → search PATH
Step 2: PATH = "/usr/bin:/bin"
Step 3: Split → ["/usr/bin", "/bin"]
Step 4a: Try "/usr/bin/grep"
  access("/usr/bin/grep", X_OK) → 0 (exists!)
  Return "/usr/bin/grep"
```

#### Task A15: Command Execution

**execute_single_command(cmd, shell)**:
- Purpose: Execute one command (no pipes yet)
- Returns: Exit status code

**High-Level Flow**:
```
1. Check if builtin
   YES → Execute builtin, return exit code
   NO → Execute external command

2. Update shell->last_exit_status

3. Return exit code
```

**Detailed Algorithm for External Commands**:
```
1. Resolve command path:
   - Call resolve_path(cmd->args[0], shell)
   - If NULL:
     * Print "command not found"
     * Return 127 (command not found code)

2. Create child process:
   - Call fork()
   - If fork fails: Print error, return 1
   
3. In CHILD process (fork returned 0):
   a. Reset signal handlers (setup_signals_child)
   b. Apply redirections (if any)
   c. Execute command:
      - Call execve(path, args, envp)
      - If execve returns (error):
        * Print error
        * Exit with code 126 (permission/execution error)
   
4. In PARENT process (fork returned child PID):
   a. Free resolved path
   b. Wait for child:
      - Call waitpid(pid, &status, 0)
   c. Extract exit status:
      - If normal exit: WEXITSTATUS(status)
      - If killed by signal: 128 + signal number
   d. Return exit status
```

**Builtin Execution**:
```
1. Call appropriate builtin function based on command name

2. Builtins run in parent process (no fork)
   - Why? cd, export, unset must modify shell state

3. Need to handle redirections carefully:
   - Save stdin/stdout
   - Apply redirections
   - Execute builtin
   - Restore stdin/stdout

4. Return builtin's exit code
```

**Example Flow**:
```
Command: ["ls", "-la", NULL]

Step 1: Not a builtin
Step 2: resolve_path("ls") → "/bin/ls"
Step 3: fork() → creates child

CHILD PROCESS:
  - Reset signals
  - execve("/bin/ls", ["ls", "-la", NULL], envp)
  - (execve doesn't return - becomes ls process)
  - ls runs, prints output, exits with 0

PARENT PROCESS:
  - waitpid(child_pid, &status)
  - Child exits with 0
  - Extract status: 0
  - Return 0
```

#### Task A16: Basic Testing

**Test Cases** (no pipes yet):

1. **Builtins**:
```bash
pwd
echo hello
cd /tmp
pwd
```

2. **External Commands**:
```bash
ls
ls -la
cat Makefile
/bin/echo test
```

3. **PATH Resolution**:
```bash
ls          # Should find /bin/ls
/bin/ls     # Explicit path
./minishell # Relative path
```

4. **Error Cases**:
```bash
nonexistent_command
/tmp/not_a_file
/etc/passwd  # File, not executable
```

5. **Exit Codes**:
```bash
ls
echo $?  # Should be 0

ls /nonexistent 2>/dev/null
echo $?  # Should be non-zero

false
echo $?  # Should be 1
```

---

### PARTNER B - Days 8-10: REDIRECTIONS

#### Understanding Redirections

**What are file descriptors?**
- Integer handles to open files
- Standard file descriptors:
  - 0 = stdin (standard input)
  - 1 = stdout (standard output)
  - 2 = stderr (standard error)

**What is redirection?**
Changing where input comes from or output goes to

**Types**:

1. **Input Redirection** `<`:
   ```bash
   cat < file.txt
   # stdin (FD 0) reads from file.txt instead of keyboard
   ```

2. **Output Redirection** `>`:
   ```bash
   ls > output.txt
   # stdout (FD 1) writes to output.txt instead of terminal
   # File is created or truncated
   ```

3. **Append Redirection** `>>`:
   ```bash
   echo "log" >> log.txt
   # stdout writes to end of log.txt
   # File is created or appended to
   ```

4. **Here-document** `<<`:
   ```bash
   cat << EOF
   line 1
   line 2
   EOF
   # stdin reads from the lines until EOF is seen
   ```

#### Task B7: Redirection Implementation

**File Opening Functions**:

**open_redir_in(filename)**:
- Purpose: Open file for reading
- Returns: File descriptor or -1 on error

**Algorithm**:
```
1. Open file in read-only mode:
   - Call open(filename, O_RDONLY)
   - Flags: O_RDONLY (read only)

2. If open fails:
   - Print error with filename
   - Return -1

3. Return file descriptor
```

**open_redir_out(filename)**:
- Purpose: Open file for writing (truncate)
- Returns: File descriptor or -1 on error

**Algorithm**:
```
1. Open file with flags:
   - O_WRONLY (write only)
   - O_CREAT (create if doesn't exist)
   - O_TRUNC (truncate to zero length)
   - Mode: 0644 (rw-r--r--)

2. If open fails:
   - Print error
   - Return -1

3. Return file descriptor
```

**open_redir_append(filename)**:
- Purpose: Open file for appending
- Returns: File descriptor or -1 on error

**Algorithm**:
```
1. Open file with flags:
   - O_WRONLY (write only)
   - O_CREAT (create if doesn't exist)
   - O_APPEND (write at end)
   - Mode: 0644

2. If open fails:
   - Print error
   - Return -1

3. Return file descriptor
```

**Heredoc Implementation**:

**handle_heredoc(delimiter)**:
- Purpose: Read lines until delimiter, provide as input
- Returns: Read end of pipe with the content

**Algorithm**:
```
1. Create pipe:
   - Call pipe(pipe_fds)
   - pipe_fds[0] = read end
   - pipe_fds[1] = write end
   - If fails: return -1

2. Loop reading lines:
   a. Display prompt: "> "
   b. Read line from user (readline)
   c. If EOF (Ctrl-D):
      - Print warning
      - Break loop
   d. If line equals delimiter:
      - Free line
      - Break loop
   e. Otherwise:
      - Write line to pipe write end
      - Write newline to pipe
      - Free line

3. Close write end of pipe

4. Return read end
```

**Why pipe?**
Heredoc content needs to be available as stdin, but it's not a file. Pipe provides a file descriptor that command can read from.

**Example**:
```bash
cat << END
hello
world
END

Heredoc process:
1. Create pipe
2. Prompt: "> "
   User types: hello
   Write "hello\n" to pipe
3. Prompt: "> "
   User types: world
   Write "world\n" to pipe
4. Prompt: "> "
   User types: END
   Match delimiter → stop
5. Close write end
6. Return read end → becomes stdin for cat
7. cat reads "hello\nworld\n" from stdin
```

#### Task B8: Applying Redirections

**apply_redirections(redirects)**:
- Purpose: Apply all redirections for a command
- Returns: 0 on success, -1 on error

**Algorithm**:
```
1. Loop through redirection list:

   For each redirection:
   
   A. REDIR_IN:
      - Open file for reading
      - If fails: return -1
      - Duplicate FD to stdin:
        * dup2(fd, STDIN_FILENO)
        * This makes stdin read from file
      - Close original FD (dup2 made a copy)
   
   B. REDIR_OUT:
      - Open file for writing (truncate)
      - If fails: return -1
      - Duplicate FD to stdout:
        * dup2(fd, STDOUT_FILENO)
      - Close original FD
   
   C. REDIR_APPEND:
      - Open file for appending
      - If fails: return -1
      - Duplicate FD to stdout
      - Close original FD
   
   D. REDIR_HEREDOC:
      - Handle heredoc (get pipe read end)
      - If fails: return -1
      - Duplicate to stdin
      - Close original FD

2. Return 0 (success)
```

**System Function**: `dup2(oldfd, newfd)`
- Duplicates oldfd to newfd
- Closes newfd first if open
- Makes newfd a copy of oldfd

**Example**:
```
Command: cat < input.txt > output.txt

Initial state:
  FD 0 (stdin)  → terminal
  FD 1 (stdout) → terminal

After input redirection:
  Open "input.txt" → FD 3
  dup2(3, 0) → FD 0 now reads from input.txt
  close(3)

After output redirection:
  Open "output.txt" → FD 3
  dup2(3, 1) → FD 1 now writes to output.txt
  close(3)

Now cat reads from input.txt, writes to output.txt
```

#### Task B9: Builtin Redirection Handling

**Problem**: Builtins run in parent process
- If we redirect stdout, parent's stdout changes!
- Need to restore after builtin executes

**Solution**: Save and restore file descriptors

**Algorithm for Builtin with Redirections**:
```
1. Save original stdin/stdout:
   - stdin_backup = dup(STDIN_FILENO)
   - stdout_backup = dup(STDOUT_FILENO)

2. Apply redirections:
   - Call apply_redirections(cmd->redirects)
   - If fails:
     * Restore FDs
     * Return error

3. Execute builtin:
   - Call builtin function
   - Get exit code

4. Restore original stdin/stdout:
   - dup2(stdin_backup, STDIN_FILENO)
   - dup2(stdout_backup, STDOUT_FILENO)
   - close(stdin_backup)
   - close(stdout_backup)

5. Return exit code
```

**Example**:
```bash
echo hello > file.txt

Without restoration:
  1. Redirect stdout to file.txt
  2. echo writes to file
  3. Shell's stdout still points to file!
  4. Prompt goes to file instead of terminal ❌

With restoration:
  1. Save original stdout → FD 10
  2. Redirect stdout to file.txt
  3. echo writes to file
  4. Restore stdout from FD 10
  5. Prompt displays normally ✓
```

#### Task B10: Integration with Executor

**Modify execute_single_command**:

For external commands:
```
In child process:
  - Apply redirections
  - execve (doesn't return)
  
No restoration needed (child process exits)
```

For builtins:
```
In parent process:
  - Save FDs
  - Apply redirections
  - Execute builtin
  - Restore FDs
```

#### Task B11: Testing Redirections

**Test Cases**:

1. **Input Redirection**:
```bash
cat < Makefile
wc -l < Makefile
```

2. **Output Redirection**:
```bash
echo hello > test.txt
cat test.txt  # Should show "hello"

ls -la > files.txt
cat files.txt  # Should show directory listing
```

3. **Append Redirection**:
```bash
echo line1 > test.txt
echo line2 >> test.txt
cat test.txt  # Should show both lines
```

4. **Multiple Redirections**:
```bash
cat < input.txt > output.txt
# Read from input.txt, write to output.txt
```

5. **Heredoc**:
```bash
cat << EOF
test line 1
test line 2
EOF
# Should print both lines

wc -l << END
line 1
line 2
line 3
END
# Should print "3"
```

6. **Builtin Redirections**:
```bash
pwd > dir.txt
cat dir.txt

echo test > out.txt
cat out.txt
```

7. **Error Cases**:
```bash
cat < nonexistent.txt
# Error: No such file or directory

cat > /root/test.txt
# Error: Permission denied
```

---

## PHASE 4: PIPES (Days 11-14)

### Understanding Pipes

**What is a pipe?**
- Connects output of one command to input of another
- Creates a unidirectional data channel
- Implemented with pipe() system call

**How pipes work**:
```
Command: ls | grep test

Creates pipe with two ends:
  pipe[0] - Read end
  pipe[1] - Write end

Process 1 (ls):
  - Writes to pipe[1]
  - Output goes into pipe

Process 2 (grep):
  - Reads from pipe[0]
  - Input comes from pipe

Data flows: ls → pipe → grep
```

**System Call**: `pipe(int fds[2])`
- Creates pipe
- fds[0] = read end
- fds[1] = write end
- Returns 0 on success, -1 on error

### BOTH PARTNERS - Days 11-14: PIPELINE EXECUTION

#### Task: Pipeline Implementation

**execute_pipeline(shell, commands)**:
- Purpose: Execute chain of commands connected by pipes
- Returns: Exit status of last command

**High-Level Strategy**:
```
For N commands:
  - Create N processes
  - Create N-1 pipes
  - Connect processes through pipes
  - Wait for all to complete
```

**Detailed Algorithm**:
```
1. Count commands in chain:
   - Traverse linked list
   - Save count

2. Allocate arrays:
   - child_pids[count] for process IDs
   - pipe_fds[count-1][2] for pipes

3. For each command (index 0 to count-1):
   
   a. Create pipe (if not last command):
      - If index < count-1:
        * pipe(pipe_fds[index])
      
   b. Fork process:
      - pid = fork()
      - If fork fails: cleanup and return error
      
   c. If child process (pid == 0):
      
      i. Reset signals
      
      ii. Handle input:
          - If first command (index == 0):
            * Input from stdin (no change needed)
          - If not first:
            * Input from previous pipe:
              dup2(pipe_fds[index-1][0], STDIN_FILENO)
      
      iii. Handle output:
           - If last command:
             * Output to stdout (no change needed)
           - If not last:
             * Output to current pipe:
               dup2(pipe_fds[index][1], STDOUT_FILENO)
      
      iv. Close all pipe FDs:
          - For j = 0 to count-2:
            * close(pipe_fds[j][0])
            * close(pipe_fds[j][1])
          - Must close unused pipe ends!
      
      v. Apply redirections (if any)
      
      vi. Execute command:
          - If builtin: execute builtin, exit with code
          - If external: execve, doesn't return
      
   d. If parent process:
      - Save child PID: child_pids[index] = pid
      - Close used pipe ends:
        * If not first: close(pipe_fds[index-1][0])
        * If not last: (don't close write end yet)

4. Parent closes all remaining pipe FDs:
   - Close final pipe write end if exists

5. Wait for all children:
   - For each child_pid:
     * waitpid(pid, &status, 0)
     * Extract exit status
   - Save last child's exit status

6. Cleanup:
   - Free child_pids array
   - Free pipe_fds array

7. Return last exit status
```

**Example Execution** (`ls | grep test | wc -l`):

```
Setup:
  3 commands
  2 pipes needed
  
  pipe1: ls → grep
  pipe2: grep → wc

Fork Process 1 (ls):
  Index 0, not last
  Create pipe1[0,1]
  Fork:
    Child:
      Input: stdin (first command)
      Output: pipe1[1]
      Close: pipe1[0], pipe2[0], pipe2[1]
      Execute: ls
    Parent:
      Save PID1
      (Don't close pipe1 yet, grep needs it)

Fork Process 2 (grep):
  Index 1, not last
  Create pipe2[0,1]
  Fork:
    Child:
      Input: pipe1[0] (from ls)
      Output: pipe2[1]
      Close: pipe1[1], pipe2[0]
      Execute: grep test
    Parent:
      Save PID2
      Close: pipe1[0] (grep has it now)

Fork Process 3 (wc):
  Index 2, last
  No new pipe
  Fork:
    Child:
      Input: pipe2[0] (from grep)
      Output: stdout (last command)
      Close: pipe2[1]
      Execute: wc -l
    Parent:
      Save PID3
      Close: pipe2[0]

Parent waits:
  wait(PID1) → ls exits
  wait(PID2) → grep exits
  wait(PID3) → wc exits with 0
  Return: 0
```

**Critical Points**:

1. **Close Unused FDs**:
   - Each process must close pipe ends it doesn't use
   - Otherwise: pipe never gets EOF signal
   - Result: Commands hang waiting for input

2. **Pipe Creation Timing**:
   - Create pipe before forking
   - Both parent and child have access

3. **Wait Order**:
   - Can wait for children in any order
   - Or wait for all together
   - Must wait for all to prevent zombies

#### Testing Pipelines

**Test Cases**:

1. **Two Commands**:
```bash
ls | wc -l
echo hello | cat
```

2. **Three Commands**:
```bash
ls -la | grep test | wc -l
cat file | grep pattern | sort
```

3. **Many Commands**:
```bash
cat /etc/passwd | grep root | cut -d: -f1 | sort | uniq
```

4. **With Redirections**:
```bash
cat < input.txt | grep test > output.txt
ls | grep .c > c_files.txt
```

5. **Builtins in Pipeline**:
```bash
echo hello | cat
pwd | cat
```

6. **Error Handling**:
```bash
nonexistent | cat
cat | nonexistent
ls | grep | wc  # Empty grep pattern
```

**Verification**:
- Compare output with bash
- Check exit codes
- Test with valgrind (check FD leaks)
- Test signal handling during pipeline

---

## PHASE 5: EXPANSION (Days 15-18)

### Understanding Variable Expansion

**What is expansion?**
Replacing variables with their values before execution

**Examples**:
```bash
echo $HOME
# $HOME replaced with "/home/user"
# Becomes: echo /home/user

echo "User: $USER"
# $USER replaced with "student"
# Becomes: echo "User: student"

echo '$USER'
# Single quotes: no expansion
# Stays: echo $USER
```

### PARTNER A - Days 15-16: Variable Expansion

#### Task A17: Identify Variables in Tokens

**find_variable(token_value, start_pos)**:
- Purpose: Find next $VAR in string
- Returns: Position and length of variable name

**Algorithm**:
```
1. Search for '$' character from start_pos

2. If not found: return NULL

3. Found '$' - check what follows:
   
   a. If next char is '?':
      - Variable is "$?"
      - Return position and length=1
   
   b. If next char is digit:
      - Not a valid variable
      - Continue search after this position
   
   c. If next char is letter or underscore:
      - Start of variable name
      - Scan while alphanumeric or underscore
      - Return position and length
   
   d. Otherwise (space, operator, etc.):
      - Not a variable
      - Continue search

4. Repeat until variable found or end of string
```

**Examples**:
```
"Hello $USER"
  Position 6, length 4 (USER)

"Path: $PATH and $HOME"
  First call: position 6, length 4 (PATH)
  Second call: position 20, length 4 (HOME)

"Cost: $5.00"
  No variable ($ followed by digit)

"Exit: $?"
  Position 6, length 1 (?)
```

#### Task A18: Expand Variables

**expand_variable(token, shell)**:
- Purpose: Replace all variables in token with values
- Modifies: token->value

**Algorithm**:
```
1. Initialize:
   - result = empty string
   - position = 0

2. Loop finding variables:
   
   a. Find next variable:
      - Call find_variable(token->value, position)
      - If none found: break
   
   b. Append text before variable:
      - Add characters from position to variable start
   
   c. Get variable value:
      - If variable is "?":
        * Convert last_exit_status to string
      - Otherwise:
        * Get from environment: get_env_value(key)
        * If not found: use empty string
   
   d. Append variable value to result
   
   e. Update position past variable

3. Append remaining text after last variable

4. Replace token->value with result

5. Free old value
```

**Example**:
```
Token value: "User $USER in $HOME"
last_exit_status: 0
USER=student
HOME=/home/student

Step 1: Find $USER at position 5
  Append: "User "
  Get value: "student"
  Append: "student"
  Position = 10

Step 2: Find $HOME at position 14
  Append: " in "
  Get value: "/home/student"
  Append: "/home/student"
  Position = 19

Step 3: No more variables
  Append: ""

Result: "User student in /home/student"
```

#### Task A19: Handle Quote Types

**Expansion Rules**:
- **No quotes** (quote_type=0): Expand all variables
- **Double quotes** (quote_type=2): Expand all variables
- **Single quotes** (quote_type=1): NO expansion

**Algorithm**:
```
For each token in token list:
  
  1. Check quote_type:
     
     If quote_type == 1 (single quotes):
       - Skip expansion
       - Continue to next token
     
     If quote_type == 0 or 2:
       - Perform expansion
       - Replace variables with values
```

**Examples**:
```bash
echo $USER
# quote_type=0 → expand
# Result: echo student

echo "$USER"
# quote_type=2 → expand
# Result: echo student

echo '$USER'
# quote_type=1 → don't expand
# Result: echo $USER
```

### PARTNER B - Days 15-16: Exit Status Expansion

#### Task B13: Track Exit Status

**Update after every command**:

**After builtin execution**:
```
exit_code = builtin_function(...)
shell->last_exit_status = exit_code
```

**After external command**:
```
waitpid(pid, &status, 0)

If WIFEXITED(status):
  exit_code = WEXITSTATUS(status)
Else if WIFSIGNALED(status):
  exit_code = 128 + WTERMSIG(status)

shell->last_exit_status = exit_code
```

**Special cases**:
- Command not found: set to 127
- Permission denied: set to 126
- Signal termination: set to 128 + signal number

#### Task B14: Expand $?

**Already handled in expand_variable**:
```
If variable name is "?":
  value = convert last_exit_status to string
```

**Test Cases**:
```bash
ls
echo $?
# Should print 0

ls /nonexistent 2>/dev/null
echo $?
# Should print non-zero

false
echo $?
# Should print 1

true
echo $?
# Should print 0

bash -c "exit 42"
echo $?
# Should print 42
```

### BOTH PARTNERS - Days 17-18: INTEGRATION & TESTING

#### Task: Integrate Expansion

**Modify main loop**:
```
Old flow:
  1. Tokenize
  2. Parse
  3. Execute

New flow:
  1. Tokenize
  2. EXPAND VARIABLES ← New step
  3. Parse
  4. Execute
```

**expand_tokens(tokens, shell)**:
- Purpose: Expand all variables in all tokens
- Called after tokenization, before parsing

**Algorithm**:
```
For each token in list:
  If token is WORD:
    expand_variable(token, shell)
```

**Why after tokenization?**
- Tokenizer preserves quote information
- Parser needs expanded values

**Why before parsing?**
- Parser works with final values
- Simpler parsing logic

#### Task: Comprehensive Testing

**Test Cases**:

1. **Basic Expansion**:
```bash
echo $HOME
echo $PATH
echo $USER
```

2. **Exit Status**:
```bash
true; echo $?
false; echo $?
ls; echo $?
```

3. **Quote Handling**:
```bash
echo $USER
echo "$USER"
echo '$USER'
```

4. **Multiple Variables**:
```bash
echo $USER $HOME $PATH
```

5. **In Commands**:
```bash
cd $HOME
ls $HOME
```

6. **In Redirections**:
```bash
cat < $HOME/.bashrc
echo test > $HOME/test.txt
```

7. **Undefined Variables**:
```bash
echo $UNDEFINED
# Should expand to empty string
```

8. **Special Cases**:
```bash
echo $?$?
echo $$
echo $0
```

---

## PHASE 6: CLEANUP & POLISH (Days 19-21)

### Memory Management

**Critical**: No memory leaks allowed!

**Function Organization**:

1. **Token Cleanup**:
```
free_tokens(tokens):
  For each token:
    Free value string
    Free token structure
```

2. **Command Cleanup**:
```
free_commands(commands):
  For each command:
    Free args array (each string then array)
    Free redirections list
    Free command structure
```

3. **Redirection Cleanup**:
```
free_redirects(redirects):
  For each redirection:
    Free filename string
    Free redirection structure
```

4. **Environment Cleanup**:
```
free_env_list(env):
  For each node:
    Free key string
    Free value string
    Free node structure

free_envp_array(envp):
  For each string:
    Free string
  Free array
```

5. **Shell Cleanup**:
```
cleanup_shell(shell):
  Free environment list
  Free envp array
  Free tokens (if any)
  Free commands (if any)
  Free child_pids (if any)
  Clear history (readline function)
```

**Testing**:
```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell
```

Run various commands, then exit. Should show:
```
All heap blocks were freed -- no leaks are possible
```

### Error Handling

**Consistent Error Messages**:

Format: `minishell: command: error_details`

Examples:
```
minishell: cd: /tmp: No such file or directory
minishell: export: `123': not a valid identifier
minishell: nonexistent: command not found
```

**Error Output**: Always to stderr (FD 2)

**Exit Codes**:
- 0: Success
- 1: General error
- 2: Misuse of builtin
- 126: Permission denied / cannot execute
- 127: Command not found
- 128+N: Terminated by signal N

### Norm Compliance

**Check regularly**:
```bash
norminette includes/ srcs/
```

**Common issues**:
- Functions > 25 lines
- Lines > 80 characters
- More than 5 functions per file
- Forbidden functions
- Global variables (only g_signal allowed!)

### Edge Cases

**Test thoroughly**:

1. **Empty Input**:
```bash
  (just Enter)
  minishell$  (reprompt, no error)
```

2. **Only Whitespace**:
```bash
        (spaces and tabs)
  minishell$  (reprompt, no error)
```

3. **Unclosed Quotes**:
```bash
  echo "hello
  minishell: syntax error: unclosed quote
```

4. **Invalid Syntax**:
```bash
  | cat
  minishell: syntax error near |
  
  cat <
  minishell: syntax error: expected filename
```

5. **Signal During Input**:
```bash
  echo hel^C
  (displays new prompt)
```

6. **Signal During Execution**:
```bash
  sleep 10
  ^C
  (kills sleep, returns to prompt)
```

7. **Multiple Redirections Same Type**:
```bash
  cat < file1 < file2
  (uses file2, last one wins)
  
  echo > f1 > f2
  (writes to f2, last one wins)
```

8. **Very Long Pipelines**:
```bash
  cat | cat | cat | cat | ... (many cats)
  (should work, maybe slow)
```

---

## PHASE 7: BONUS (Optional, Days 22+)

### Logical Operators && and ||

**Behavior**:
- `cmd1 && cmd2`: Execute cmd2 only if cmd1 succeeds (exit 0)
- `cmd1 || cmd2`: Execute cmd2 only if cmd1 fails (exit non-zero)

**Implementation**:
Need AST (tree structure) to handle precedence

**Example**:
```bash
ls && echo "success" || echo "failure"

If ls succeeds (exit 0):
  Execute echo "success" → prints "success"
  Don't execute echo "failure" (left side of || succeeded)

If ls fails (exit non-zero):
  Don't execute echo "success" (left side of && failed)
  Execute echo "failure" → prints "failure"
```

### Wildcards (Globbing)

**Behavior**:
`*` expands to all matching files in current directory

**Example**:
```bash
ls *.c
# Expands to: ls file1.c file2.c file3.c

echo test*
# Expands to: echo test1 test2 testing

cat *.txt
# Expands to: cat file1.txt file2.txt
```

**Implementation**:
1. Detect `*` in token
2. Get current directory contents (opendir/readdir)
3. Match filenames against pattern
4. Replace token with matched files
5. Sort alphabetically (bash behavior)

---

## FINAL DELIVERABLES

### README.md

Required sections:
1. **Header**: "This project has been created as part of the 42 curriculum by <login1>, <login2>"
2. **Description**: What minishell is and does
3. **Instructions**: How to compile and run
4. **Resources**: References used, AI usage details

### Code Quality

**Before submission**:
- ✅ All features working
- ✅ No memory leaks (valgrind clean)
- ✅ Norm compliant
- ✅ No forbidden functions
- ✅ Makefile works correctly
- ✅ README complete

### Testing

Create comprehensive test script:
```bash
#!/bin/bash

# Test script for minishell

echo "Testing basic commands..."
./minishell << EOF
pwd
echo hello
ls
exit
EOF

echo "Testing pipes..."
./minishell << EOF
ls | wc -l
cat Makefile | grep CC
exit
EOF

# etc...
```

Compare with bash behavior!

---

## TIME MANAGEMENT TIPS

**Weekly Milestones**:
- Week 1: Setup + Tokenizer + Environment + Basic builtins
- Week 2: Parser + Remaining builtins + Signals
- Week 3: Execution + Redirections
- Week 4: Pipes + Expansion
- Week 5: Testing + Cleanup + Bonus (if time)
- Week 6: Final testing + Documentation

**Daily Routine**:
1. 15-minute standup with partner
2. Code independently for 3-4 hours
3. Integration session (1-2 hours)
4. Testing and debugging (1 hour)
5. Document progress

**Communication**:
- Daily progress updates
- Share blockers immediately
- Code review before merging
- Pair program on difficult parts

---

## SUCCESS CHECKLIST

### Mandatory
- [ ] Shell displays prompt
- [ ] Command history works
- [ ] Path resolution works
- [ ] Quote handling (single and double)
- [ ] Redirections: <, >, <<, >>
- [ ] Pipes work correctly
- [ ] Environment variables expand
- [ ] $? expands correctly
- [ ] Ctrl-C, Ctrl-D, Ctrl-\ behave correctly
- [ ] All builtins implemented and working
- [ ] No memory leaks
- [ ] Norm compliant
- [ ] README complete

### Bonus
- [ ] && and || operators
- [ ] Parentheses for grouping
- [ ] Wildcards (*) work

Good luck with your project!
