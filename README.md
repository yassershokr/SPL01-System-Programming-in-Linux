#    SPL01-System-Programming-in-Linux
## Simple Unix Utilities, Femto Shell and Pico Shell

This repository contains implementations of simple Unix utilities (`pwd`, `echo`, `cp`, `mv`), a basic shell (`Femto Shell`) and ('Pico Shell') in C. These programs are designed for educational purposes to demonstrate basic system programming and shell functionalities.

## Utilities

### 1. pwd
- **Description**: Prints the current working directory.
- **Compilation**: `gcc -o pwd pwd.c`
- **Usage**: `./pwd`
- **Example Output**: `/home/user`

### 2. echo
- **Description**: Prints the user input string to stdout.
- **Compilation**: `gcc -o echo echo.c`
- **Usage**: `./echo Hello World`
- **Example Output**: `Hello World`

### 3. cp
- **Description**: Copies a file from a source to a destination.
- **Compilation**: `gcc -o cp cp.c`
- **Usage**: `./cp source_file destination_file`
- **Example**: Copies `file.txt` to `/tmp/file_copy.txt`.

### 4. mv
- **Description**: Moves a file to a new location or renames it.
- **Compilation**: `gcc -o mv mv.c`
- **Usage**: `./mv source_file destination_file`
- **Example**: Moves `file.txt` to `/tmp/file_renamed.txt`.

## Femto Shell

### Description
Femto Shell is a simple shell that supports basic commands like `echo` and `exit`. It is designed to demonstrate basic shell functionalities.

### Features
- **echo [text]**: Echoes back the text entered by the user.
- **exit**: Terminates the shell with a goodbye message.
- Prints an "Invalid command" error for unsupported commands.

### Compilation
To compile the shell, use the following command:

```bash
gcc -o femto_shell femto_shell.c

## Pico Shell
A simple shell implementation in C supporting built-in commands and external program execution.

## Features
 Built-in Commands:
- `echo`
- `pwd`
- `cd`
- `exit`

 External command execution using `fork()` and `execvp()`
 Dynamic memory allocation for argument parsing
 Error handling for system calls
---
## Compilation
```bash
gcc -Wall -Wextra -o pico_shell pico_shell.c
###Example Session:
pico-shell> pwd
/home/user/pico_shell

pico-shell> echo Hello World!
Hello World!

pico-shell> cd ..
pico-shell> pwd
/home/user

pico-shell> ls
pico_shell  pico_shell.c  README.md

pico-shell> exit
