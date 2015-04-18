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
  firstID     = taskStart(firstOne);
  listenerID  = taskStart(listener);
  thirdID     = taskStart(third);
 
  /*
   * Body of main function loops endlessly looking for something to do and reacting
   * based on a subsumption hierarchy of priority.
   */

  while(1){
/*    
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
*/
//    msgList(listenerID);
    loopcnt++;
    pause(500);
  }    
  
  return 0;
}


void  firstOne(void){
  int x = 0;
  int sndRslt = 0;
  int sndpri = 1;
  cmd_struct body;
  
  body.action = MOVE;
  body.direction = FORWARD;
  body.value1 = 4;
  x = msgCtl(listenerID, IPC_STAT);
//  if(x < 3){
    sndRslt = msgSnd(listenerID, firstID, sndpri, IPC_NOWAIT, body);
    taskSetState(thirdID, sndRslt);
//    x = taskGetState(firstID);
//    taskSetState(firstID, 1);
//    if(sndRslt != IPC_QSUCC){
      taskSetState(firstID, x);
//    }
//  }
  taskSetStatus(firstID, 1000);
}

void  listener(void){
  cmd_struct  todo;
  int x = 0;

  x = msgCtl(listenerID, IPC_STAT);
  taskSetState(listenerID, x);
  
//  msgList(listenerID);
/*  
  todo = msgRcv(listenerID);                // Read command off of mesage queue
  if(todo.action != IPC_QEMPTY){            // The msgQ wasn't empty
//    x = taskGetState(listenerID);
    taskSetState(listenerID, 2);
  }
  if(todo.action == IPC_QEMPTY){
    taskSetState(listenerID, 99);
  }
*/
}

void  third(void){
  int x;
  
  x = taskGetState(thirdID); 
//  taskSetState(thirdID, x+=1); 
}
