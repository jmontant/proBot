/*
 * mymotor.h
 * Proportional - Integral - Derivative (PID) motor control with Compass rotate
 */

#ifndef MYMOTOR_H
#define MYMOTOR_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "robot_defs.h"                     // General robot definitions and I/O pin assignments

/* Required robot_deff.h pin definitions
 * ENC_L_PIN                                      // Left wheel encoder
 * ENC_R_PIN                                      // Right wheel encoder
 * WHEEL_L_PIN                                    // Left wheel servo pin
 * WHEEL_R_PIN                                    // Right wheel servo pin
 */

/* Motor function constants */
#define MOTOR_PRESENT                             // Motor control exists
#define CTRL_INT    250                           // Control Interval pause in milliseconds
#define K_INT       0.3                           // Integral error gain constant
#define K_PRO       0.6                           // Proportional gain constant
#define K_DRV       0.1                           // Derivative (slope) constant
#define CLICKS      0.09                          // Clicks per interval based on 100% duty factor
#define V_MAX       100                           // Maximum velocity percentage
#define DIST_PER_CLICK  0.26                      // Inches traveled per encoder "click"
#define DEG_PER_CLICK   3.438                     // Degrees turned per "click"

// Motor Mode constants
#define STR_MOTOR     0x00                        // Straight Motor control
#define P_MOTOR       0x01                        // Proportional motor control
#define I_MOTOR       0x02                        // Intrgral motor control (used only for debug)
#define PI_MOTOR      0x03                        // Proportional/Integral motor control
#define D_MOTOR       0x04                        // Derivative motor control (used only for debug)
#define PD_MOTOR      0x05                        // Proportional/Derivative motor control
#define ID_MOTOR      0x06                        // Integral & Derivative motor CTRL (used in debug)
#define PID_MOTOR     0x07                        // Proportional/Integral/Derivative CTRL (Default)

// Global Positioning (dead reckoning) data structure
struct pose {
  int   validPos;                                 // Valid Position data if 1 (semaphore flag).
  float xPos;                                     // Robots current x coordinate from origin.
  float yPos;                                     // Robots current y coordinate from origin.
  int   rHeading;                                 // Robots current heading "relative" to origin.
  int   gHeading;                                 // Robots current "global" compass heading.
};  

/* Global motor function prototypes */
int  initMotorControl(void);                      // Start motor Control in a new cog.
void  motorSetMode(unsigned char mode);           // Set the motor control mode (See motor mode constants).
void  motorSetBias(int bias);                     // Set the L/R bias value to make robot swerve.
int   motorGetFunction(void);                     // Return current motor function ("stop" if idle).

void  motorMove(int dir, int vel, int dist=0);    // Motion Direction, Percentage velocity, & Distance.
void  motorRotate(int dir, int deg);              // Rotate Left/Right specified number of degrees
                                                  //  or Face a specified compass direction
int   motorStop(void);                            // Stop current motor function at start of next control loop.

int   motorSetPosition(float x=0.0, float y=0.0); // Set robot's current position to specified values
int   motorSetHeading(void);                      // Update heading with current value from compass module
pose  motorGetPose(void);                         // Return current Pose structure values
struct cmd_struct motorCommand(struct cmd_struct cmdRequest);


#if defined(__cplusplus)
}
#endif
/* __cplusplus */  
#endif
/* MYMOTOR_H */  

/**
 * TERMS OF USE: MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
