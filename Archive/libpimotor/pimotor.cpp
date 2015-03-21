/*
  Proportional - Integral - Derivative (PID) motor control with Compass rotate
  
  Proportional error is the difference between the desired velocity and the current velocity.
  Integral error is the cummulative difference between the Left and Right velocities.
  Derivative (slope) difference between the current proportional error and the previous one.
  All of these values are multiplied by a constant to adjust their affect on the motor speed.

  written by Paul Bammel March 2014       // Initial version modeled from "Mobile Robots" by J. Jones & A. Flynn
  updated by Paul Bammel August 2014      // Modified to handle rotation using the compass module
  updated by Paul Bammel December 2014    // Code clean-up and optimizations
  updated by Paul Bammel January 2015     // Added Derivative code to provide smoothing of motor responses.

*/

#include "simpletools.h"
#include "servo.h"
#include "mycompass.h"
#include "robot_defs.h"
#include "pimotor.h"

/* Private pimotor function prototypes */
int   limit_range(int val, int low, int high);                // Limit range of supplied value
void  set_servo(int vel, int motor_index);                    // Set the speed of a single servo (0-100%)
void  alter_power(int error, int motor_index);                // Set and remember power level
float integrate(float left_vel, float right_vel, float bias); // Itegrate left and right servo velocity to synchronize servos
void  motorControl(void *par);                                // Process loop that runs in its own cog and provide motor funtions
float get_velClicks(int motor_index);                         // Returns number of encoder "clicks" since last pass through loop.
void  init_encoders(void);                                    // Initialize encoders to count "clicks" to determine wheel velocity


// Stack space for Speed Control cog
unsigned int stack[(160 + (50 * 4))];

int   control_interval  = 250;            // Pause between servo loop intervals

int   mFunc             = STOP;           // Servo function to perform
int   mPower[2]         = {0, 0};         // Servo power for Left/Right servo(as a percentage of 100)
int   mSign             = 0;              // Sign to indicate motor direction (1=Forward, -1=Backward)

float des_vel_clicks    = 0.0;            // Desired velocity in clicks/interval
float des_bias_clicks   = 0.0;            // Desired bias in clicks/interval
int   des_dist_clicks   = 0;              // Desired distance in encoder clicks. Zero = ignore.
int   dist_clicks       = 0;              // Cumulative distance traveled in clicks
float integral          = 0.0;            // Integral of velocity difference between servos
int   curHeading        = 0;              // Current compass heading
int   desHeading        = 0;              // Desired (new) compass heading

int   gps_x             = 0;              // Global Position x-coordinate
int   gps_y             = 0;              // Global Position y-coordinate

/* Limit range of value */
int limit_range(int val, int low, int high){
  if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

/* Set the speed of a single servo (0-100%) based on direction and velocity provided */
void set_servo(int vel, int motor_index){
  if (motor_index == 0){                  // Motor index = 0 (Left servo)
    if (mFunc == FORWARD){
      servo_speed(WHEEL_L_PIN, vel);      // Set left servo speed forward
    } else {
      servo_speed(WHEEL_L_PIN, -vel);     // Set left servo speed reverse
    }
  } else {                                // Motor index = 1 (Right servo)
    if (mFunc == FORWARD){
      servo_speed(WHEEL_R_PIN, -vel);     // Set right servo speed forward
    } else {
      servo_speed(WHEEL_R_PIN, vel);      // Set right servo speed reverse
    }
  }
}

/* Set and remember power level */
void alter_power(int err, int motor_index){
  int tmp_pwr = 0;                        // Temporary servo power value
  
  tmp_pwr = ((mPower[motor_index] * CLICKS) + err) / CLICKS;
  mPower[motor_index] = limit_range(tmp_pwr, -200, 200);
  set_servo(mPower[motor_index], motor_index);
}

/* Itegrate left and right servo velocity to synchronize servos */
float integrate(float left_vel, float right_vel, float bias){
  integral = integral + left_vel + bias - right_vel;
  return integral;
}

/* Start SpeedControl function in separate cog*/
void initMotorControl(void){
  int cog = cogstart(&motorControl, NULL, stack, sizeof(stack));
}

/* MotorControl running in independent cog */
void motorControl(void *par){
  float integral_error = 0.0;                                   // Integral error between servos
  float left_error = 0.0, right_error = 0.0;                    // Proportional velocity error values
  int   left_velClicks = 0, right_velClicks = 0;                // Current Left & Right velocity (in clicks)
  int   left_last = 0, right_last = 0;                          // Previous Left & Right velocity (in clicks)
  int   angleDiff = 0;                                          // Diff between current & desired heading
  int   turnSpeed = 0;                                          // Speed robot should be turning at

  init_encoders();                                              // Set up wheel encoders.
  compass_init(0);                                              // Initialize compass on bus.

  while(1){
    left_velClicks = get_velClicks(0);                          // Get current left velocity (in clicks)
    right_velClicks = get_velClicks(1);                         // Get current right velocity (in clicks)
    
    switch (mFunc){                                             // Based on desired motor function
      case FORWARD:                                             // Move robot Forward
      case BACKWARD:                                            //  or Backward.

//        integral_error = K_INT *                                // Integrate Left & Right velocity
//          integrate(left_vel, right_vel, des_bias_clicks);      //  also introduce possible bias.
        left_error = K_PRO *
          (des_vel_clicks - left_velClicks - integral_error);   // Proportional speed adjustment of left servo
        right_error = K_PRO *
          (des_vel_clicks - right_velClicks + integral_error);  // Proportional speed adjustment of right servo

        alter_power(left_error, 0);                             // Alter servo speeds as necessary
        alter_power(right_error, 1);
        break;
        
      case LEFT:                                                // Rotate Left or
      case RIGHT:                                               //  Right desired number of degrees.
        curHeading = compass_smplHeading();                     // Get current heading from compass
        angleDiff = compass_diff(curHeading, desHeading);       // Diff between cur & Desired heading
        if (mFunc == LEFT){                                     // Rotate Left desired number of degrees.
          while(angleDiff > 0){
            turnSpeed = abs(angleDiff)/2;                       // Use half the turn angle as turn speed.
            if(turnSpeed < 10) turnSpeed = 10;                  // Unless the turn angle is less than 10.
            servo_set(WHEEL_L_PIN, 1500-turnSpeed);             // Rotate Left wheel
            servo_set(WHEEL_R_PIN, 1500-turnSpeed);             // Rotate Right wheel
            pause(8);                                           // Make sure there is at least 8ms between compass readings.
            curHeading = compass_smplHeading();                 // Get current heading from compass
            angleDiff = compass_diff(curHeading, desHeading);   // Diff between Current & Desired heading
          }
        } else {
          if ( mFunc == RIGHT){                                 // Rotate Right desired number of degrees.
            while(angleDiff < 0){
              turnSpeed = abs(angleDiff)/2;                     // Use half the turn angle as turn speed.
              if(turnSpeed < 10) turnSpeed = 10;                // Unless the turn angle is less than 10.
              servo_set(WHEEL_L_PIN, 1500+turnSpeed);           // Rotate Left wheel
              servo_set(WHEEL_R_PIN, 1500+turnSpeed);           // Rotate Right wheel
              pause(8);                                         // Make sure there is at least 8ms between compass readings.
              curHeading = compass_smplHeading();               // Get current heading from compass
              angleDiff = compass_diff(curHeading, desHeading); // Diff between Current & Desired heading
            }
          } else {
            mFunc = STOP;                                       // Rotation complete or invalid parameters.
            servo_set(WHEEL_L_PIN, 1500);                       // Force Left servo to stop
            servo_set(WHEEL_R_PIN, 1500);                       // Force Right servo to stop
          }
        }          
        break;
        
      case STOP:                                                // Stop servo motion immediately
      default:
        mFunc = STOP;                                           // Set motor function to stop
        servo_set(WHEEL_L_PIN, 1500);                           // Force Left servo to stop
        servo_set(WHEEL_R_PIN, 1500);                           // Force Right servo to stop
        integral = 0.0;                                         // Reset Integral to zero
        break;
    }
    pause(control_interval);
  }
}

// Initialize counters in motor cog to track Left & Right encoders
void init_encoders(void){
  FRQA = 1;                             // Add 1 every time we see an edge (hole in left wheel)
  PHSA = 0;                             // Always start the edge count at zero.
  FRQB = 1;                             // Add 1 every time we see an edge (hole in right wheel)
  PHSB = 0;                             // Always start the edge count at zero.

  CTRA = 0x28000000 + ENC_L_PIN;        // Configure Left wheel counter for positive edges
  CTRB = 0x28000000 + ENC_R_PIN;        // Configure Right wheel counter for positive edges
}

// Return current velocity of a particular motor in clicks/interval
float get_velClicks(int motor_index){
  int count;

  if (motor_index == 0){
    count = PHSA;                       // Retrieve count for left encoder
    PHSA = 0;                           // Clear left encoder
    return count;                       // Return count for left servo
  } else {
    count = PHSB;                       // Retrieve count for right encoder
    PHSB = 0;                           // Clear right encoder
    return count;                       // Return count for right servo
  }
}

// Set a direction and motion velocity
void motorMove(int dir, int vel){
  if(dir == FORWARD) mSign = 1; else mSign = -1;
  des_vel_clicks = CLICKS * abs(vel);   // Express percentage velocity in clicks per control interval.
  des_bias_clicks = 0;                  // Make sure bias is zero so robot moves in a straight line.
  mFunc = dir;                          // Set motor function to direction robot should travel.
}

// Adjust bias to make robot swerve left or right.
void motorSetBias(float bias){
  des_bias_clicks = CLICKS * bias;      // Express bias in clicks per interval.
}

// Rotate Left/Right a particular number of degrees
void  motorRotate(int dir, int deg){
  curHeading = compass_smplHeading();  // Get current heading from compass
  if(dir == RIGHT){
    desHeading = curHeading + deg;                      // Add turn degrees to current heading
    if (desHeading > 360){
      desHeading = desHeading - 360;                    // Adjust value if result is greater than 360
    }
    mFunc = dir;
  } else {
    desHeading = curHeading - deg;                      // Subtract turn degrees from current heading
    if (desHeading < 0){
      desHeading = desHeading + 360;                    // Adjust value if result is negative
    }
    mFunc = dir;
  }
}

/* Rotate to a specific compass heading (angle) */
int motorRotateTo(int newHeading){
  int angleDiff;
  desHeading = newHeading;
  
  curHeading = compass_smplHeading();                   // Get current heading from compass
  angleDiff = compass_diff(curHeading, desHeading);     // Diff between current & Desired heading
  if(angleDiff < 0){                                    // Right is the shortest turn direction.
    mFunc = RIGHT;
  } else {                                              // Left is the shortest turn direction
    mFunc = LEFT;
  }
}      

// Return current motor function
int motorGetFunction(void){
  return mFunc;
}

// Force motors to stop right away.
int motorStop(void){
  mFunc = STOP;
}

// Returns true if motors are stopped
int motorHasFinished(void){
  if (mFunc == STOP)
    return(TRUE);
  else
    return(FALSE);
}
