/*
 * pimotor.h
 * Proportional - Integral - Derivative (PID) motor control with Compass rotate
 */


/* Required robot_deff.h pin definitions
 * ENC_L_PIN                                      // Left wheel encoder
 * ENC_R_PIN                                      // Right wheel encoder
 * WHEEL_L_PIN                                    // Left wheel servo pin
 * WHEEL_R_PIN                                    // Right wheel servo pin
 */

/* Motor function constants */
#define MOTOR_PRESENT                             // Motor control exists
#define K_INT       0.8                           // Integral error gain constant
#define K_PRO       0.8                           // Proportional gain constant
#define K_DRV       0.6                           // Derivative (slope) constant
#define CLICKS      0.08                          // Clicks per interval based on 100% duty factor
#define V_MAX       100                           // Maximum velocity percentage

/* Global motor function prototypes */
void  initMotorControl(void);                     // Start motor Control in a new cog.
void  motorSetBias(int bias);                     // Set the L/R bias value to make robot swerve.
void  motorSetDist(int dist);                     // 
void  motorMove(int dir, int vel);                // Motion Direction & Percentage velocity.
void  motorRotate(int dir, int deg);              // Tell robot to rotate a fixed number of degrees in a particular direction
int   motorRotateTo(int desHeading);              // Rotate to a specific compass direction.
int   motorGetFunction(void);                     // Return currently executing motor function or "stop" if idle.
int   motorStop(void);                            // Stop any running motor functions at the start of next control loop.
int   motorHasFinished(void);                     // Returns true if motors are stopped
