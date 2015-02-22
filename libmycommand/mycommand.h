/*
 *  Voice Command recognition using EasyVR
 *  
 *  Written by Paul & Amy Bammel
 *  Version: 0.01 Dec 2014 Initial release
 *  
 */

#ifndef MYCOMMAND_H
#define MYCOMMAND_H

#if defined(__cplusplus)
extern "C" {
#endif

#define CMD_TIME        2             // How many seconds to listen for each command word
#define CMD_SI_KNOB     TYPICAL       // Sets the confidence threshold of built-in words
#define CMD_SD_LEVEL    NORMAL        // Sets the confidence threshold of custom words

#define WG_COMMAND      1             // Intial command word (Verb)
#define WG_DETERMINER   2             // Unit of measure
#define WG_DEGREES      3             // Various angles in degres
#define WG_NOUNS        4             // Nouns - subject of select commands

// Command "action" words
#define WG1_MOVE        0             // Move Forward or Backward
#define WG1_TURN        1             // Turn Left or Right
#define WG1_LOOK        2             // Turn "head" Left or Right
#define WG1_FACE        3             // Rotate to a particular direction or object
#define WG1_AGAIN       4             // Repeat last successful command
#define WG1_REPORT      5             // Provide report on requested information
#define WG1_SCAN        6             // Perform a 180 degree sweep of area

// Determiners - Units of measure
#define WG2_STEPS       0             // 6 times units = number of inches
#define WG2_INCHES      1             // Use units directly as number of inches
#define WG2_FEET        2             // 12 times units = number of inches
#define WG2_DEGREES     3             // Return proper number of degrees

// Degrees - Used for Turning and Facing
#define WG3_TWENTY      0             // Twenty degrees
#define WG3_THIRTY      1             // Thirty degrees
#define WG3_FORTYFIVE   2             // Forty five dgrees
#define WG3_SIXTY       3             // Sixty degrees
#define WG3_NINETY      4             // Ninety degrees
#define WG3_ONE_EIGHTY  5             // One hundred and eighty degrees

// Nouns used as subjects in spoken commands
#define WG4_NORTH       0             // Zero degrees compass heading
#define WG4_SOUTH       1             // 180 degrees compass heading
#define WG4_EAST        2             // 90 degrees compass heading
#define WG4_WEST        3             // 270 degrees compass heading
#define WG4_OBJECT      4             // Closest object recorded in last scan
#define WG4_DISTANCE    5             // Distance in cm to object in front of ping sensor
#define WG4_TEMPERATURE 6             // Return current temperature in degrees fahrenheit
#define WG4_DIRECTION   7             // Current compass direction
#define WG4_MAZE        9             // Solve maze
#define WG4_BATTERY     10            // Return current battery voltage
#define WG4_STATUS      11            // Return general status report

// Synthetic token definitions
#define ANY_DEG         901           // Returnd Match true if any valid degree was spoken
#define ANY_NUM         902           // Returnd Match true if any valid number was spoken
#define BAD_CMD         903           // Invalid command - Does Not Compute
#define EXECUTE         904           // Done parsing, execute command as given

int init_myCommand(void);
int getSentence(void);

#if defined(__cplusplus)
}
#endif
/* __cplusplus */  
#endif
/* MYCOMMAND_H */  

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
