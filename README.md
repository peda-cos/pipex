*This project has been created as part of the 42 curriculum by peda-cos.*

# pipex

## Description

**pipex** reproduces the behavior of UNIX shell pipes in C. Given two commands and two files, it replicates the shell construct:

```sh
< file1 cmd1 | cmd2 > file2
```

The program forks child processes, connects them via a pipe, redirects file descriptors with `dup2`, and uses `execve` to execute the commands — mirroring how a real shell manages pipelines.

The bonus extends this to support **multiple pipes** (`cmd1 | cmd2 | ... | cmdn`) and **here documents** (`<<` input with `>>` append output).

## Instructions

### Compilation

```sh
# Mandatory (2-command pipeline)
make

# Bonus (multiple pipes + here_doc)
make bonus

# Clean object files
make clean

# Full clean (objects + binary)
make fclean

# Rebuild
make re
```

### Execution

**Mandatory:**
```sh
./pipex file1 cmd1 cmd2 file2
# Equivalent to: < file1 cmd1 | cmd2 > file2
```

**Examples:**
```sh
./pipex infile "ls -l" "wc -l" outfile
./pipex infile "grep hello" "wc -w" outfile
./pipex /dev/stdin "cat" "sort" outfile
```

**Bonus — multiple pipes:**
```sh
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
# Equivalent to: < file1 cmd1 | cmd2 | cmd3 | ... | cmdn > file2
```

**Bonus — here document:**
```sh
./pipex here_doc LIMITER cmd1 cmd2 file
# Equivalent to: cmd1 << LIMITER | cmd2 >> file
```

## Resources

### Documentation & References

- [pipe(2) man page](https://man7.org/linux/man-pages/man2/pipe.2.html) — creating anonymous pipes between processes
- [fork(2) man page](https://man7.org/linux/man-pages/man2/fork.2.html) — creating child processes
- [execve(2) man page](https://man7.org/linux/man-pages/man2/execve.2.html) — executing programs
- [dup2(2) man page](https://man7.org/linux/man-pages/man2/dup2.2.html) — duplicating file descriptors
- [waitpid(2) man page](https://man7.org/linux/man-pages/man2/waitpid.2.html) — waiting for child processes
- *The Linux Programming Interface* by Michael Kerrisk — chapters on processes, pipes, and file I/O

### AI Usage

AI assistance (Claude via OpenCode) was used for:

- **Project planning**: generating a structured task breakdown and design document covering architecture decisions (process model, error handling strategy, here_doc implementation approach)
- **Test case design**: suggesting edge cases including missing infile, command not found, permission denied, and multi-pipe bonus scenarios

