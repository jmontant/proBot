/**
* Test harness for motor control library.
*/

#include "servo.h"
#include "motor.h"
#include "simpletools.h"

int enc_cnt;
int des_cnt;


int main()
{
  servo_start();                        // Start servo cog, if not already started.
  init_speedControl();                  // Fire up Speed Control in a new cog.

  move(100, 0);                          // Move forward at 50% of full speed.
  pause(5000);                          // Continue for 3 seconds.
  stop();                               // Stop forward motion.

  move(-100, 0);                         // Move backward at 50% of full speed.
  pause(3000);                          // Continue for 3 seconds.
  stop();                               // Stop forward motion.

  rotate(LEFT, 90);
  pause(3000);

  rotate(RIGHT, 90);
  pause(3000);

  rotate(RIGHT, 180);
  pause(3000);

  stop();

  return 0;
}
