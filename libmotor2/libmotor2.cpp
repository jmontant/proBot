/**
* Test harness for motor control library.
*/

#include "servo.h"
#include "motor.h"
#include "simpletools.h"

const int ON  = 22;                     // LCD On code.
const int CLR = 12;                     // LCD Clear Screen code.

int enc_cnt;
int des_cnt;


int main()
{
  serial *lcd = serial_open(3, 3, 0, 9600);
  writeChar(lcd, ON);
  writeChar(lcd, CLR);
  pause(5);

  servo_start();                        // Start servo cog, if not already started.
  init_speedControl();                  // Fire up Speed Control in a new cog.

  dprint(lcd,"Turn Left\n");
  rotate(LEFT, 90);
  pause(4000);
/*
  while (1)
  {
    enc_cnt = get_debug_cnt();
    des_cnt = get_turn_cnt();
    print("cnt = %d, turn_count = %d, mFunc = %d\n", enc_cnt, des_cnt, get_mFunc());
  }
*/
  dprint(lcd,"Turn Right\n");
  rotate(RIGHT, 90);
  pause(4000);

  stop();

  return 0;
}
