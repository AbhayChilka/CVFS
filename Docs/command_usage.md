# 📘 Command Reference – CVFS

This document provides detailed information about all supported commands in the Custom Virtual File System (CVFS).

---

## 🔹 create

**Syntax:**
create <filename> <permission>

**Description:**
Creates a new file with the given name and permission.

**Permissions:**
- 1 → Read
- 2 → Write
- 3 → Read & Write

**Example:**
create Demo 3

**Returns:**
- Success → File descriptor
- Failure → Error message

---

## 🔹 open

**Syntax:**
open <filename> <mode>

**Description:**
Opens an existing file in specified mode.

**Modes:**
- 1 → Read
- 2 → Write
- 3 → Read & Write

**Example:**
open Demo 3

---

## 🔹 close

**Syntax:**
close <filename>

**Description:**
Closes the specified file and removes its file table entry.

---

## 🔹 closeall

**Syntax:**
closeall

**Description:**
Closes all currently opened files.

---

## 🔹 write

**Syntax:**
write <file_descriptor>

**Description:**
Writes user input data into the file starting from current write offset.

**Example:**
write 0

---

## 🔹 read

**Syntax:**
read <file_descriptor> <size>

**Description:**
Reads specified number of bytes from file starting at current read offset.

**Example:**
read 0 10

---

## 🔹 ls

**Syntax:**
ls

**Description:**
Lists all existing files in the system.

---

## 🔹 stat

**Syntax:**
stat <filename>

**Description:**
Displays metadata of the file using filename.

Shows:
- File name
- Inode number
- File size
- Actual file size
- Link count
- Reference count
- Permission

---

## 🔹 truncate

**Syntax:**
truncate <filename>

**Description:**
Removes all data from the file but keeps the file entry.

---

## 🔹 rm

**Syntax:**
unlink <filename>

**Description:**
Deletes the file from the system and frees allocated memory.

---
