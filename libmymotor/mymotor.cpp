/*
 *  Proportional - Integral - Derivative (PID) motor control with Compass rotate
  
 *   Proportional error is the difference between the desired velocity and the current velocity.
 *   Integral error is the cummulative difference between the Left and Right velocities.
 *   Derivative (slope) difference between the current proportional error and the previous one.
 *   All of these values are multiplied by a constant to adjust their affect on the motor speed.
 *
 *   written by Paul Bammel March 2014      // Initial version modeled from
 *                                          //  "Mobile Robots" by J. Jones & A. Flynn
 *   updated by Paul Bammel August 2014     // Modified to handle rotation using the compass module.
 *   updated by Paul Bammel December 2014   // Code clean-up and optimizations.
 *   updated by Paul Bammel January 2015    // Added Derivative code for smoother motor responses.
 *                                          // Also added Dead Reckoning global position tracking.
 *
 */

#include "simpletools.h"                    // General propeller & C++ functions
#include "servo.h"                          // Control up to 14 servos in another core
#include "mycompass.h"                      // HMC5883L 3-Axis compass module functions
#include "robot_defs.h"                     // General robot definitions and I/O pin assignments
#include "mymotor.h"                        // Header file for this pimotor.cpp file

// Private pimotor function prototypes
void  set_servo(int vel, int motor_index);  // Set the speed of a single servo (0-100%)
void  motorControl(void *par);              // Provide motor funtions, runs in its own cog
float get_velClicks(int motor_index);       // Returns number of encoder "clicks" since last pass.
void  init_encoders(void);                  // Initialize encoders to count velocity in "clicks".


// Stack space for Speed Control cog
unsigned int mymtr_stack[(160 + (50 * 4))];

static volatile int     mFunc             = STOP;     // Servo function to perform
static volatile int     mPower[2]         = {0, 0};   // Servo power for Left/Right servo(0% - 100%)
static volatile int     mSign             = 0;        // Indicate robot direction (1=Forward, -1=Backward)
static volatile unsigned char mMode       = 0x07;     // Motor Control Mode (Default PID)

static volatile int     des_vel_clicks    = 0.0;      // Desired velocity in clicks/interval
static volatile int     des_bias_clicks   = 0.0;      // Desired bias in clicks/interval
static volatile float   desInchDist       = 0.0;      // Desired distance in inches. Zero = ignore.
static volatile float   curInchDist       = 0.0;      // Cumulative distance traveled in inches
                                                      //  used when moving fwd/bkwd a fixed distance.
static volatile float   integral          = 0.0;      // Integral of velocity difference between servos
static volatile int     curHeading        = 0;        // Current compass heading
static volatile int     desHeading        = 0;        // Desired (new) compass heading
static volatile int     orgHeading        = 0;        // Compass heading when origin was set.

struct  pose  gps;                                    // Robot's global position
                                                      //  relative to starting (origin).

/* Start SpeedControl function in separate cog*/
int initMotorControl(void){
  int mymtr_cogID = cogstart(&motorControl, NULL, mymtr_stack, sizeof(mymtr_stack));
  return(mymtr_cogID);
}

/* MotorControl running in independent cog */
void motorControl(void *par){
  float integralError = 0.0;                                      // Integral error between servos
  float leftError = 0.0, rightError = 0.0;                        // Proportional velocity error values
  float deltaDist = 0.0;                                          // Distance traveled since last check.
  float deltaX = 0.0, deltaY = 0.0;                               // Delta X & Y offset from last location
  int   deltaHeading = 0;                                         // Difference in Heading from last reading
  int   left_velClicks = 0, right_velClicks = 0;                  // Current Left & Right velocity (in clicks)
  int   leftDelta = 0.0, rightDelta = 0.0;                        // Delta between desired and measured velocity
  int   leftLast = 0, rightLast = 0;                              // Previous Left & Right velocity (in clicks)
  int   angleDiff = 0;                                            // Diff between current & desired heading
  int   turnSpeed = 0;                                            // Speed robot should be turning at

  init_encoders();                                                // Set up wheel encoders.
  compass_init(MODE_CONT);                                        // Initialize compass.
  serial *term = serial_open(USB_RX_PIN,USB_TX_PIN,0,115200);     // Debug terminal serial port.

  while(1){
    left_velClicks = get_velClicks(0);                            // Obtain left velocity (in clicks)
    right_velClicks = get_velClicks(1);                           // Obtain right velocity (in clicks)
    curHeading = compass_smplHeading();                           // Obtain current global heading

    deltaHeading = compass_diff(curHeading, gps.gHeading);        // Diff between new & prev heading
    deltaDist = 0.5 * (float) (left_velClicks + right_velClicks)  // Avg click distance of both wheels
                * DIST_PER_CLICK;                                 // times distance per click
    deltaX = deltaDist * cos(deltaHeading * PI/180);              // Calculate Delta in X position
    deltaY = deltaDist * sin(deltaHeading * PI/180);              // Calculate Delta in Y position
    
    gps.validPos = 0;                                             // Indicate we are updating gps data
    gps.xPos += deltaX;                                           // Update global x-y position with
    gps.yPos += deltaY;                                           //  latest incremenetal changes.
    gps.gHeading = curHeading;                                    // Record current global heading
    gps.rHeading += (float)(right_velClicks - left_velClicks)     // Update relative heading with
                    * DEG_PER_CLICK;                              //  new delta based on clicks.
    gps.validPos = 1;                                             // Indicate gps update is complete.
    
    if(desInchDist > 0){                                          // If traversing a desired distance
      curInchDist += deltaDist;                                   // Accumulate the overall dist traveled.
      if(curInchDist >= desInchDist){                             // If we've reached our desired dist.
        if(mFunc == FORWARD || mFunc == BACKWARD){                //  and we were going fwd/bkwd
          mFunc = STOP;                                           //  then stop.
        } else{                                                   // Otherwise...
          curInchDist = desInchDist = 0.0;                        //  clear variables and perform new func.
        }
      }
    }                                  
    
    switch (mFunc){                                               // Based on desired motor function
      case FORWARD:                                               // Move robot Forward
      case BACKWARD:                                              //  or Backward.
        
        leftDelta = des_vel_clicks - left_velClicks;              // Left Delta between Actual vs Desired speed
        rightDelta = des_vel_clicks - right_velClicks;            // Right Delta between Actual vs Desired speed

        integral   += left_velClicks - right_velClicks +          // Integrate Left & Right velocity
                      des_bias_clicks;                            //  with desired bias.
        integralError = K_INT * integral;                         // also introduce possible bias.
        
        leftError = rightError = 0;                               // Start with error values at zero.
        if((mMode & 0x01) == 0x01){
          leftError  = K_PRO * leftDelta;                         // Proportional speed adjustments 
          rightError = K_PRO * rightDelta;                        //  of left & right servos.
        }
        if((mMode & 0x02) == 0x02){
          leftError  += integralError;                            // Plus Integral error
          rightError += integralError;
        }
        if((mMode & 0x04) == 0x04){
          leftError  += K_DRV * (left_velClicks - leftLast);      // Add in Derivative error
          rightError += K_DRV * (right_velClicks - rightLast);
        }          
        if(mMode == 0x00){                                        // Straight motor mode,
          set_servo((des_vel_clicks / CLICKS), 0);                //  so just set servos to
          set_servo((des_vel_clicks / CLICKS), 1);                //  desired velocity.
          break;
        }
                  
        leftLast = left_velClicks;                                // Remember current left velocity
        rightLast = right_velClicks;                              // Remember current right velocity
        
        mPower[0] += leftError / CLICKS;                          // Adjust left servo %vel if necessary
        mPower[1] += rightError / CLICKS;                         // Adjust right servo %vel if necessary

        if (mPower[0] > V_MAX) mPower[0] = V_MAX;                 // Limit max left servo velocity
          else if (mPower[0] < -V_MAX) mPower[0] = -V_MAX;
        if (mPower[1] > V_MAX) mPower[1] = V_MAX;                 // Limit max right servo velocity
          else if (mPower[1] < -V_MAX) mPower[1] = -V_MAX;

        set_servo(mPower[0], 0);                                  // Alter left servo speed
        set_servo(mPower[1], 1);                                  // Alter left servo speed

        break;
        
      case LEFT:                                                  // Rotate Left or
      case RIGHT:                                                 //  Right desired number of degrees.
      
        angleDiff = compass_diff(curHeading, desHeading);         // Diff between cur & Desired heading
        if (mFunc == LEFT){                                       // Rotate Left desired number of degrees.
          while(angleDiff > 0){
            turnSpeed = abs(angleDiff)/2;                         // Use half the turn angle as turn speed.
            if(turnSpeed < 10) turnSpeed = 10;                    // Unless the turn angle is less than 10.
            servo_set(WHEEL_L_PIN, 1500-turnSpeed);               // Rotate Left wheel
            servo_set(WHEEL_R_PIN, 1500-turnSpeed);               // Rotate Right wheel
            pause(8);                                             // Wait at least 8ms between readings.
            curHeading = compass_smplHeading();                   // Get current heading from compass
            angleDiff = compass_diff(curHeading, desHeading);     // Diff between Current & Desired heading
          }
        } else {
          if ( mFunc == RIGHT){                                   // Rotate Right desired number of degrees.
            while(angleDiff < 0){
              turnSpeed = abs(angleDiff)/2;                       // Use half the turn angle as turn speed.
              if(turnSpeed < 10) turnSpeed = 10;                  // Unless the turn angle is less than 10.
              servo_set(WHEEL_L_PIN, 1500+turnSpeed);             // Rotate Left wheel
              servo_set(WHEEL_R_PIN, 1500+turnSpeed);             // Rotate Right wheel
              pause(8);                                           // Wait at least 8ms between compass readings.
              curHeading = compass_smplHeading();                 // Get current heading from compass
              angleDiff = compass_diff(curHeading, desHeading);   // Diff between Current & Desired heading
            }
          } else {
            mFunc = STOP;                                         // Rotation complete or invalid parameters.
            servo_set(WHEEL_L_PIN, 1500);                         // Force Left servo to stop
            servo_set(WHEEL_R_PIN, 1500);                         // Force Right servo to stop
            integral = 0.0;                                       // Reset Integral to zero
            mPower[0] = mPower[1] = 0;                            // Reset servo velocity to zero
            desInchDist = curInchDist = 0.0;                      // Reset when you stop.
          }
        }          
        break;
        
      case STOP:                                                  // Stop servo motion immediately
      default:
        mFunc = STOP;                                             // Set motor function to stop
        servo_set(WHEEL_L_PIN, 1500);                             // Force Left servo to stop
        servo_set(WHEEL_R_PIN, 1500);                             // Force Right servo to stop
        integral = 0.0;                                           // Reset Integral to zero
        mPower[0] = mPower[1] = 0;                                // Reset servo velocity to zero
        desInchDist = curInchDist = 0.0;                          // Reset when you stop.
        break;
    }
    pause(CTRL_INT);
  }
}

void init_encoders(void){
/*
 *  Initialize counters in motor cog to track Left & Right encoders
 *  The rising edge of a pulse indicates a hole in the wheel
 */
  FRQA = 1;                                             // Add 1 to count at every rising edge.
  PHSA = 0;                                             // Start the edge count at zero.
  FRQB = 1;                                             // Add 1 to count at every rising edge.
  PHSB = 0;                                             // Start the edge count at zero.

  CTRA = 0x28000000 + ENC_L_PIN;                        // Left wheel counter set for positive edges
  CTRB = 0x28000000 + ENC_R_PIN;                        // Right wheel counter set for positive edges
}

// Return current velocity of a particular motor in clicks/interval
float get_velClicks(int motor_index){
  int count = 0;

  if (motor_index == 0){
    count = PHSA;                                       // Retrieve count for left encoder
    PHSA = 0;                                           // Clear left encoder
    return count;                                       // Return count for left servo
  } else {
    count = PHSB;                                       // Retrieve count for right encoder
    PHSB = 0;                                           // Clear right encoder
    return count;                                       // Return count for right servo
  }
}

/* Set the speed of a single servo (0-100%) based on direction and velocity provided */
void set_servo(int vel, int motor_index){
  if (motor_index == 0){                    // Motor index = 0 (Left servo)
    if (mFunc == FORWARD){
      servo_speed(WHEEL_L_PIN, vel);        // Set left servo speed forward
    } else {
      servo_speed(WHEEL_L_PIN, -vel);       // Set left servo speed reverse
    }
  } else {                                  // Motor index = 1 (Right servo)
    if (mFunc == FORWARD){
      servo_speed(WHEEL_R_PIN, -vel);       // Set right servo speed forward
    } else {
      servo_speed(WHEEL_R_PIN, vel);        // Set right servo speed reverse
    }
  }
}

/* Generic robot command structure interface */
struct cmd_struct motorCommand(struct cmd_struct cmdRequest){
  
  struct cmd_struct cmdResult;
  
  switch(cmdRequest.action){
    case  MOVE:
      if(cmdRequest.direction == FORWARD)
        mSign = 1; else mSign = -1;
      des_vel_clicks = CLICKS * abs(cmdRequest.value1); // Convert vel to # of Clicks equivalent.
      if (cmdRequest.value1 > V_MAX){
        des_vel_clicks = CLICKS * V_MAX;                // Limit max servo velocity
      } else if (cmdRequest.value1 < -V_MAX){
        des_vel_clicks = CLICKS * V_MAX;                // and convert to # of Clicks equivalent.
      }    
      desInchDist = cmdRequest.value2;                  // Set desired distance if provided.
      curInchDist = 0.0;                                // Reset current distance traveled.
      des_bias_clicks = 0;                              // Start bias at zero for a straight line.
      integral = 0.0;                                   // Reset Integral to zero.
      mFunc = cmdRequest.direction;                     // Motor function equal to direction of travel.
      break;
    case  TURN:
      curHeading = compass_smplHeading();               // Get current heading from compass
    
      switch(cmdRequest.direction){
        case RIGHT:
          desHeading = curHeading + cmdRequest.value1;  // Add turn degrees to current heading
          if (desHeading > 360){
            desHeading = desHeading - 360;              // Adjust value if result is greater than 360
          }
          mFunc = cmdRequest.direction;
        case LEFT:
          desHeading = curHeading - cmdRequest.value1;  // Subtract turn degrees from current heading
          if (desHeading < 0){
            desHeading = desHeading + 360;              // Adjust value if result is negative
          }
          mFunc = cmdRequest.direction;
        case FACE:
          desHeading = cmdRequest.value1;
          int angleDiff = compass_diff(curHeading, desHeading); // Diff between current & Desired heading
          if(angleDiff < 0){                            // Right is the shortest turn direction.
            mFunc = RIGHT;
          } else {                                      // Left is the shortest turn direction
            mFunc = LEFT;
          }
      }
      break;
    case  BIAS:
      des_bias_clicks = CLICKS * cmdRequest.value1;     // Express bias in clicks per interval.
      break;
    case  MODE:
      mMode = cmdRequest.value1;                        // Establish motor control mode.
      break;
    case  SETPOS:
      gps.xPos = cmdRequest.value1;                     // Set/Reset global x position coordinate.
      gps.yPos = cmdRequest.value2;                     // Set/Reset global y position coordinate.
      gps.gHeading = compass_smplHeading();             // Get current global heading from compass.
      if (gps.xPos==0 && gps.yPos==0){                  // If setting the origin location...
        gps.rHeading = 0;                               //  Set relative heading to zero.
        orgHeading = gps.gHeading;                      //  Remember origin compass heading.
      }
      break;
    case  GETPOS:
      while(gps.validPos == 0){                         // If semaphore flag False,
        pause(1);                                       //  wait a moment and try again.
      }
      cmdResult.action = mFunc;
      cmdResult.direction = gps.gHeading;
      cmdResult.value1 = gps.xPos;
      cmdResult.value2 = gps.yPos;
      return(cmdResult);                                // Return pointer to valid gps structure.
      break;
    case  GETFUNC:
      cmdResult.action = mFunc;
      return(cmdResult);                                // Return current motor function
      break;
    case  STOP:
      mFunc = STOP;                                     // Set motor function to Stop
      break;
  }    
}


