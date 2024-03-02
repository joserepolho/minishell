# Shell Built-in Commands and Their Options

This document provides an overview of commonly used built-in commands in Unix-like shells such as Bash, along with some of their options. Built-in commands are executed directly by the shell, without spawning an external process.

## `cd` - Change Directory

Changes the current directory.

- `cd` : Changes to the home directory.
- `cd [directory]` : Changes to the specified directory.

### Non-zero return:
- The target directory does not exist.
- The user lacks permission to enter the target directory.
- The argument provided is not a directory (e.g., a file).

## `echo` - Display a Line of Text

Outputs the strings it is being passed as arguments.

- `echo [string]` : Prints the string to standard output.
- `echo [-n] [string]` : Do not output the trailing newline.

### Always returns Zero

`exit status` : 0

## `export` - Set Export Environment Variable

Marks an environment variable to be exported with any newly forked child processes and thus allows a child process to inherit all marked variables.

- `export [VARNAME=value]` : Sets the environment variable `VARNAME` to `value`.
- `export` : Lists all names that are exported in the current shell.

### Non-zero return:
- The variable name is invalid.

## `unset` - Remove Variable

Removes the variable from the environment.

- `unset [VARNAME]` : Removes the environment variable `VARNAME`.

### Non-zero return:
- The variable name is invalid.

## `pwd` - Print Working Directory

Prints the current working directory.

- `pwd` : Prints the current working directory.

### Non-zero return:
- The current working directory cannot be determined.

## `env` - Print Environment

Prints the current environment.

### Always returns Zero

## `exit` - Exit the Shell

Exits the shell with a status of `n`.

- `exit [n]` : Exits the shell with a status of `n`.
- `exit` : Exits the shell with the status of the last command executed.
- `exit [nan]` : Exits the shell with a status of `255`.
- `exit [n>255]` : Exits the shell with a status of `n`.