/*
 *  MySonar - Ultrasonic Ping))) sensor handler
 *
 *  written by Paul Bammel April 2014
 *
 */

#ifndef MYSONR_H
#define MYSONAR_H

#if defined(__cplusplus)
extern "C" {
#endif

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

int   initSonarControl(void);              // Start sonar_control in separate cog.
void  sonarPointAt(int angle);              // Turn ping sensor to face a given direction.
void  sonarScan(int range=90);              // Scan a particular range either side of straight ahead.
void  sonarPing(void);                      // Repeatedly ping whatever direction head is facing.
int   sonarPingNow(void);                   // Perform a single ping and update pingDist variable.
void  sonarSetInc(int inc);                 // Set sweep/scan increment value.
target  sonarGetTarget(int type);           // Return Selected Dist & Dir values.
void  sonarStop(void);                      // Stop any sonar activity at next interval.
int   sonarGetFunction(void);               // Return current sonar function being performed.
int   sonarFindTarget(int type);            // Return angle to target center (pos=right, neg=left)

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
