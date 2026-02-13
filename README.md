# Operating Systems Lab Assignments (4th Semester)

This repository contains the solutions for the Operating Systems Laboratory assignments. The code demonstrates proficiency in Linux shell scripting, system calls, process management, and file handling in a UNIX environment.

## 👨‍💻 Team Members

| Name | Roll Number |
| :--- | :--- |
| **Aayush Praveen** | 24AI002 |
| **Abhinav Mishra** | 24AI004 |
| **Aditya Kumar** | 24AI007 |
| **Aman Sharma** | 24AI010 |

---

## 🛠️ Environment & Tools
* **OS:** Ubuntu Linux (22.04 LTS / 24.04 LTS)
* **Shell:** Bash (Bourne Again SHell)
* **Compiler:** GCC (GNU Compiler Collection)
* **Debuggers/Tools:** `strace`, `gdb`, `top`, `bc`

---

## 📂 Assignment 1: Basic Linux Commands & File Management
**Objective:** To understand basic Linux commands for file handling (`touch`, `cat`, `cp`, `mv`) and hierarchical directory management.

* **File:** `assignment1.sh`
* **Description:** An automated shell script that:
    1.  Creates a directory structure (`ProjectWorkspace/Docs`, `Code`, etc.).
    2.  Performs file creation and redirection (`echo`, `>`).
    3.  Analyzes file statistics (`wc`).
    4.  Demonstrates pattern matching by moving files based on wildcards.
* **Execution:**
    ```bash
    chmod +x assignment1.sh
    ./assignment1.sh
    ```

---

## 📂 Assignment 2: Advanced Shell Scripting
**Objective:** To implement logic using control structures (loops, conditions) and design menu-driven programs.

### 1. Factorial Calculator
* **File:** `factorial.sh`
* **Logic:** Calculates $n!$ using a `for` loop. Handles negative number validation.

### 2. Address Book (CRUD)
* **File:** `addressbook.sh`
* **Logic:** A menu-driven program to Create, Read, Update, and Delete contacts in a persistent text file (`contacts.txt`). Uses `grep` and `sed` for data manipulation.

### 3. Bulk Image Renamer
* **File:** `rename_jpg.sh`
* **Logic:** Automatically renames all `.jpg` files in the directory by prefixing the current date (`YYYY-MM-DD`).

### 4. Geometry Calculator
* **File:** `geometry.sh`
* **Logic:** Calculates Area and Circumference for various shapes. Uses `bc` (Basic Calculator) for high-precision floating-point arithmetic.


## 📂 Assignment 3: System Calls & Process Interactions
**Objective:** To understand the boundary between user-space and kernel-space by implementing standard Linux system calls in C and analyzing them with system monitoring tools.

### 1. System Call Implementation
* **File:** `syscall_demo.c`
* **Description:** A C program that bypasses standard library functions (`fopen`, `fprintf`) to interact directly with the kernel.
* **Key Concepts Implemented:**
    * **File I/O:** Used `open()` (with flags `O_CREAT|O_WRONLY`), `write()`, and `close()` for direct file manipulation.
    * **Process Creation:** Used `fork()` to create a child process.
    * **Process Identification:** Used `getpid()` to retrieve Process IDs for both parent and child.
    * **Synchronization:** Implemented `wait()` to ensure the parent process waits for the child to terminate, preventing "zombie" processes.

### 2. System Analysis & Debugging
**Objective:** Apply debugging techniques to observe system-level behavior.

* **Tool: `strace` (System Call Tracer)**
    * Used to intercept and record the system calls invoked by the process.
    * **Observation:** Verified that `printf` internally calls `write` and `fork` internally calls `clone`.
    * *Command:* `strace ./syscall_demo`

* **Tool: `gdb` (GNU Debugger)**
    * Used for step-by-step execution and inspecting file descriptors.
    * **Observation:** Inspected the file descriptor integer values (typically returning `3` for the first open file).
    * *Command:* `gdb ./syscall_demo`

* **Tool: `top` (Process Monitor)**
    * Used to observe the process status and CPU consumption during the execution loop.

### **Compilation & Execution**
```bash
# Compile with debugging information included
gcc -g syscall_demo.c -o syscall_demo

# Execute the program
./syscall_demo

# Verify the output file created by system calls
cat syscall_output.txt
