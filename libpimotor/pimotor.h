/*
  motor.h
  Proportional - Integral servo control
*/


/* Motor function commands */
#define ROTATE  2

/* Motor function constants */
#define INTEGRAL    0.8                           // Integral error gain
#define PRO_GAIN    0.8                           // Proportional gain
#define CLICKS      0.08                          // Clicks per interval based on 100% duty factor

int   limit_range(int val, int low, int high);    // Limit range of value
void  set_servo(int vel, int motor_index);        // Set the speed of a single servo (0-100%)
void  alter_power(int error, int motor_index);    // Set and remember power level
float integrate(float left_vel, float right_vel, float bias); // Itegrate left and right servo velocity to synchronize servos
void  init_speedControl(void);                    // Fire up Speed Control in a new cog.
void  speed_control(void *par);       
void  move(int dir, int vel);                     // Motion Direction & Percentage velocity.
void  adj_bias(int bias);
float get_velClicks(int motor_index);
void  init_encoders(void);
void  set_dist(int dist);
void  rotate(int dir, int deg);
int   get_mFunc(void);
int   stop(void);

