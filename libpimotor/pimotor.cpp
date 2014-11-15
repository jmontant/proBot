/*
  Proportional - Integral motor control with Compass rotate

  written by Paul Bammel March 2014
  updated by Paul Bammel August 2014

  modeled from "Mobile Robots" by J. Jones & A. Flynn

*/

#include "simpletools.h"
#include "servo.h"
#include "mycompass.h"
#include "robot_defs.h"
#include "pimotor.h"


// Stack space for Speed Control cog
unsigned int stack[(160 + (50 * 4))];

int   control_interval  = 250;          // Pause between servo loop intervals

int   mFunc             = STOP;         // Servo function to perform
int   mPower[2]         = {0, 0};       // Percentage servo power (speed as a percentage of 100)

float des_vel_clicks    = 0.0;          // Desired speed in clicks/interval
float des_bias_clicks   = 0.0;          // Desired bias in clicks/interval
int   des_dist_clicks   = 0;            // Desired distance in encoder clicks. Zero = ignore.
int   dist_clicks       = 0;            // Cumulative distance traveled in clicks
float integral          = 0.0;          // Integral of velocity difference
int   cur_heading       = 0;            // Current compass heading
int   des_heading       = 0;            // Desired (new) compass heading


/* Limit range of value */
int limit_range(int val, int low, int high)
{
  if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

/* Set the speed of a single servo (0-100%) */
void set_servo(int vel, int motor_index)
{
  if (motor_index == 0) {
    if (mFunc == FORWARD) {
      servo_speed(WHEEL_L_PIN, vel);     // Set left servo speed forward
    } else {
      servo_speed(WHEEL_L_PIN, -vel);    // Set left servo speed reverse
    }
  } else {
    if (mFunc == FORWARD) {
      servo_speed(WHEEL_R_PIN, -vel);    // Set right servo speed forward
    } else {
      servo_speed(WHEEL_R_PIN, vel);     // Set right servo speed reverse
    }
  }
}

/* Set and remember power level */
void alter_power(int error, int motor_index)
{
  int tmp_pwr = 0;                      // Temporary servo power value
  
  tmp_pwr = ((mPower[motor_index] * CLICKS) + error) / CLICKS;
  mPower[motor_index] = limit_range(tmp_pwr, -200, 200);
    set_servo(mPower[motor_index], motor_index);
}

/* Itegrate left and right servo velocity to synchronize servos */
float integrate(float left_vel, float right_vel, float bias)
{
  integral = integral + left_vel + bias - right_vel;
  return integral;
}

/* Start SpeedControl function in separate cog*/
void init_speedControl(void)
{
  int cog = cogstart(&speed_control, NULL, stack, sizeof(stack));
}

/* SpeedControl running in independent cog */
void speed_control(void *par)
{
  float integral_error = 0.0, left_error = 0.0, right_error = 0.0;
  int left_vel = 0, right_vel = 0, angleDiff = 0;

  serial *term = serial_open(SR_TX_PIN, SR_RX_PIN, 0, 115200);

  init_encoders();                                              // Set up wheel encoders.
  compass_init(0);                                              // Initialize compass on bus.

  while(1)
  {
    switch (mFunc)                                              // Based on desired motor function
    {
      case FORWARD:                                             // Move robot Forward
      case BACKWARD:                                            //  or Backward.

        left_vel = get_velClicks(LEFT);                         // Get current left velocity (in clicks)
        right_vel = get_velClicks(RIGHT);                       // Get current right velocity (in clicks)

//        integral_error = INTEGRAL *                             // Integrate Left & Right velocity
//          integrate(left_vel, right_vel, des_bias_clicks);      //  also introduce possible bias.
        left_error = PRO_GAIN *
          (des_vel_clicks - left_vel - integral_error);         // Proportional speed adjustment of left servo
        right_error = PRO_GAIN *
          (des_vel_clicks - right_vel + integral_error);        // Proportional speed adjustment of right servo

        dprint(term, "des_vel = %f, Left vel = %d, error = %f, power = %d \t", des_vel_clicks, left_vel, left_error, mPower[0]);
        dprint(term, "Right vel = %d, error = %f, power = %d\n", right_vel, right_error, mPower[1]);

        alter_power(left_error, 0);                             // Alter servo speeds as necessary
        alter_power(right_error, 1);
        break;
      case LEFT:                                                // Rotate Left or
      case RIGHT:                                               //  Right desired number of degrees.
        cur_heading = compass_smplHeading();                    // Get current heading from compass
        angleDiff = compass_diff(cur_heading, des_heading);     // Diff between cur & Desired heading
        dprint(term, "dir = %d, cur_heading = %d, des_heading = %d\n", mFunc, cur_heading, des_heading);
        if (angleDiff > 0 && mFunc == LEFT)                     // Rotate Left desired number of degrees.
        {
          while (angleDiff > 0)                                 // While current heading is greater than desired.
          {
//            dprint(term, "Left Turn: dir = %d, cur_heading = %d, des_heading = %d\n", mFunc, cur_heading, des_heading);
            servo_set(WHEEL_L_PIN, 1500-10);             // Rotate Left wheel, speed proportional to angle
            servo_set(WHEEL_R_PIN, 1500-10);             // Rotate Right wheel, speed proportional to angle
            pause(10);                                          // Make sure there is at least 10ms between compass readings.
            cur_heading = compass_smplHeading();                    // Get current heading from compass
            angleDiff = compass_diff(cur_heading, des_heading); // Diff between Current & Desired heading
          }
        }
        else if (angleDiff < 0 && mFunc == RIGHT)               // Rotate Right desired number of degrees.
        {
          while (angleDiff < 0)                                 // While current heading is less than desired.
          {
//            dprint(term, "Right: dir = %d, cur_heading = %d, des_heading = %d\n", mFunc, cur_heading, des_heading);
            servo_set(WHEEL_L_PIN, 1500+10);             // Rotate Left wheel, speed proportional to angle
            servo_set(WHEEL_R_PIN, 1500+10);             // Rotate Right wheel, speed proportional to angle
            pause(10);                                          // Make sure there is at least 10ms between compass readings.
            cur_heading = compass_smplHeading();                    // Get current heading from compass
            angleDiff = compass_diff(cur_heading, des_heading); // Diff between Current & Desired heading
          }
        }
        else
        {
          mFunc = STOP;                                         // Rotation complete or invalid parameters.
          servo_set(WHEEL_L_PIN, 1500);                         // Force Left servo to stop
          servo_set(WHEEL_R_PIN, 1500);                         // Force Right servo to stop
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

// Return current velocity of a particular motor in clicks/interval
float get_velClicks(int motor_index)
{
  int count;

  if (motor_index == 0) {
    count = PHSA;                                               // Retrieve count from left encoder
    PHSA = 0;                                                   // Clear left encoder
    return count;                                               // Return count for left servo
  } else {
    count = PHSB;                                               // Retrieve count from right encoder
    PHSB = 0;                                                   // Clear right encoder
    return count;                                               // Return count for right servo
  }
}

// Initialize counters in motor cog to track Left & Right encoders
void init_encoders(void)
{
  FRQA = 1;                             // Add 1 every time we see an edge (hole in left wheel)
  PHSA = 0;                             // Always start the edge count at zero.
  FRQB = 1;                             // Add 1 every time we see an edge (hole in right wheel)
  PHSB = 0;                             // Always start the edge count at zero.

  CTRA = 0x28000000 + ENC_L_PIN;        // Configure Left wheel counter for positive edges
  CTRB = 0x28000000 + ENC_R_PIN;        // Configure Right wheel counter for positive edges

}

// Set a direction and motion velocity
void move(int dir, int vel)
{
  des_vel_clicks = CLICKS * abs(vel);     // Express percentage velocity in clicks per control interval.
  des_bias_clicks = 0;                    // Make sure bias is zero so robot moves in a straight line.
  mFunc = dir;                            // Set motor function to direction robot should travel.
}

// Adjust bias to make robot swerve left or right.
void adj_bias(float bias)
{
  des_bias_clicks = CLICKS * bias;        // Express bias in clicks per interval.
}

// Rotate Left/Right a particular number of degrees
void  rotate(int dir, int deg)
{
  cur_heading = compass_smplHeading();  // Get current heading from compass
  if (dir == RIGHT) {
    des_heading = cur_heading + deg;    // Add turn degrees to current heading
    if (des_heading > 360) {
      des_heading = des_heading - 360;  // Adjust value if result is greater than 360
    }
    des_bias_clicks = 0;
    mFunc = ROTATE;
  } else {
    des_heading = cur_heading - deg;    // Subtract turn degrees from current heading
    if (des_heading < 0) {
      des_heading = des_heading + 360;  // Adjust value if result is less than 0
    }
    des_bias_clicks = 0;
    mFunc = dir;
  }
}

// Return current motor function
int get_mFunc(void)
{
  return mFunc;
}

// Force motors to stop right away.
int stop(void)
{
  mFunc = STOP;
}

// Returns true if motors are stopped
int hasFinished(void)
{
  if (mFunc == STOP)
    return(TRUE);
  else
    return(FALSE);
}
