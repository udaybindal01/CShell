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
* `Foreground and Background Process Handling`
* `myshrc`
* `activities`
* `signals`
* `fg and bg`
* `Neonate`
* `iMan` <br />

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


**Foreground Processes:** Standard command execution. The shell waits for the process to complete before accepting more commands.
**Background Processes:** Commands followed by & are executed in the background. The shell continues accepting commands immediately.

###### Details:

The shell prints the PID of background processes and notifies the user when they finish.
For processes running longer than 2 seconds, the shell prints the execution time upon completion.

7. `myshrc`

###### Description:
.myshrc is a custom configuration file inspired by .bashrc. It allows users to define aliases and functions that enhance the command-line experience.

###### Example Aliases::
```
reveall = reveal -l
alias home = hop ~
```


8. `I/O Redirection`

###### Description:

`>`: Redirects standard output to a file (overwrites if the file exists).
`>>`: Appends standard output to a file (creates the file if it doesn’t exist).
`<`: Redirects input from a file to a command.

9. `Pipe Support`

###### Description:

The shell supports an unlimited number of pipes to pass the output of one command as input to the next.

```
echo "Hello" | wc
Error Handling: Prints "Invalid use of pipe" if there's nothing on either side of a pipe.
```

10. `Redirection with Pipes`

###### Description:
Redirection works in conjunction with pipes.

###### Example:

```
cat < file.txt | wc | cat > result.txt
```

11. `Activities Command`

###### Description:
Lists all processes spawned by the shell in lexicographic order. Each entry includes:
```
PID Command Name State (Running or Stopped)
```

12. `Signals and Process Management`

###### Description:
`ping <pid> <signal_number>` sends a signal to a process.
Modulo 32 logic ensures signals wrap around.
Errors are handled if the PID doesn’t exist.

###### Example:

```
ping 221 9  # Sends SIGKILL
ping 430 47 # Sends SIGTERM (47 % 32 = 15)
```

13. `Foreground and Background Process Handling:`

###### Description:
`fg <pid>`: Brings a background process to the foreground.
`bg <pid>`: Resumes a stopped process in the background.

14. `Neonate`

###### Description:
neonate -n [time_arg]: Continuously prints the PID of the most recently created process every [time_arg] seconds until the key x is pressed.

15. `Networking: iMan Command`

###### Description:
`iMan <command_name>`: Fetches man pages from the website http://man.he.net/ and prints the response.
HTML tags are included in the output.
If the page doesn’t exist, the response will still print the error message.

###### Example:
```
iMan sleep    # Fetches the man page for 'sleep'
iMan invalid_command  # Prints the 'no matches found' response.
```

## Compilation and Execution
To compile the shell program, use the provided Makefile:
```
make
```

To run the shell program, use:

```
./a.out
```

