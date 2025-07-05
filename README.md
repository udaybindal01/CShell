# Custom Shell Program
This project implements a custom shell program with various commands that mimic and extend standard shell functionality. Below is a detailed description of each command and its usage.

## Table of Contents
1. Overview <br />
2. Commands Implemented
* `hop`
* `reveal`
* `log`
* `seek`
* `proclore`
* `Foreground and Background Process Handling` <br />
3. Compilation and Execution

## Overview
The shell program developed in this project includes several custom commands designed to extend the functionality of a typical shell environment. The commands are written in C and offer enhanced features such as improved directory navigation, process management, and command history tracking.

## Commands Implemented

1. `hop`

###### Description:
The hop command is used to change the current working directory in the shell. It supports various flags and arguments to navigate through directories.

###### Usage:
```
hop [directory_path]
```
###### Supported Flags:

`.` : Stay in the current directory.
`..` : Move to the parent directory.
`~` : Move to the shell's home directory.
`-` : Move to the previous directory.

###### Details:

If no argument is provided, the command hops into the shell's home directory.
If multiple arguments are given, hop executes sequentially for each.
It prints the full path of the working directory after changing.

2. `reveal`

###### Description:
The reveal command lists all files and directories in the specified directories in lexicographic order.

###### Usage:
```
reveal [options] [directory_path]
```

###### Supported Flags:

`-a` : Display all files, including hidden ones.
`-l` : Display detailed information similar to the ls -l command in Bash.

###### Details:

Supports paths like `.`, `..`, `~`, and `-`.
The output is color-coded: green for executables, white for files, and blue for directories.

3. `log`

###### Description:
The log command manages a history of the last 15 valid commands entered in the shell. It helps in tracking and re-executing commands.

###### Usage:
```

log
log purge
log execute <index>
```
###### Details:

log: Displays the list of commands stored in history.
log purge: Clears the command history.
log execute <index>: Executes a command from the history at the specified index.

4. `seek`

###### Description:
The seek command searches for files or directories in a specified target directory. It matches the prefix of the name, ignoring file extensions.

###### Usage:
```
seek [options] [target_directory]
```

###### Supported Flags:

`-d` : Search for directories only.
`-f` : Search for files only.
`-e` : Print contents if one file is found or change to the directory if one directory is found.

###### Details:

It returns a list of matching paths (files in green, directories in blue).
Handles invalid flag combinations gracefully.

5. `proclore`

###### Description:
The proclore command retrieves and displays information about a process, such as its PID, process status, process group, virtual memory usage, and executable path.

###### Usage:
```
proclore [pid]
```

###### Details:

If no argument is provided, the command displays information about the shell itself.
The process status can be R/R+ (Running), S/S+ (Sleeping in an interruptible wait), or Z (Zombie).
The + signifies whether it is a foreground or background process.

6. `Foreground and Background Process Handling`

###### Description:
The shell supports managing both foreground and background processes.

###### Usage:

**Foreground Processes:** Standard command execution. The shell waits for the process to complete before accepting more commands.
**Background Processes:** Commands followed by & are executed in the background. The shell continues accepting commands immediately.

###### Details:

The shell prints the PID of background processes and notifies the user when they finish.
For processes running longer than 2 seconds, the shell prints the execution time upon completion.

## Compilation and Execution
To compile the shell program, use the provided Makefile:
```
make
```

To run the shell program, use:

```
./a.out
```

