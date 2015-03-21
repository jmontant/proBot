/**
* This is the main Blank Simple C++ Project program file.
*/

#include "pimotor.h"
#include "robot_defs.h"
#include "simpletools.h"
#include "servo.h"
#include "emic_hndlr.h"
#include "mycompass.h"


int main(){
  
  initMotorControl();

  int Heading = 0;
  
  compass_init(MODE_CONT);
  voice(8);
/*
  say("Let me calibrate the compass.");
  servo_set(WHEEL_L_PIN, 1490);               // Rotate Left wheel slowly.
  servo_set(WHEEL_R_PIN, 1490);               // Rotate Right wheel slowly in the opposite direction.
  
  compass_calibrate();                        // Call compass calibration routine.

  servo_set(WHEEL_L_PIN, 1500);               // Stop the Left wheel
  servo_set(WHEEL_R_PIN, 1500);               // Stop the Right wheel
*/

  pause(4000);
  say("Forward, speed at 50% for two seconds.");
  motorMove(FORWARD,50);
  pause(2000);
  motorStop();


  pause(2000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  
  say("Rotate right ninety degrees.");
  motorRotate(RIGHT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading
  
  pause(2000);
  say("Rotate right ninety degrees.");
  motorRotate(RIGHT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate right ninety degrees.");
  motorRotate(RIGHT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate right ninety degrees.");
  motorRotate(RIGHT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate left ninety degrees.");
  motorRotate(LEFT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate left ninety degrees.");
  motorRotate(LEFT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate left ninety degrees.");
  motorRotate(LEFT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate left ninety degrees.");
  motorRotate(LEFT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate right forty five degrees.");
  motorRotate(RIGHT, 45);
  pause(1000);
  say("Rotate right forty five degrees.");
  motorRotate(RIGHT, 45);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  
  pause(2000);
  say("Rotate left forty five degrees.");
  motorRotate(LEFT, 45);
  pause(2000);
  say("Rotate left forty five degrees.");
  motorRotate(LEFT, 45);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading


  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn North.");
  motorRotateTo(NORTH);                              // Turn to face North (Zero degrees)

  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn East.");
  motorRotateTo(EAST);                               // Turn to face East (90 degrees)

  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn South.");
  motorRotateTo(SOUTH);                              // Turn to face South (180 degrees)
  
  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn West.");
  motorRotateTo(WEST);                               // Turn to face West (270 degrees)

  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn North.");
  motorRotateTo(NORTH);                              // Turn to face North (0 degrees)

  return 0;
}
  