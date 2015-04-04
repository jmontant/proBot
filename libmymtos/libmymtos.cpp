/*
 * mymtos - cooperative multitasking kernel.
 */

#include  "mymtos.h"
#include  "simpletools.h"
#include  "robot_defs.h"


void  firstOne(void);
void  listener(void);
void  third(void);

int   firstID       = 0;
int   listenerID    = 0;
int   thirdID       = 0;

int main(){
  int loopcnt = 0;
  
  initTaskSwitcher();

  /*
   *  Start up functions that need to run in
   *  the multi-tasking kernel
   */
  firstID     = taskStart(firstOne, LOW_PRI);
  listenerID  = taskStart(listener, HIGH_PRI);
  thirdID     = taskStart(third);
 
  /*
   * Body of main function loops endlessly looking for something to do and reacting
   * based on a subsumption hierarchy of priority.
   */

  while(1){
    
    print("Let's check MTOS operations\n");
    print("FirstOne Task Status = %d, Priority = %d, State = %d\n", 
      taskGetStatus(firstID), 
      taskGetPriority(firstID),
      taskGetState(firstID));
    print("Listener Task Status = %d, Priority = %d, State = %d\n", 
      taskGetStatus(listenerID), 
      taskGetPriority(listenerID),
      taskGetState(listenerID));
    print("Third Task Status = %d, Priority = %d, State = %d\n", 
      taskGetStatus(thirdID), 
      taskGetPriority(thirdID),
      taskGetState(thirdID));
    print("\n");
    loopcnt++;
    pause(500);
  }    
  
  return 0;
}


void  firstOne(void){
  int x = 0;
  int sndRslt;
  int sndpri = 1;
  cmd_struct body;
  
  body.action = MOVE;
  body.direction = FORWARD;
  body.value1 = 4;
  sndRslt = msgCtl(firstID, IPC_STAT);
  if(sndRslt < 2){
    sndRslt = msgSnd(listenerID, firstID, sndpri, IPC_NOWAIT, body);
    taskSetState(firstID, x++);
    if(sndRslt != IPC_QSUCC){
      taskSetState(firstID, sndRslt);
    }
  }
}

void  listener(void){
  cmd_struct  todo;
  int x = 0;
  
  todo = msgRcv(listenerID);                // Read command off of mesage queue
  if(todo.action != IPC_QEMPTY){            // The msgQ wasn't empty
    taskSetState(listenerID, x++);
  }
}

void  third(void){
  int x = 0;
  
  taskSetState(thirdID, x++);
}
