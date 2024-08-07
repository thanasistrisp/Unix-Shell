# System Programming - Exercise 1 (Implementing a Shell)

## Compilation

Either in the root directory or in the `source_code` directory:

```bash
make
```

Tested at: `Ubuntu 18.04.6 LTS` with `g++` 7.5.0 (Linux 25).

## Execution

Only in source_code directory:

```bash
./mysh
```

## Features

## Multiple commands in one line

Multiple commands in one line are supported by using `;` as a delimiter. The commands are executed sequentially. `&&` operator is not supported.

Note: In background execution, `&` should be followed by `;` to execute the next command. It is not supported without `;` after it as in bash e.g.
as instructions had this structure: `sort file1 &; ls &;` and not `sort file1 & ls &`.

## Memory management

No memory leaks were detected by using valgrind. However, if execvp fails (e.g. command not found), we exit child and C++ destructor is not called by default
(probably for performance reasons). In this case, some fault leaks are detected `still reachable memory` by valgrind. This is not a problem as the child program
terminates and the memory is freed by the OS. It should be noted that *never* there are `definitely lost` memory leaks. You can check this by running:

```bash
make valgrind
```

However, the intended way to run the program is `./mysh` for correct execution and signal handling.

### Built-in Commands

- `cd` - Change directory
- `exit` - Exit the shell
- other commands requested in the exercise

`cd` can be used with no arguments or with `~` to change to the home directory, or with a single argument to change to the specified directory.
It can also be used with the `-` option to change to the previous directory.

Note: You can exit the shell by pressing `Ctrl+D` as well.

### Special characters

All special characters (except `|` and `;`) can be escaped inside double quotes (`"`). Wordexp is used to expand the arguments including wildcards,
environment variables and escaping special characters. Whitespaces are allowed between delimiters (`;`, `|`, `&`, `>`, `<`, `>>`) and commands.

### Syntax error checking

Generally, no syntax error checking is done. Whatever error returned by shell is error from `execvp`.

### Fork - exec

Fork is used to create a child process for each command by replacing child with execvp. The parent process waits for the child process to finish with pause (until signal of done execution is received) before executing the next command or it does not wait if the command is followed by `&` (background execution).

Note: `execvp` is used instead of `execv` to allow the use of PATH. Custom commands (built-in commands) are executed in the parent process without forking.
This is done to allow the use of `cd` command as it changes the directory of the child process and not the parent process. That, however, means
that all those commands do not support redirection and piping which is not a problem as they are not supposed to be used with those.

### Redirection

File descriptors are redirected using `dup2` and `open` system calls in the child process with the appropriate flags.

### Piping

Piping is done by creating a pipe and forking a child process for each command. The child process reads from the pipe and writes to the pipe.

Note: multiple pipes are supported as well.

### Background execution

Background execution is done by not waiting for the child process to finish by returning however the control to shell.
`fg` command is not implemented.

### Wildcards characters

Wildcards characters (`*`, `?`, `[`, `]`) are expanded using `wordexp` before handling the command.

Note: wordexp is very similar to glob.

### Aliases

Aliases are supported by using a map to store the aliases. The aliases are checked before executing the command and if an alias is found, the command is replaced with the alias.

### Signal handling

Signal handling is done by using `sigaction` system call. The signals handled are `SIGINT` and `SIGTSTP`. `SIGINT` is handled by ignoring it in the parent process and by killing the child process in the child process. `SIGTSTP` is handled by ignoring it in the parent process and by pausing the child process in the child process.

Tip: `Ctrl+Z` when used in shell process, it is ignored and deleted from the input with `\b` (backspace) character.

### History

History is implemented by using a queue to store the commands (by inheriting std::deque for fixed size). We could also use a vector, but the overall complexity is the same $O(1)$, as the number of stored items is fixed. The history is printed by using `myHistory` command and we can go back in history by using `myHistory i` command. The history is available only in the current session. The default is 20, but it can be changed in header definition. History commands are not included in history.
