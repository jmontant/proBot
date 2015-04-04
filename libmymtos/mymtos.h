/*
 * @file mymtos.h
 *
 * @author Paul Bammel
 *
 * @version v1.01
 *
 * @brief This library provides cooperative multitasking kernel 
 * functions for running multiple concurrent tasks in a single cog.
 *  
 *  v1.00 Paul Bammel - February 2015
 *    - Simple cooperative kernel
 *    - All tasks (functions) run to completion before yielding
 *    - A function must provide for external state determination
 *      if it runs longer than a single iteration
 *      
 *  v1.01 Paul Bammel - March 2015
 *    - Added simple Message Queue for InterProcess Communication
 */

#ifndef MYMTOS_H
#define MYMTOS_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "robot_defs.h"             // Need definition of command structure.

//General MTOS kernel constants
#define MAXTASKS  8                 // Maximum number of tasks for MTOS kernel.
#define QUEDEPTH  3                 // Maximum depth of message queues.
#define NULMSG    99                // Represents a NUL pointer in message queues.

// Task Status settings
#define HELD        0               // Task is in a Hold and will not run.
#define RUNABLE     1               // Task will run in its' time slice.
#define SLEEPING    2               // Task is Sleeping for X milliseconds.

// Priority classes
#define LOW_PRI     4               // Task will only run every fourth iteration.
#define NORMAL_PRI  2               // Task will run every other iteration.
#define HIGH_PRI    1               // Task will run every iteration.

// IPC Message Queue Constants
#define IPC_STAT    0x40            // Return Message Queue Status.
#define IPC_CLEAR   0x20            // Clear Message Queue.
#define IPC_CREAT   0x10            // Create Message Queue.

#define IPC_WAIT    0x04            // Calling process will wait to continue.
#define IPC_NOWAIT  0x02            // Calling process will continue immediately.
#define IPC_IMMEAD  0x01            // Calling process wants to be next or not at all.

// IPC Message Queue Status Results
#define IPC_QFAIL  -1               // Message Queue operation failed.
#define IPC_QSUCC   0               // Message Queue operation was a Success.
#define IPC_QEMPTY  1               // Message Queue is Empty.
#define IPC_QFULL   2               // Message Queue is Full.

int   initTaskSwitcher(void);

/**
 *  @brief MultiTasking Kernel
 *  
 *  Every Function submitted as a task to run will be run from "start to finish"
 *  with each iteration. Being a cooperative system no one task should hog the processor,
 *  and large/multipart tasks should be broken into states to control what part
 *  should execute next.
 *
 *  taskStruct definition
 *    jobID       = taskList index
 *    jobPointer  = address of function to run
 *    jobStatus   = Runable, Held, Sleeping
 *    jobDelay    = CNT + (CLKFREQ * #ofMilliSeconds)
 *    jobPriority = High, Normal, Low
 *    jobState    = Finite State Machine index used by task if necessary
 */
void  taskSwitch(void *par);

/**
 *  @brief Start a new task.
 *  
 *  Provide the function pointer to a function and this
 *  will add it to the tasklist
 */
int   taskStart( void (*func)(void), int priority=NORMAL_PRI, int stat=RUNABLE);

/**
 *  @brief Set task status.
 *  
 *  Pass the job ID of the task along with a value
 *  of zero to Hold the job, 1 to make Runable, or
 *  a positive number of milliseconds to delay 
 *  "sleep" the task before running again.
 */
int   taskSetStatus(int id, int value);

/**
 *  @brief Set task priority.
 *
 *  Set the task priority to control frequency at which the task runs.
 *  Values: 1=High, 2=Normal, and 3=Low.
 *  Default priority is Normal.
 */
int   taskSetPriority(int id, int value);

/**
 *  @brief Set task State.
 *
 *  Task state is information carried over from one iteration
 *  of a task to the next. Typically used to drive a 
 *  Finite State Machine (FSM) within the task.
 */
void  taskSetState(int id, int value);

/**
 *  @brief Get task Status.
 *  
 *  Will return the current value of a given tasks Status.
 *    0=Held, 1=Runable, 2=Sleeping
 */
int   taskGetStatus(int id);

/**
 *  @brief Get task Priority.
 *
 *  Will return the current value of a given tasks Priority.
 *  1=High, 2=Normal, 3=Low
 */
int   taskGetPriority(int id);

/**
 *  @brief Get task State.
 *  
 *  Will return the current value of a given tasks State parameter.
 *  The meaning of this value is task dependent.
 */
int   taskGetState(int id);

/**
 *  @brief Send Message to Destination msgQ
 *  
 */
int   msgsnd(int destid, int srcid, int pri, int typ, cmd_struct body);

/**
 *  @brief Receive Message from msgid msgQ
 *  
 */
cmd_struct   msgrcv(int msgid, int msgflg=0);

/**
 *  @brief Alter conditions of msgQ
 *  
 */
int   msgctl(int msgid, int msgcmd);

#if defined(__cplusplus)
}
#endif
/* __cplusplus */  
#endif
/* MYMTOS_H */  

/*
 * TERMS OF USE: MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
