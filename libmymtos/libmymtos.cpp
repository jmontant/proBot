/*
 * mymtos - cooperative multitasking kernel.
 */

#include  "mymtos.h"
#include  "simpletools.h"
#include  "robot_defs.h"

void  arbitrate(void);
void  cruise(void);
void  avoid(void);

int   moveInput     = 0;
int   cruiseID      = 0;
int   cruiseCommand = 0;
int   cruiseFlag    = 0;
int   avoidID       = 0;
int   avoidCommand  = 0;
int   avoidFlag     = 0;

int main(){
  int loopcnt = 0;
  
  initTaskSwitcher();

  /*
   *  Start up functions that need to run in
   *  multi-tasking kernel simultaneously
   */
  cruiseID = taskStart(cruise, LOW_PRI);
  avoidID = taskStart(avoid, HIGH_PRI);
 
  /*
   * Body of main function loops endlessly looking for something to do and reacting
   * based on a subsumption hierarchy of priority.
   */

  while(1){
    print("Let's check arbitration\n");
    arbitrate();
    print("moveInput = %d\n", moveInput);
    print("Loop count = %d\n", loopcnt++);
    if(loopcnt > 10){
      print("Loop count triggered job held\n");
      taskSetStatus(avoidID, HELD);
    }      
//    if(loopcnt > 20){
//      taskSetStatus(avoidID, 2000);
//    }
    print("Cruise Task Status = %d, Priority = %d, State = %d\n", 
      taskGetStatus(cruiseID), 
      taskGetPriority(cruiseID),
      taskGetState(cruiseID));
    print("Avoid Task Status = %d, Priority = %d, State = %d\n", 
      taskGetStatus(avoidID), 
      taskGetPriority(avoidID),
      taskGetState(avoidID));
    pause(500);
  }    
  
  return 0;
}

void  arbitrate(void){
  if(cruiseFlag == 1){
    moveInput = cruiseCommand;
  }    
  if(avoidFlag == 1){
    moveInput = avoidCommand;
  }
}      

void  cruise(void){
  int x = 0;
  cruiseCommand = 21;
  cruiseFlag = 1;
  x = taskGetState(cruiseID);
  taskSetState(cruiseID, x+=1);
}

void  avoid(void){
  int x = 0;
  
  avoidCommand = 41;
  avoidFlag = 1;
  x = taskGetState(avoidID);
  taskSetState(avoidID, x+=1);
}
