#include "simpletools.h"                    // General propeller & C++ functions
#include "servo.h"                          // Control up to 14 servos in another core
#include "mycompass.h"                      // HMC5883L 3-Axis compass module functions
#include "robot_defs.h"                     // General robot definitions and I/O pin assignments
#include "mymotor.h"                        // Header file for this pimotor.cpp file


void  motorSetMode(unsigned char mode){
/*
 *  Establish motor control mode - See motor control contants in header file.
 *  Standard, Proportional, Proportional and Integral, etc.
 */
  mMode = mode;
}

int motorSetPosition(float x, float y){
/*
 *  Set new x,y coordinates, if no values provided assume 0,0 (new origin)
 *  Global heading will always be set to current compass heading.
 *  Relative heading will only be reset if establishing a new origin.
 */
  gps.xPos = x;                                         // Set/Reset global x position coordinate.
  gps.yPos = y;                                         // Set/Reset global y position coordinate.
  gps.gHeading = compass_smplHeading();                 // Get current global heading from compass.
  if (x==0 && y==0){                                    // If setting the origin location...
    gps.rHeading = 0;                                   //  Set relative heading to zero.
    orgHeading = gps.gHeading;                          //  Remember origin compass heading.
  }    
}
  
int   motorSetHeading(void){
/*
 *  Update global heading with current value from compass module
 */
  gps.gHeading = compass_smplHeading();                 // Get current global heading from compass.
}
  
pose  motorGetPose(void){
/*
 *  Return current Pose structure of gps coordinantes
 *  Only return when semaphore flag is true.
 */
  while(gps.validPos == 0){                             // If semaphore flag False,
    pause(1);                                           //  wait a moment and try again.
  }    
  return(gps);                                          // Return pointer to valid gps structure.
}  

// Set a direction and motion velocity
void motorMove(int dir, int vel, int dist){
  if(dir == FORWARD) mSign = 1; else mSign = -1;
  des_vel_clicks = CLICKS * abs(vel);                   // Convert vel to # of Clicks equivalent.
  if (vel > V_MAX){
    des_vel_clicks = CLICKS * V_MAX;                    // Limit max servo velocity
  } else if (vel < -V_MAX){
    des_vel_clicks = CLICKS * V_MAX;                    // and convert to # of Clicks equivalent.
  }    
  desInchDist = dist;                                   // Set desired distance if provided.
  curInchDist = 0.0;                                    // Reset current distance traveled.
  des_bias_clicks = 0;                                  // Start bias at zero for a straight line.
  integral = 0.0;                                       // Reset Integral to zero.
  mFunc = dir;                                          // Motor function equal to direction of travel.
}

// Adjust bias to make robot swerve left or right.
void motorSetBias(float bias){
  des_bias_clicks = CLICKS * bias;                      // Express bias in clicks per interval.
}

void  motorRotate(int dir, int deg){
/*
 *  Rotate Left/Right a specified number of degrees
 *  Face a particular compass heading
 */
  int angleDiff = 0;
  
  curHeading = compass_smplHeading();                   // Get current heading from compass

  switch(dir){
    case RIGHT:
      desHeading = curHeading + deg;                      // Add turn degrees to current heading
      if (desHeading > 360){
        desHeading = desHeading - 360;                    // Adjust value if result is greater than 360
      }
      mFunc = dir;
    case LEFT:
      desHeading = curHeading - deg;                      // Subtract turn degrees from current heading
      if (desHeading < 0){
        desHeading = desHeading + 360;                    // Adjust value if result is negative
      }
      mFunc = dir;
    case FACE:
      desHeading = deg;
      angleDiff = compass_diff(curHeading, desHeading);     // Diff between current & Desired heading
      if(angleDiff < 0){                                    // Right is the shortest turn direction.
        mFunc = RIGHT;
      } else {                                              // Left is the shortest turn direction
        mFunc = LEFT;
      }
  }
}


// Return current motor function
int motorGetFunction(void){
  return mFunc;                                         // Return current motor function
}

// Force motors to stop right away.
int motorStop(void){
  mFunc = STOP;                                         // Set motor function to Stop
}
