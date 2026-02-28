# ⚙️ Internal Working – CVFS

This document explains how different operations are handled internally inside CVFS.

---

## 🧠 System Initialization

When the program starts:

1. Superblock is initialized.
2. A linked list of inodes is created.
3. UFDT (User File Descriptor Table) is initialized.
4. All file table pointers are set to NULL.

---

## 📂 File Creation – create

Steps:

1. Check if free inode is available.
2. Verify file name does not already exist.
3. Allocate an inode from linked list.
4. Allocate memory buffer for file data.
5. Set metadata (permission, file size, type).
6. Update superblock free inode count.
7. Return file descriptor.

---

## 📂 File Open – open

Steps:

1. Search inode by filename.
2. Verify permission compatibility.
3. Create file table entry.
4. Initialize read/write offset.
5. Assign file descriptor in UFDT.

---

## ✍️ Write Operation – write

Steps:

1. Validate file descriptor.
2. Check write permission.
3. Copy data into file buffer.
4. Update write offset.
5. Update actual file size.

---

## 📖 Read Operation – read

Steps:

1. Validate file descriptor.
2. Check read permission.
3. Read data from buffer.
4. Update read offset.

---

## ❌ Delete File – unlink

Steps:

1. Locate inode.
2. Decrease link count.
3. Free buffer memory.
4. Reset inode metadata.
5. Increase free inode count.

---

## 🔒 Close File – close

Steps:

1. Locate file descriptor in UFDT.
2. Free file table entry.
3. Set UFDT entry to NULL.

---

## 🧱 Data Structures Interaction

Superblock → Tracks free inodes  
Inode → Stores file metadata  
File Table → Maintains offsets & mode  
UFDT → Maps file descriptor to file table  

---

## ⚠️ Important Notes

- All operations are performed in memory.
- Maximum file size is fixed (e.g., 1024 bytes).
- Maximum number of inodes is predefined.