/**
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

#define MAXTASKS 10

#define HELD      0
#define RUNABLE   1
#define SLEEPING  2


//Priority classes
#define LOW_PRI     3
#define NORMAL_PRI  2
#define HIGH_PRI    1

int   initTaskSwitcher(void);
void  taskSwitch(void *par);
int   taskStart( void (*func)(void), int priority=NORMAL_PRI);
int   taskSetStatus(int id, int value);
int   taskSetPriority(int id, int value);
void  taskSetState(int id, int value);
int   taskGetStatus(int id);
int   taskGetPriority(int id);
int   taskGetState(int id);

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
