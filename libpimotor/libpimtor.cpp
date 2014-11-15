/**
* This is the main Blank Simple C++ Project program file.
*/

#include "pimotor.h"
#include "robot_defs.h"
#include "simpletools.h"
#include "servo.h"
#include "mycompass.h"


int main(){
  
  init_speedControl();

  servo_set(WHEEL_L_PIN, 1490);               // Rotate Left wheel slowly.
  servo_set(WHEEL_R_PIN, 1490);               // Rotate Right wheel slowly in the opposite direction.
  
  compass_calibrate();                        // Call compass calibration routine.

  servo_set(WHEEL_L_PIN, 1500);               // Stop the Left wheel
  servo_set(WHEEL_R_PIN, 1500);               // Stop the Right wheel
  
  pause(1000);

  move(FORWARD, 50);                  // Move forward at 50% speed.
/*
  pause(4000);                        // After 4 seconds
  move(FORWARD, 80);                  // Increase speed to 80%
  pause(4000);                        // and then after another 4 seconds
  move(FORWARD, 100);                 // increase speed to 100%
  pause(4000);                        // and after yet another 4 seconds
*/
  stop();                             // Stop moving.
  
  pause(1000);                        // Wait 2 seconds before beginning to turn.

  rotate(LEFT, 90);                  // Rotate Right 90 degrees.
  pause(500);
  rotate(RIGHT, 90);

  return 0;
}
  