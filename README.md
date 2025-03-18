#    SPL01-System-Programming-in-Linux
## Simple Unix Utilities, Femto Shell and Pico Shell & its extention

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

gcc -o femto_shell femto_shell.c

## Pico Shell - A Simple UNIX Shell in C

## Features
- **Built-in Commands:**
  - `echo`: Prints text to the terminal
  - `pwd`: Prints the current working directory
  - `cd`: Changes the working directory
  - `exit`: Exits the shell
- **External Command Execution:** Supports commands like `ls`, `cat`, `date`, etc.
- **Dynamic Argument Parsing:** No fixed limit on the number of arguments
- **Memory Management:** No memory leaks
- **Error Handling:** Validates system calls and prints descriptive error messages

---

## Compilation Instructions
Use the following command to compile the shell:
gcc -Wall -Wextra -o pico_shell pico_shell.c

## Pico Shell Extention

## Description
**Pico Shell** is a simple UNIX-like shell implemented in C. It supports basic command execution, local and environment variables, variable substitution, and exporting variables to the environment.

This project demonstrates important operating system concepts like:
- Process creation and control
- Environment manipulation
- String tokenization
- Memory management in C

## Features
 Built-in commands:
- `pwd`
- `cd`
- `echo`
- `export`
- `exit`

 Local variable assignments in the format `var=value`
 Variable substitution using `$var`
 Export local variables to environment variables with the `export` command
 External command execution with inherited environment variables
 Error handling and dynamic memory allocation

## Compilation
To compile the Pico Shell:

gcc -o pico_shell pico_shell.c


