/*
 * MySonar - Runs the Ping))) Ultrasonic sensor in its own cog.
 */

#include  "mysonar.h"
#include  "robot_defs.h"
#include  "simpletools.h"


int main(){
  int turnAngle = 0;
  struct target sonar;
  
  initSonarControl();                  // Start sonar_control in separate cog.
  pause(500);

  sonarPointAt(90);                     // Face Ping))) sensor straight ahead.
  while(sonarGetFunction() != STOP){
    continue;                           // Wait for sweep to finish.
  }
  
  sonarScan(30);
  while(sonarGetFunction() != STOP){
    continue;                           // Wait for sweep to finish.
  }
  pause(500);
/*
  sonarPing();                          // Begin repeatedly pinging and storing current distance.
  for(int i; i < 20; i++){
    sonar = sonarGetTarget(CURRENT);
    print("Distance = %d\n", sonar.dist);   // Grab current distance reading.
    pause(500);
  }
  sonarStop();                          // Stop repeatedly pinging.
  
  sonarPointAt(45);
  print("Immediate Distance = %d\n", sonarPingNow());   // Return immediate ping result.
*/  
//  sonarScan();                          // 180 degree sweep of area in front of robot for objects.
  
  while(sonarGetFunction() != STOP){
    continue;                           // Wait for sweep to finish.
  }
      
//  turnAngle = sonarFindTarget(CLOSEST); // Return angle to target center (pos=right, neg=left)
  
  print("turnAngle = %d\n", turnAngle);
  
  return 0;
}

