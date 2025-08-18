# Minishell

**Minishell** is a collaborative project developed as part of the **42 School** curriculum.  
The goal is to build a simple Linux shell inspired by **bash**.  

It provides a minimal but functional environment where users can run commands, handle pipes, redirections, environment variables, signals, and more — just like in a real shell.  

---

## 🎯 Project Goals
- Process creation and management (fork, execve, waitpid)
- File descriptors and redirections
- Pipes and inter-process communication
- Signal handling in Unix
- Environment variable management
- Parsing and lexical analysis

---

## 🚀 Features

### ✅ Implemented
- **Prompt display** – shows a prompt while waiting for user input.  
- **Command history** – commands are stored and can be recalled.  
- **Executable search & execution** – supports PATH resolution, relative, and absolute paths.  
- **Signal handling** – uses at most one global variable for received signals.  
- **Quoting rules**:  
  - `'` single quotes prevent interpretation of all special characters.  
  - `"` double quotes prevent interpretation except for `$`.  
- **Redirections**:  
  - `<` input redirection  
  - `>` output redirection  
  - `>>` output append redirection  
  - `<<` heredoc with delimiter (does not update history)  
- **Pipes (`|`)** – supports multiple pipes, chaining commands together.  
- **Environment variables** – expansion with `$VAR`.  
- **Special variable** – `$?` expands to the last command’s exit status.  
- **Signals (interactive mode)**:  
  - `Ctrl-C` → displays a new prompt on a new line  
  - `Ctrl-D` → exits the shell  
  - `Ctrl-\` → ignored  
- **Built-in commands**:  
  - `echo` with option `-n`  
  - `cd` with relative/absolute paths  
  - `pwd` with no options  
  - `export` with no options  
  - `unset` with no options  
  - `env` with no options or arguments  
  - `exit` with no options  

### ✨ Bonus Features
- **Logical operators** – `&&` and `||` with parenthesis for priorities.  
- **Wildcards** – `*` works for the current working directory.  

---

### ❌ Not Implemented
- Advanced bash features outside the project scope:  
  - backslashes `\`  
  - semicolons `;`  
  - job control  
  - advanced globbing  
  - subshells  
  - process substitution  
  - other bash-specific features  

---

## 🛠️ How to Run

```bash
git clone git@github.com:beatrizfiochi/minishell.git
cd minishell
make
./minishell
```

---

## 📖 Usage Example
```
minishell$ echo "Hello, World!"
Hello, World!

minishell$ export NAME=42
minishell$ echo $NAME
42

minishell$ ls | grep minishell > output.txt
```

---

## 👤 Authors
Developed by `@beatrizfiochi` and `@djunho`


