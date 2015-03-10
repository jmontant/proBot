/*
 * MySonar - Runs the Ping))) Ultrasonic sensor in its own cog.
 */

#include  "mysonar.h"
#include  "robot_defs.h"
#include  "simpletools.h"

struct cmd_struct snrCommand;
struct cmd_struct snrResponse;

cmd_struct sonarAction(int action, int dir=0, int value1=0, int value2= 0){
  snrCommand.action = action;
  snrCommand.direction = dir;
  snrCommand.value1 = value1;
  snrCommand.value2 = value2;
  sonarCommand(snrCommand);

}

int main(){
  int turnAngle = 0;
  struct target sonar;
  
  initSonarControl();                  // Start sonar_control in separate cog.
  pause(500);

  sonarAction(POINT,0, 90);               // Face Ping))) sensor straight ahead.
  
  sonarAction(SCAN, 0, 30);
  while(sonarAction(GETFUNC).action != STOP){
    continue;                             // Wait for sweep to finish.
  }
  pause(500);
/*
  sonarAction(PING);                      // Begin repeatedly pinging and storing current distance.
  for(int i; i < 20; i++){
    sonar = sonarAction(TARGET, 0, CURRENT);
    print("Distance = %d\n", sonar.dist);   // Grab current distance reading.
    pause(500);
  }
  sonarAction(STOP);                      // Stop repeatedly pinging.
  
  sonarAction(POINT, 0, 45);
  print("Immediate Distance = %d\n", sonarPingNow());   // Return immediate ping result.
*/  
//  sonarScan();                          // 180 degree sweep of area in front of robot for objects.
  
  while(sonarAction(GETFUNC).action != STOP){
    continue;                             // Wait for sweep to finish.
  }
      
//  turnAngle = sonarAction(TARGET, 0, CLOSEST); // Return angle to target center (pos=right, neg=left)
  
  print("turnAngle = %d\n", turnAngle);
  
  return 0;
}

