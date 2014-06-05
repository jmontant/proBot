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

  rotate(LEFT, 90);
  pause(4000);

  rotate(RIGHT, 90);
  pause(4000);

  stop();

  return 0;
}
