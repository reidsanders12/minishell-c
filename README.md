# minishell-c

A minimal Unix-style command shell written in C. It reads commands, splits them into arguments, and runs them as child processes using the same fork/exec/wait model that shells like bash are built on.

## Features

- Interactive prompt that runs any program on your `PATH` (`ls -la`, `echo`, `pwd`, `gcc`, ...)
- Process creation and management with `fork()`, `execvp()`, and `waitpid()`
- Built-in commands: `cd` and `exit`
- Handles empty input, unknown commands, and Ctrl-D (EOF) cleanly
- System-level error reporting with `perror()`

## Build and run

Requires macOS or Linux (uses POSIX system calls).

```
gcc -Wall -Wextra -o minishell minishell.c
./minishell
```

## Example

```
Minishell> pwd
/home/user/projects
Minishell> cd ..
Minishell> echo hello from minishell
hello from minishell
Minishell> exit
```

## How it works

1. **Parse:** `parse_input()` tokenizes the line with `strtok()` on spaces, tabs, and newlines into a NULL-terminated argument array (the format `execvp()` expects).
2. **Built-ins:** `cd` and `exit` are handled directly by the shell process.
3. **Execute:** everything else is run by forking a child process, which replaces itself with the requested program via `execvp()`, while the parent waits until the child exits or is terminated by a signal.

### Why `cd` has to be a built-in

A child process can't change its parent's working directory. If `cd` were run as a separate program, it would change directories and immediately exit, leaving the shell where it started. So the shell has to call `chdir()` itself.

## Limitations and next steps

- No pipes (`|`), I/O redirection (`>`, `<`), or background jobs (`&`)
- No quote handling, so `echo "hello world"` is split into two arguments
- No command history
