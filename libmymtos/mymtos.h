/*
 * @file mymtos.h
 *
 * @author Paul Bammel
 *
 * @version v1.00
 *
 * @brief This library provides cooperative multitasking kernel 
 * functions for running multiple concurrent tasks in a single cog.
 *  
 *  written by Paul Bammel -  February 2015
 *    - Simple cooperative kernel
 *    - All tasks (functions) run to completion before yielding
 *    - A function must provide for external state determination
 *      if it runs longer than a single iteration
 */

#ifndef MYMTOS_H
#define MYMTOS_H

#if defined(__cplusplus)
extern "C" {
#endif

//General MTOS kernel constants
#define MAXTASKS  8               // Maximum number of tasks for MTOS kernel
#define QUEDEPTH  3               // Maximum depth of message queues

// Task Status settings
#define HELD        0
#define RUNABLE     1
#define SLEEPING    2

// Priority classes
#define LOW_PRI     3
#define NORMAL_PRI  2
#define HIGH_PRI    1

// Message Queue Constants
#define IPC_STAT    10            // Return Message Queue Status
#define IPC_CLR     11            // Clear messageQ

#define IPC_WAIT    20            // Calling process will wait to continue
#define IPC_NOWAIT  21            // Calling process will continue immediately


int   initTaskSwitcher(void);

/**
 *  @brief MultiTasking Kernel
 *  
 *  
 *
 *  taskStruct definition
 *    jobID = taskList index
 *    jobPointer = address of function to run
 *    jobStatus = Runable, Held, Sleeping
 *    jobDelay = CNT + (CLKFREQ * #ofSeconds)
 *    jobPriority = High, Normal, Low
 *    jobState = Finite State Machine index used by task if necessary
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
int   msgsnd(int destid, int srcid, int msgtyp, struct cmd_struct msgbody);

/**
 *  @brief Receive Message from msgid msgQ
 *  
 */
struct cmd_struct   msgrcv(int msgid, int msgflg);

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
