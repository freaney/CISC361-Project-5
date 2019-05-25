# Project 5
By Connor Onweller and Sophia Freaney

The main objective of this assignment is to design and implement a user-level thread library. Library supports thread creation, thread scheduling, thread synchronization, and inter-thread communication functions.

## Usage
Compile and run with:
```
make
```

## Phase 1
* Thread library initialization
* Thread library shutdown
* Thread creation
* Thread termination
* Thread yielding (of CPU to another thread)

## Phase 3
### Thread Synchronization
* Initialize and destroy semaphore
* Wait (P) on specified semaphore
* Signal (V) on specified semaphore

## Phase 4
### Inter-Thread Communication
* Create thread with `mbox_create`
* Destroy message box with `mbox_destroy`
* Deposit message with `mbox_deposit`
* Withdraw message with `mbox_withdraw`
* Nonblocking send with `send`
* Nonblocking receive with `receive`
