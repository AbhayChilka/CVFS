# 📁 Custom Virtual File System (CVFS) – C++

## 📌 Project Overview

The **Custom Virtual File System (CVFS)** is a simulation of a UNIX-like file system implemented in C++.

This project demonstrates how core file system components such as **Inodes, Superblock, File Table, and File Descriptors** work internally.

It provides a command-line interface similar to Linux shell commands and mimics system calls like:

- create
- open
- read
- write
- close
- ls
- stat
- fstat
- truncate
- rm

This project was built to understand low-level file system architecture and system programming concepts.

---

## 🧠 Concepts Covered

- Inode-based File System Design
- File Descriptor Management
- Linked List Implementation
- Dynamic Memory Allocation
- Command Line Parsing
- System Programming Fundamentals

---

## 🏗️ System Architecture

- The internal architecture of CVFS is inspired by the UNIX file system.
### [Architecture Document](Docs/architecture.md)

---

## 📘 Commands Description
### [Commands_Usage Document](Docs/command_usage.md)

---

## ⚙️ Internal-Working
### [Internal_work Document](Docs/internal_working.md)

---

## ⚙️ Features Implemented

- Create new file
- Delete file
- Open file
- Close file
- Read file
- Write file
- List all files
- Display file information
- Truncate file
- Lseek functionality
- Permission handling (Read / Write / Read & Write)

---

## 🖥️ Supported Commands

| Command | Description |
|----------|-------------|
| create <name> <permission> | Create new file |
| open <name> <mode> | Open file |
| write <fd> | Write into file |
| read <fd> <size> | Read file data |
| close <name> | Close file |
| closeall | Close all opened files |
| ls | List all files |
| stat <name> | Display file information |
| fstat <fd> | Display file info using FD |
| truncate <name> | Remove file data |
| rm <name> | Delete file |

---

## 🛠️ How to Compile and Run

### On Linux / Ubuntu

```bash
g++ CVFS.cpp -o cvfs
./cvfs


