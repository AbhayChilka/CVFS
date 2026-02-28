# CVFS Architecture

## 1. Overview
Marvellous CVFS is an in-memory virtual file system implemented in C++.
It simulates core Linux file system concepts such as inodes, file descriptors,
permissions, and read/write offsets without using kernel-level APIs.

All file metadata and data are maintained in RAM, while optional backup
is taken to disk at program termination.

---

## 2. BootBlock
The BootBlock stores startup-related information.

### Structure
- Information: String indicating successful boot process

### Responsibility
- Display boot status during initialization

---

## 3. SuperBlock
The SuperBlock maintains global file system metadata.

### Fields
- TotalInodes
- FreeInodes

### Responsibility
- Track inode availability
- Prevent file creation when inode limit is reached

---

## 4. Inode (Disk Inode List Block – DILB)

Each file is represented using an inode structure.
All inodes are pre-created and managed as a linked list.

### Fields
- FileName
- InodeNumber
- FileSize (max allowed size)
- ActualFileSize
- FileType (REGULARFILE / FREE)
- ReferenceCount
- Permission (READ / WRITE)
- Buffer (data storage)
- next pointer

### Design Choice
- Linked list simplifies inode allocation and traversal
- Fixed-size buffer avoids complex memory management

---

## 5. FileTable
FileTable stores runtime information for an opened file.

### Fields
- ReadOffset
- WriteOffset
- Mode
- Pointer to inode

Each opened file gets its own FileTable entry.

---

## 6. UFDT (User File Descriptor Table)
UFDT is an array inside the UAREA structure.

### Characteristics
- Maximum open files: 20
- Index acts as file descriptor
- Entries 0,1,2 reserved

UFDT maps file descriptors to FileTable structures.

---

## 7. UAREA
Represents process-level information.

### Fields
- ProcessName
- UFDT array

Simulates per-process file descriptor management.

---

## 8. Memory Layout

BootBlock
|
SuperBlock
|
Inode Linked List (DILB)
|
UFDT Array
|
FileTable
|
File Data Buffer


---

## 9. Backup Mechanism
Before exit, all existing files are written to disk using `BackupFile()`
to preserve data externally.

---

## 10. Limitations
- Single-level file system
- No directory support
- No persistent storage during runtime
- Single process only

---

## 11. Future Enhancements
- Directory hierarchy
- Persistent disk-backed storage
- Multi-process support
- Permission enforcement per operation
