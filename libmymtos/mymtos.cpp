/*
 *  mymtos.cpp - Cooperative Multi-Tasking kernel
 *  to allow concurrently running processes within a single cog.
 *  
 *  written by Paul Bammel -  February 2015
 *    - Simple cooperative kernel
 *    - All tasks (functions) run to completion before yielding
 *    - A function must provide for external state determination
 *      if it runs longer than a single iteration
 *      
 */

#include  "simpletools.h"
#include  "mymtos.h"

/*
 *taskStruct definition
 *  jobID = taskList index
 *  jobPointer = address of function to run
 *  jobStatus = Runable, Held, Sleeping
 *  jobDelay = CNT + (CLKFREQ * #ofSeconds)
 *  jobPriority = High, Normal, Low
 *  jobState = Finite State Machine index used by task if necessary
 */
struct  taskStruct{
  int   jobID;                                      // job (task) ID.
  void  (*jobPointer)(void);                        // Pointer to task function
  int   jobStatus;                                  // Runable, Held, etc.
  int   jobDelay;                                   // Delay task until systemtime exceeds
  int   jobPriority;                                // Higher Priority tasks run more often
  int   jobState;                                   // Finite state per task iterations
};  

static volatile struct  taskStruct  taskList[MAXTASKS];   // Array of task Entries
static volatile int     nextTask  = 0;                    // Next task entry (jobID).

// Stack space for Speed Control cog
unsigned int switch_stack[(160 + (50 * 4))];

/* Start taskSwitcher function in separate cog*/
int initTaskSwitcher(void){
  int switch_cogID = cogstart(&taskSwitch, NULL, switch_stack, sizeof(switch_stack));
  return(switch_cogID);
}

void taskSwitch(void *par){
  int task = 0;
  int execLoop = 1;                                 // Tracks loops through kernel
                                                    //  to control task priorities.

  while (1){
    for (task = 0; task < nextTask; task++){
      switch(taskList[task].jobStatus){             // Based on current task status
        case  RUNABLE:
          if (execLoop % taskList[task].jobPriority == 0) {
            taskList[task].jobPointer();            // Run current task
          }            
          break;
          
        case  SLEEPING:
          if(CNT >= taskList[task].jobDelay){       // If current systemtime >= delaytime
            taskList[task].jobDelay = 0;            // Reset Delay Time
            taskList[task].jobStatus = RUNABLE;     // Set task status back to Runable
            taskList[task].jobPointer();            // Run task
          }
          break;
          
        case  HELD:                                 // If current task is Held, skip it.
        default:                                    // Undefined status, skip task.
          break;
      }
    }
    if (execLoop++ > 4)                             // The execLoop count cycles from
      execLoop = 1;                                 //  1 to 4 to calculate job priority.
  }
}

/*
 *  Start a new task.
 *  Provide the function pointer to a function and this
 *  will add it to the tasklist
 */
int taskStart( void (*func)(void), int priority){
  if(nextTask < MAXTASKS){
    taskList[nextTask].jobID = nextTask;
    taskList[nextTask].jobPointer = func;
    taskList[nextTask].jobStatus = RUNABLE;
    taskList[nextTask].jobDelay = 0;
    taskList[nextTask].jobPriority = priority;
    taskList[nextTask].jobState = 0;
    return(nextTask++);                           // Return jobID & increment for next time.
  }
  return -1;                                      // Return - failed to start new task
}

/*
 *  Set task state.
 *  Pass the job ID of teh task along with a value
 *  of zero to Hold the job or a positive number of milliseconds
 *  to delay "sleep" the task before running again.
 */
int taskSetStatus(int id, int value){
  switch(value){
    case HELD:
      taskList[id].jobStatus = HELD;
      break;
    case RUNABLE:
      taskList[id].jobStatus = RUNABLE;
      break;
    default:
      taskList[id].jobStatus = SLEEPING;
      taskList[id].jobDelay = CNT+(CLKFREQ*(value/1000));
      break;
  }
}

/*
 *  Set task priority.
 *  Set the task priority to control frequency at which the task runs.
 *  Values: 1=High, 2=Normal, and 3=Low.
 *  Default priority is Normal.
 */
int taskSetPriority(int id, int value){
  if (value <= LOW_PRI) { 
    taskList[id].jobPriority = value;
  } else
    return (-1);
  return(value);
}

/*
 *  Set task State.
 *  Task state is information carried over from one iteration
 *  of a task to the next. Typically used to drive a 
 *  Finite State Machine (FSM) within the task.
 */
void taskSetState(int id, int value){
  taskList[id].jobState = value;
}  

/*
 *  Get task Status.
 *  Will return the current value of a given tasks Status.
 *    0=Held, 1=Runable, 2=Sleeping
 */
int   taskGetStatus(int id){
  return(taskList[id].jobStatus);
}

/*
 *  Get task Priority.
 *  Will return the current value of a given tasks Priority.
 *  1=High, 2=Normal, 3=Low
 */
int   taskGetPriority(int id){
  return(taskList[id].jobPriority);
}

/*
 *  Get task State.
 *  Will return the current value of a given tasks State parameter.
 *  The meaning of this value is task dependent.
 */
int   taskGetState(int id){
  return(taskList[id].jobState);
}