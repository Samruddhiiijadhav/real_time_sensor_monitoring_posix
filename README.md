# Real-Time Multi-Process Sensor Monitoring System using POSIX APIs

## Overview
This project simulates a real-time sensor monitoring system using Linux system programming concepts. Multiple sensor processes generate data, and a controller process collects, processes, displays, and logs the data.

## Features
- Multi-process system using `fork()` and `exec()`
- Inter-process communication using `pipe()`
- Multithreading using `pthreads`
- Synchronization using `mutex`
- Signal handling (`SIGINT`) for graceful shutdown
- File logging using `open()`, `write()`, `lseek()`, `close()`
- Shared memory using `shmget()`, `shmat()`, `shmdt()`, `shmctl()`

## Architecture

The system is designed as a multi-process and multi-threaded application using Linux system calls and POSIX APIs.

** Process Level
- Four separate sensor processes are created using `fork()` and `exec()`:
  - Voltage Sensor
  - Current Sensor
  - Power Sensor
  - Frequency Sensor

- Each sensor generates simulated data at different sampling rates and sends it to the controller using pipes.
- The controller process acts as the central unit that receives, processes, and manages all sensor data.

** Thread Level (Controller)

The controller process uses multiple threads:

    *Sensor Thread*
  - Each thread reads data from one pipe (one sensor)
  - Updates shared variables and shared memory

    *Main Thread*
  - Displays real-time sensor values
  - Logs data into a file (`log.txt`)

** Inter-Process Communication (IPC)

    *Pipes*
  - Used for communication between sensor processes and controller

    *Shared Memory
  - Used to store the latest sensor values for fast access

** Synchronization
    
    *Mutex*
  - A mutex lockis used to protect shared data:
  - Prevents race conditions between threads
  - Ensures safe read/write operations

** Signal Handling

  - `SIGINT (Ctrl + C)` is handled for graceful shutdown
  - Stops all threads safely and releases resources

** File I/O

- Sensor data is logged using:
  - `open()`
  - `write()`
  - `lseek()`
  - `close()`

- Data is continuously stored in `log.txt`

** Data Flow

The data flows through the system as follows:

  1. Sensor processes (Voltage, Current, Power, Frequency) generate simulated data.
  2. Each sensor sends data to the controller process using pipes.
  3. The controller process uses separate threads to read data from each pipe.
  4. Threads update shared variables and shared memory with the latest sensor values.
  5. The main thread reads data from shared memory.
  6. Data is displayed on the terminal in real-time.
  7. Data is also written to a log file (`log.txt`) using file I/O system calls.

## Technologies Used

- **C Programming Language**
- **POSIX APIs**
- **Linux System Calls**
- **Pthreads (Multithreading)**
- **Inter-Process Communication (IPC)**
  - Pipes
  - Shared Memory
- **File I/O System Calls**
  - open(), write(), lseek(), close()
- **Signal Handling**
  - SIGINT

## Project Structure

sensor_monitor_system/
├── controller.c
├── voltage_sensor.c
├── current_sensor.c
├── power_sensor.c
├── frequency_sensor.c
├── sensor.h
├── shm_data.h
├── Makefile
├── README.md
├── .gitignore
└── log.txt (generated at runtime)

## How to Compile

*To compile the project, use the Makefile:*

  make

*To compile the project Manually* 

  gcc controller.c -o controller -pthread
  gcc voltage_sensor.c -o voltage_sensor
  gcc current_sensor.c -o current_sensor
  gcc power_sensor.c -o power_sensor
  gcc frequency_sensor.c -o frequency_sensor
  
## How to Run

*After compilation, run the controller:*

  ./controller
  
## How to Stop
*To stop the program, press:*

 CTRL + C
 
## Sample Output

V: 230.12 | I: 5.34 | P: 1229.84 | F: 49.98
V: 229.87 | I: 5.10 | P: 1172.33 | F: 50.02
V: 231.05 | I: 5.45 | P: 1259.22 | F: 49.95

## Learning Outcomes

- Understanding of process creation using `fork()` and `exec()`
- Implementation of inter-process communication using pipes and shared memory
- Experience with multithreading using POSIX threads (pthreads)
- Use of mutex for synchronization and avoiding race conditions
- Handling signals for graceful program termination
- Performing file operations using Linux system calls
- Designing a real-time data simulation system

## Future Improvements

- Add GUI for better visualization
- Use advanced IPC like message queues
- Improve data handling using circular buffer
- Enable remote monitoring
