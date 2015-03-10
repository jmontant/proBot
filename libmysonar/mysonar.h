/*
 *  @file mysonar.h
 *
 *  @author Paul Bammel April 2014
 *
 *  @brief MySonar - Ultrasonic Ping))) sensor handler
 */

#ifndef MYSONR_H
#define MYSONAR_H

#if defined(__cplusplus)
extern "C" {
#endif

#include  "robot_defs.h"

// General definitions
#define CURRENT     1
#define CLOSEST     2
#define FARTHEST    3
#define SINGLE      4
#define CONTINUOUS  5


struct target {
  int dist;                                 // Distance to selected target.
  int dir;                                  // Direction to selected target.
};  

struct obstacle {                           // Distance to objects (walls) by robot.
  int right;                                // Far right edge (object/wall)
  int left;                                 // Far left edge (object/wall)
  int center;                               // Straight in front of robot
};  

static volatile int sFunc     = STOP;       // Current sonar function being performed
static volatile int pingDist  = 0;          // Current distance returned by Ping sensor.
static volatile int pingDir   = 0;          // Current direction Ping sensor (head) is facing.
static volatile int newDir    = 0;          // New direction Ping sensor (head) should face.
static volatile int minDist   = 0;          // Distance to closest object ping can see
static volatile int minDir    = 0;          // Direction to closest object seen
static volatile int maxDist   = 0;          // Distance to farthest object ping can see
static volatile int maxDir    = 0;          // Direction to farthest object seen
static volatile int sweepInc  = 5;          // Number of degrees to adv when scanning/sweeping
static volatile int panStart  = 0;          // Right limit(edge) of Sweep/Scan operation.
static volatile int panEnd    = 180;        // Left limit(edge) of Sweep/Scan operation.

// Sonar function prototypes
int   initSonarControl(void);               // Start sonar_control in separate cog.
void  sonarPointAt(int angle);              // Turn ping sensor to face a given direction.
struct cmd_struct sonarCommand(struct cmd_struct cmdRequest);
int   sonarFindTarget(int type);            // Return angle to target center (pos=right, neg=left)

/* Private sonar function prototypes */
void  sonar_control(void *par);             // Independent cog sonar control template definition.
int   findLeftEdge(int type);               // Left Edge detection function template definition.
int   findRightEdge(int type);              // Right Edge detection function template definition.

#if defined(__cplusplus)
}
#endif
/* __cplusplus */  
#endif
/* MYSONAR_H */  

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
