/**
* This is the main Blank Simple C++ Project program file.
*/

#include "io_pins.h"
#include "pimotor.h"
#include "robot_defs.h"
#include "simpletools.h"


int main(){
  
  init_speedControl();
  
  move(FORWARD, 50);                  // Move forward at 50% speed.

  pause(4000);                        // After 4 seconds
  move(FORWARD, 80);                  // Increase speed to 80%
  pause(4000);                        // and then after another 4 seconds
  move(FORWARD, 100);                 // increase speed to 100%
  pause(4000);                        // and after yet another 4 seconds
  stop();                             // Stop moving.
  
  pause(2000);                        // Wait 2 seconds before beginning to turn.
  rotate(RIGHT, 90);                  // Rotate Right 90 degrees.

  return 0;
}
  