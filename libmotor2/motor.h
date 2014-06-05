/*
  motor.h
  Proportional - Integral servo control
*/

/* Turning directions */
#define LEFT      0
#define RIGHT     1
#define FORWARD   4
#define BACKWARD  5

/* Motor function commands */
#define STOP    0
#define MOVE    1
#define ROTATE  2

/* Motor Function Constants */
#define INTEGRAL    0.6                   // Integral error gain
#define PRO_GAIN    1.0                   // Proportional gain
#define INT_CLICKS  0.06                  // Clicks per interval based on 100% duty factor
#define DEG_CLICKS  0.18                  // Number of encoder clicks per degree of rotation


float limit_range(float val, float low, float high);
void  pwm_motor(float vel, int motor_index);
void  alter_power(float error, int motor_index);
float integrate(float left_vel, float right_vel, float bias);
void  init_speedControl(void);                  // Fire up Speed Control in a new cog.
void  speed_control(void *par);       
void  move(float vel, float bias);              // Motion velocity + turning bias
void  adj_bias(int l_dist, int r_dist);
float get_velClicks(int motor_index);
void  init_encoders(void);
void  set_dist(int dist);
void  rotate(int dir, int deg);
int   get_mFunc(void);
int   stop(void);
