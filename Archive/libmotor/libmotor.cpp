/**
* This is the robot Proportional - Integral motor drive
* test harness code.
*/

#include <simpletools.h>
#include <motor.h>

int main(){
  init_speedControl();                  // Fire up Speed Control in a new cog.

  pause(1000);

//  move(50, 0);                          // Begin moving forward
//  pause(10000);
//  stop();

  rotate(RIGHT, 90);
  pause(7000);

  rotate(LEFT, 90);
  pause(7000);

  return 0;
}
