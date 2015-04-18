/**
* @file robot_defs.h
*
* @author Paul Bammel
*
* @version 2.00
*
* @brief Global definitions used in all robot applications.
*
*/

#ifndef ROBOT_DEFS_H
#define ROBOT_DEFS_H

#if defined(__cplusplus)
extern "C" {
#endif


/**
 * @brief Generally accepted absolute values used throughout the applications
 * but this way they are defined only once.
 */
#ifndef	TRUEON
#define TRUE	1
#define	ON		1
#endif

#ifndef	FALSEOFF
#define FALSE	0
#define	OFF		0
#endif

#ifndef PI 
#define PI		3.141592653589793
#endif


/**
 * @brief Global definitions for standard values that need to be past between functions
 * (ie. What Action to take, What direction to go, etc.).
 */

// General Handler Action Words
#define	STOP	10							// Stop any running handler function.
#define	GETFUNC 11                          // Return current handler function.

// Motor Handler Action Words
#define MOVE	20							// Move Forward/Backward
#define TURN	21							// Turn Left/Right or face a particular direction
#define BIAS	22							// Set motor bias
#define MODE	23							// Set motor mode (PI, PD, PID, etc)
#define SETPOS	24							// Set GPS location to specific values
#define GETPOS	25							// Get current GPS location

// Sonar Handler Action words
#define	SWEEP	30							// Continuous pass over defined area for objects
#define SCAN  	31							// A single pass over defined area for objects
#define PING  	32							// Continuous ping with head in current position
#define CHECK 	33                          // Check Left, Right, & Center for clearance
#define AVOID 	34                          // Determine action required to avoid obstacles
#define POINT 	35                          // Point sonar in a particular direction
#define TARGET  36                          // Return Direction and distance to target
#define SETINC  37                          // Set Sweep/Scan turn increment

// Robot Direction words
#define   LEFT      40
#define   RIGHT     41
#define   UP        42
#define   DOWN      43
#define   FORWARD   44
#define   BACKWARD  45
#define	  FACE      46
#define	  LOOK      47


/**
 * @brief Tri-color LED controls
 * Red = Fail/error, Yellow(Red & Green) = Caution/Trigger, Green = Pass/VR word.
 */
#define	 PASS	1
#define	 VR		1
#define	 FAIL	2
#define	 ERR	2
#define	 CAUTION 3
#define	 TRIG	3


/**
 * @brief Propeller EEPROM data storage definitions.
 * EEPROM memory area to store long term data values.
 */
#define _EE_ADDR_START	63400			// Starting address of EEPROM data storage area.
#define	_EE_CMPS_ID		0				// Address offset to Compass Calibration ID string.
#define _EE_CMPS_XCAL	10				// Address offset to Compass X-axis calibration value.
#define	_EE_CMPS_YCAL	15				// Address offset to Compass Y-axis calibration value.

/*
 * @brief Common command structure used for sub-system communication
 */
struct  cmd_struct{
  int action;                                       // Action verb - Move, Turn, Look, etc.
  int direction;                                    // Direction - Left, Right, Backward, etc.
  int value1;                                       // Number of inches/degrees, Speed, Angle, etc.
  int value2;										// Additional parameter, where needed
};


#if defined(__cplusplus)
}
#endif
/* __cplusplus */ 
#endif
/* ROBOT_DEFS_H */ 
