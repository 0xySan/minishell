![coverage](https://img.shields.io/badge/coverage-101%25-299617)
![version](https://img.shields.io/badge/version-0.2.34-blue)

# 🐚 minishell

A simple Unix shell implemented in C, built as part of the 42 curriculum.  
This project recreates core features of a POSIX shell like `bash` or `shell`, following the specifications provided by the school.

---

## ✅ Features

- Prompt display and command input
- Execution of binaries from `$PATH`
- Built-in commands:
  - `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`
- Pipes (`|`)
- Redirections (`>`, `>>`, `<`, `<<`)
- Environment variable expansion (`$VAR`)
- Signal handling (`Ctrl+C`, `Ctrl+\`)
- Exit codes and error messages that follow shell behavior

---

## 💡 What I Learned

- Writing a **lexer, parser, and executor** from scratch
- Managing **processes and file descriptors** (`fork`, `execve`, `dup2`, etc.)
- Handling **signals and terminal behavior**
- Understanding **shell grammar**, quoting, and edge cases
- Debugging complex **memory leaks** and edge behaviors with tools like `valgrind`
- Working in a **multi-person team** with Git and a shared codebase

---

## 📁 Usage

```bash
git clone https://github.com/your-username/minishell.git
cd minishell
make
./minishell
```
