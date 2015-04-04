/*
 *  mymtos.cpp - Cooperative Multi-Tasking kernel
 *  to allow concurrently running processes within a single cog.
 *  
 *  v1.00 Paul Bammel - February 2015
 *    - Simple cooperative kernel
 *    - All tasks (functions) run to completion before yielding
 *    - A function must provide for external state determination
 *      if it runs longer than a single iteration
 *  v1.01 Paul Bammel - March 2015
 *    - Added IPC message queue
 */

#include  "simpletools.h"
#include  "mymtos.h"
#include  "robot_defs.h"

/*
 *taskStruct definition
 *  jobID       = taskList index
 *  jobPointer  = address of function to run
 *  jobStatus   = Runable, Held, Sleeping
 *  jobDelay    = CNT + (CLKFREQ * #ofSeconds)
 *  jobPriority = High, Normal, Low
 *  jobState    = Finite State Machine index used by task if necessary
 */
struct  Task_Struct{
  int   jobID;                                      // job (task) ID.
  void  (*jobPointer)(void);                        // Pointer to task function
  int   jobStatus;                                  // Runable, Held, etc.
  int   jobDelay;                                   // Delay task until systemtime exceeds
  int   jobPriority;                                // Higher Priority tasks run more often
  int   jobState;                                   // Finite state per task iterations
};  

static volatile struct  Task_Struct  taskList[MAXTASKS];  // Array of task Entries
static volatile int     nextTask  = 0;                    // Next task entry (jobID).

/*
 *  Message Queue definition
 *    msgSrc  = Source ID of task message came from
 *    msgPri  = Message Priority
 *    msgNext = Pointer to the next message in teh queue
 *    msgBody = Comand/Status being passed to message receiver
 */
struct  MsgQ_Struct{
  int   msgSrc;
  int   msgPri;
  int   msgNext;
  struct cmd_struct msgBody;
};

static volatile struct MsgQ_Struct msgQue[MAXTASKS] [QUEDEPTH];
static volatile int msgHead[MAXTASKS];                  // Head of message list
static volatile int msgFree[MAXTASKS];                  // Head of Free nodes list

// Stack space for Speed Control cog
unsigned int switch_stack[(160 + (50 * 4))];

/* Start taskSwitcher function in separate cog*/
int initTaskSwitcher(void){
  int mtos_cogID = cogstart(&taskSwitch, NULL, switch_stack, sizeof(switch_stack));
  return(mtos_cogID);
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
int taskStart( void (*func)(void), int priority, int stat){
  if(nextTask < MAXTASKS){
    taskList[nextTask].jobID = nextTask;
    taskList[nextTask].jobPointer = func;
    taskList[nextTask].jobStatus = stat;
    taskList[nextTask].jobDelay = 0;
    taskList[nextTask].jobPriority = priority;
    taskList[nextTask].jobState = 0;
    msgHead[nextTask] = NULMSG;                   // Mark message queue as empty.
    return(nextTask++);                           // Return jobID & increment for next time.
  }
  return -1;                                      // Return - failed to start new task
}

/*
 *  Set task status.
 *  Pass the job ID of the task along with a value
 *  of zero to Hold the job, 1 to make Runable, or
 *  a positive number of milliseconds to delay 
 *  "sleep" the task before running again.
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

/*
 *  Send message to Dest msgQ from source msgQ
 *    - save copies of current head of msg list and free list.
 *    - Put new message in first free list node.
 *    - Insert into list based on priority
 */
int msgsnd(int destid, int srcid, int pri, int typ, cmd_struct msg){
  int nxtFree
      = msgQue[destid][msgFree[destid]].msgNext;      // Point to second node in free list
  int oldHead = msgHead[destid];                      // Temp save of current msgQue head
  int tmpPtr = NULMSG;                                // Temp pointer to traverse list
  int nxtPtr = NULMSG;                                // Temp pointer to traverse list
  
  /*  if typ = IMMEAD and que not empty, fail. */
  if((typ == IPC_IMMEAD) & (msgHead[destid] != NULMSG)){
    return(IPC_QFAIL);
  }
  
  if(msgFree[destid] != NULMSG){                      // If message queue still has room
    msgQue[destid][msgFree[destid]].msgSrc = srcid;
    msgQue[destid][msgFree[destid]].msgPri = pri;
    msgQue[destid][msgFree[destid]].msgBody.action = msg.action;
    msgQue[destid][msgFree[destid]].msgBody.direction = msg.direction;
    msgQue[destid][msgFree[destid]].msgBody.value1 = msg.value1;
    msgQue[destid][msgFree[destid]].msgBody.value2 = msg.value2;
    tmpPtr = msgFree[destid];                         // Temp pointer points at new node
    msgFree[destid] = nxtFree;                        // Update head of free list
  } else {
    return(IPC_QFULL);                                // Queue was full couldn't add msg
  }    
  
  if((msgHead[destid] == NULMSG) |                    // Message queue is empty or
    (msgQue[destid][msgHead[destid]].msgPri < pri)){  //  first node has a lower priority.
      oldHead = msgHead[destid];                      // Remember where Head is currently
      msgHead[destid] = tmpPtr;                       // Point head at new message
      msgQue[destid][tmpPtr].msgNext = oldHead;       // Point new msg.next at old head
  } else {
    while(msgQue[destid][oldHead].msgPri < pri){
      if(msgQue[destid][oldHead].msgNext != NULMSG){
        oldHead = msgQue[destid][oldHead].msgNext;    // Point to next message in queue.
      } else {
        break;                                        // Reached end of msq queue.
      }
    }
    nxtPtr = msgQue[destid][oldHead].msgNext;         // Remember pointer to next msg
    msgQue[destid][oldHead].msgNext = tmpPtr;         // Point current msq at new node
    msgQue[destid][tmpPtr].msgNext = nxtPtr;          // Point new msg.next at next in queue
  }
}

/*
 *  Receive message from msgid message Queue
 *    - Save pointer to second message node in list.
 *    - Pull out body of message to reeturn to task reading the mesage.
 *    - Save pointer to what was the top of the free nodes list.
 *    - Set top of free list to point to message node just read.
 *    - Make just read msg node point to old top of free list.
 *    - Make msg head point to next unread node in message list.
 */  
cmd_struct msgrcv(int msgid, int msgflg){
  int newHead;
  int oldFree;
  cmd_struct msg;

  /* If msg que empty return error */
  if(msgHead[msgid] == NULMSG){
    msg.action = IPC_QEMPTY;
    return(msg);
  }
  
  newHead = msgQue[msgid][msgHead[msgid]].msgNext;  // Save pointer to next msg node.
  msg.action = msgQue[msgid][msgHead[msgid]].msgBody.action;
  msg.direction = msgQue[msgid][msgHead[msgid]].msgBody.direction;
  msg.value1 = msgQue[msgid][msgHead[msgid]].msgBody.value1;
  msg.value2 = msgQue[msgid][msgHead[msgid]].msgBody.value2;
  oldFree = msgFree[msgid];                         // Save pointer to top of free list
  msgFree[msgid] = msgHead[msgid];                  // New top of free is last msg node
  msgQue[msgid][msgFree[msgid]].msgNext = oldFree;  // Point last msg node to old top of free.
  msgHead[msgid] = newHead;                         // Point msg head to next msg node.
  return(msg);
}

/*
 *  Configuration of Message Queue (msgid)
 *    - STAT  returns count of entries in list
 *    - CLEAR empties list of all messages
 */
int msgctl(int msgid, int msgcmd){
  int tmp;
  
  switch(msgcmd){
    case IPC_STAT:
      if(msgHead[msgid] == NULMSG){
        return(0);                                // No messages in queue.
      } else {
        for(tmp = 0; tmp < QUEDEPTH; tmp++){      // Find end of list.
          if(msgQue[msgid][tmp].msgNext == NULMSG){
            return(++tmp);                        // Return message count.
          }
        }                      
      }        
      break;
    case IPC_CLEAR:
      msgHead[msgid] = NULMSG;
      msgFree[msgid] = 0;
      break;
    default:
      break;
  }      
}  
