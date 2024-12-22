# Multithreaded Systems and Synchronization Simulations

This repository contains implementations of multithreaded systems and synchronization problems using the C programming language. The programs demonstrate solutions to complex threading and synchronization challenges, including deadlock avoidance, resource sharing, inventory management, and matrix multiplication optimization.

---

## Tasks Overview

### Q1: Deadlock-Free Thread Scheduling

**Description:**  
This program simulates thread scheduling to avoid deadlocks using resource instance ordering. The program ensures that threads acquire locks in a strict order, avoiding circular waits and guaranteeing deadlock-free execution.

**Design:**
- 3 threads (T1, T2, T3) acquire 2 locks (Lock A and Lock B).
- Locks must be acquired in a strict order (e.g., Lock A before Lock B).
- Each thread acquires each lock 3 times.

**Input:** None.  
**Output:**  
- Messages indicating lock acquisition and waiting by threads (e.g., "T1 acquired Lock A", "T2 waiting for Lock A").
- Program terminates after all threads complete execution.

**Assumptions:**
- Threads follow a predefined order for lock acquisition to avoid deadlock.
- No external user input is required.

---

### Q2: Networked Servers Synchronization

**Description:**  
This program simulates a synchronization problem where 5 servers share network channels. Each server requires exclusive access to two adjacent network channels to process data. The program ensures mutual exclusion and prevents starvation using semaphores.

**Design:**
- 5 servers, each requiring access to two shared channels.
- Channels are represented as semaphores.
- Each server processes data for 1 second when both channels are acquired.

**Input:** None.  
**Output:**  
- Messages indicating when a server is waiting for or has acquired channels (e.g., "Server 1 is processing", "Server 2 is waiting").
- Program terminates after each server has processed data 3 times.

**Assumptions:**
- Servers access shared channels in a fair manner to prevent starvation.
- Channels are released immediately after data processing.

---

### Q3: Warehouse Inventory Management

**Description:**  
This program simulates inventory management in a warehouse with limited storage. Delivery trucks deliver products, and storage managers organize them. A circular buffer is used to represent the storage, ensuring synchronization between threads.

**Design:**
- Circular buffer for storage with a maximum capacity.
- Multiple delivery truck threads deliver random quantities of products.
- Multiple storage manager threads remove random quantities of products.

**Input:**
- Number of delivery trucks and storage managers.
- Buffer size (defined as a constant).

**Output:**  
- Current inventory status after each operation.
- Messages indicating buffer full or empty conditions.

**Assumptions:**
- Delivery quantities and storage removal are random but within defined bounds.
- Trucks wait when the buffer is full, and managers wait when the buffer is empty.

---

### Q4: Matrix Multiplication with Threads

#### Part 1: Element-wise Threaded Multiplication

**Description:**  
Performs matrix multiplication using threads, where each thread calculates one element of the resultant matrix.

**Design:**
- Dynamically allocates matrices based on user input.
- Creates one thread per element of the resultant matrix.

**Input:**
- Matrix A (m x n).
- Matrix B (n x p).

**Output:**
- Resultant Matrix C.
- Speedup over sequential matrix multiplication.

**Assumptions:**
- Each thread handles only one matrix element.
- Matrices are dynamically allocated.

---

#### Part 2: Optimized Thread Pool Multiplication

**Description:**  
Optimizes the matrix multiplication process using a thread pool, where a fixed number of threads handle multiple tasks. Threads are reused to compute elements of the resultant matrix.

**Design:**
- Thread pool size is equal to the number of CPU cores.
- Thread pool accepts tasks via a generic API.

**Input:**
- Matrix A (m x n).
- Matrix B (n x p).

**Output:**
- Resultant Matrix C.
- Speedup over sequential and element-wise implementations.

**Assumptions:**
- Thread pool size is system-dependent.
- Generic thread pool API can handle tasks beyond matrix multiplication.

---

## How to Compile and Run

### Prerequisites
- GCC Compiler.
- Pthreads Library.

### Compilation
- Compile each program:
  ```bash
  gcc -pthread q1.c -o q1
  gcc -pthread q2.c -o q2
  gcc -pthread q3.c -o q3
  gcc -pthread q4_1.c -o q4_1
  gcc -pthread q4_2.c -o q4_2
