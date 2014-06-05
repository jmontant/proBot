/*
  Proportional - Integral motor control

  written by Paul Bammel March 2014

  modeled from "Mobile Robots" by J. Jones & A. Flynn

*/

#include "motor.h"
#include "servo.h"
#include "io_pins.h"

// Stack space for Speed Control cog
unsigned int stack[(160 + (50 * 4))];


int   control_interval  = 250;          // Run servo loop this often
int   mFunc             = STOP;         // Which motor function should it perform
int   des_dir           = RIGHT;        // Desired turning direction. Default Right
int   turn_count        = 0;
float des_vel_clicks    = 0.0;          // Desired velocity, clicks/interval
float des_bias_clicks   = 0.0;          // Desired bias, clicks/interval
int   des_dist_clicks   = 0;            // Desired distance in encoder clicks. Zero = ignore.
int   dist_clicks       = 0;            // Cumulative distance traveled in clicks
float mPower[2]         = {0.0, 0.0};   // Power(speed) command to servos
float integral          = 0.0;          // Integral of velocity difference
int   mSign[2]          = {1, 1};       // Sign of rotation of motor
int   mPin[2]           = {LT_WHEEL, RT_WHEEL};   // Motor servo pins Left & Right
int   mEnc[2]           = {LENC_PIN, RENC_PIN};   // Motor encoder pins Left & Right

/* Limit range of value */
float limit_range(float val, float low, float high)
{
  if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

/* Find sign of argument */
int get_sign(float val)
{
  if (val >= 0.0) return 1;
    else return -1;
}

/* Set the speed of a single servo */
void pwm_motor(float vel, int motor_index)
{
  if (motor_index == 0) {
    if (mSign[motor_index] > 0) {
      servo_speed(mPin[motor_index], (int) -vel);     // Set left servo speed forward
    } else {
      servo_speed(mPin[motor_index], (int) vel);      // Set left servo speed reverse
    }
  } else {
    if (mSign[motor_index] > 0) {
      servo_speed(mPin[motor_index], (int) vel);      // Set right servo speed forward
    } else {
      servo_speed(mPin[motor_index], (int) -vel);     // Set right servo speed reverse
    }
  }
}

/* Set and remember power level */
void alter_power(float err, int motor_index)
{
  mPower[motor_index] = limit_range(mPower[motor_index] + err, -150, 150);
  pwm_motor(mPower[motor_index], motor_index);
}

/* Itegrate left and right servo velocity to synchronize servos */
float integrate(float left_vel, float right_vel, float bias)
{
  integral = integral + left_vel + bias - right_vel;
  return integral;
}

/* Start SpeedControl function in seperate cog*/
void init_speedControl(void)
{
  int cog = cogstart(&speed_control, NULL, stack, sizeof(stack));
}

/* SpeedControl running in independent cog */
void speed_control(void *par)
{
  float left_vel, right_vel, integral_error, left_error, right_error;
  int angleDiff, left_count, right_count, cnt;


  init_encoders();                                          // Set up wheel encoders.

  while(1)
  {
    switch (mFunc)                                          // Based on desired motor function
    {
      case MOVE:                                            // Move robot Forward/Backward.
                                                            // Turn if Bias provided
        left_vel = get_velClicks(LEFT);                     // Get current left velocity (in clicks)
        right_vel = get_velClicks(RIGHT);                   // Get current right velocity (in clicks)
        integral_error = INTEGRAL *                       // Integrate Left & Right velocity
          integrate(left_vel, right_vel, des_bias_clicks);  //  also introduce possible bias.
        left_error = PRO_GAIN *
          (des_vel_clicks - left_vel - integral_error);     // Proportional speed adjustment of left servo
        right_error = PRO_GAIN *
          (des_vel_clicks - right_vel + integral_error);    // Proportional speed adjustment of right servo
          alter_power(left_error, 0);                       // Alter servo speeds as necessary
          alter_power(right_error, 1);
        break;
      case ROTATE:                                          // Rotate Left/Right desired degrees
        left_count = PHSA;                                  // Save the left & right wheel counts for distance
        right_count = PHSB;
        PHSA = 0;
        cnt = 0;

        if (des_dir == LEFT)                                // Rotate Left
        {
          servo_speed(mPin[LEFT], -50);
          servo_speed(mPin[RIGHT], -50);
          while (cnt < turn_count)
          {
            cnt = PHSA;
          }
          PHSA = left_count;
          PHSB = right_count;
          mFunc = STOP;
          servo_set(mPin[LEFT], 1500);                        // Force Left servo to stop
          servo_set(mPin[RIGHT], 1500);                       // Force Right servo to stop
        }
        else if (des_dir == RIGHT)                          // Rotate Right
        {
          servo_speed(mPin[LEFT], 50);
          servo_speed(mPin[RIGHT], 50);
          while (cnt < turn_count)
          {
            cnt = PHSA;
          }          
          PHSA = left_count;
          PHSB = right_count;
          mFunc = STOP;
          servo_set(mPin[LEFT], 1500);                        // Force Left servo to stop
          servo_set(mPin[RIGHT], 1500);                       // Force Right servo to stop
        }
        else { mFunc = STOP; }                              // If I don't think I should rotate
                                                            //  left or right then I should stop.
        break;
      case STOP:                                            // Stop servo motion immediately
      default:
        mFunc = STOP;                                       // Set motor function to stop
        servo_set(mPin[LEFT], 1500);                        // Force Left servo to stop
        servo_set(mPin[RIGHT], 1500);                       // Force Right servo to stop
        integral = 0.0;                                     // Reset Integral to zero
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
    count = PHSA;
    PHSA = 0;
    return count;
  } else {
    count = PHSB;
    PHSB = 0;
    return count;
  }
}

// Initialize counters in motor cog to track Left & Right encoders
void init_encoders(void)
{
  FRQA = 1;                             // Add 1 every time we see an edge
  PHSA = 0;                             // Always start the edge count at zero.
  FRQB = 1;                             // Add 1 every time we see an edge
  PHSB = 0;                             // Always start the edge count at zero.

  CTRA = 0x28000000 + mEnc[0];          // Configure Left wheel counter for positive edges
  CTRB = 0x28000000 + mEnc[1];          // Configure Right wheel counter for positive edges

}

// Set a motion velocity and provide a turning bias if desired
void move(float vel, float bias)
{
  des_vel_clicks = INT_CLICKS * abs(vel);     // Percentage velocity expressed in clicks
  des_bias_clicks = INT_CLICKS * bias;
  mSign[LEFT] = get_sign(vel - bias);
  mSign[RIGHT] = get_sign(vel + bias);
  mFunc = MOVE;
}

// Adjust bias based on distance from nearby objects
void adj_bias(int l_dist, int r_dist)
{
  float bias = 0;

  if (l_dist <= 12) bias = (10 - l_dist) * -2.0;
  if (r_dist <= 12) bias = (10 - r_dist) * 2.0;
  des_bias_clicks = INT_CLICKS * bias;
}

// Rotate Left/Right a particular number of degrees
void  rotate(int dir, int deg)
{
  des_dir = dir;
  turn_count = DEG_CLICKS * deg;
  mFunc = ROTATE;
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

