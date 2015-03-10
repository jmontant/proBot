/*
 *  MyMotor - Full control of robot wheel motors (servos)
 *  from an independent cog.
 */
#include "servo.h"
#include "EmicHndlr.h"
#include "mycompass.h"

#include "mymotor.h"
#include "robot_defs.h"
#include "simpletools.h"

struct cmd_struct mtrCommand;

int motorAction(int action, int dir=0, int value1=0, int value2= 0){
  mtrCommand.action = action;
  mtrCommand.direction = dir;
  mtrCommand.value1 = value1;
  mtrCommand.value2 = value2;
  motorCommand(mtrCommand);

}

int main(){
  
  initMotorControl();
  
  motorAction(MODE, 0, PI_MOTOR);

  int Heading = 0;
  
  voice(8);
  
  say("Move Forward, speed at 50% for a few seconds.");
  motorAction(MOVE, FORWARD, 50);
  pause(4000);
  motorAction(STOP);

/*
  say("Let me calibrate the compass.");
  servo_set(WHEEL_L_PIN, 1490);               // Rotate Left wheel slowly.
  servo_set(WHEEL_R_PIN, 1490);               // Rotate Right wheel slowly in the opposite direction.
  
  compass_calibrate();                        // Call compass calibration routine.

  servo_set(WHEEL_L_PIN, 1500);               // Stop the Left wheel
  servo_set(WHEEL_R_PIN, 1500);               // Stop the Right wheel
*/

  pause(4000);
  say("Forward, speed at 50% for two inches.");
  motorAction(MOVE, FORWARD, 50, 2);
  pause(3000);


  pause(2000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  
  say("Rotate right ninety degrees.");
  motorAction(TURN, RIGHT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading
  
  pause(2000);
  say("Rotate right ninety degrees.");
  motorAction(TURN, RIGHT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate right ninety degrees.");
  motorAction(TURN, RIGHT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate right ninety degrees.");
  motorAction(TURN, RIGHT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate left ninety degrees.");
  motorAction(TURN, LEFT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate left ninety degrees.");
  motorAction(TURN, LEFT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate left ninety degrees.");
  motorAction(TURN, LEFT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate left ninety degrees.");
  motorAction(TURN, LEFT, 90);
  say("I'm currently facing,");
  Heading = compass_smplHeading();                   // Get current heading from compass
  sayInt(Heading);                                   // Announce current compass heading

  pause(2000);
  say("Rotate right forty five degrees.");
  motorAction(TURN, RIGHT, 45);
  pause(1000);
  say("Rotate right forty five degrees.");
  motorAction(TURN, RIGHT, 45);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  
  pause(2000);
  say("Rotate left forty five degrees.");
  motorAction(TURN, LEFT, 45);
  pause(2000);
  say("Rotate left forty five degrees.");
  motorAction(TURN, LEFT, 45);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading


  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn North.");
  motorAction(TURN, FACE, NORTH);                    // Turn to face North (Zero degrees)

  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn East.");
  motorAction(TURN, FACE, EAST);                     // Turn to face East (90 degrees)

  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn South.");
  motorAction(TURN, FACE, SOUTH);                    // Turn to face South (180 degrees)
  
  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn West.");
  motorAction(TURN, FACE, WEST);                     // Turn to face West (270 degrees)

  pause(4000);
  Heading = compass_smplHeading();                   // Get current heading from compass
  say("I'm currently facing,");
  sayInt(Heading);                                   // Announce current compass heading
  say("and will now turn North.");
  motorAction(TURN, FACE, NORTH);                    // Turn to face North (0 degrees)

  return 0;
}
