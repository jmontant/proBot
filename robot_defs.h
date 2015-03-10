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
#define   LEFT      0
#define   RIGHT     1
#define   UP        2
#define   DOWN      3
#define   FORWARD   4
#define   BACKWARD  5
#define	  FACE      6


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
 * @brief Propeller I/O pin definitions, Set based on a particular machine.
 * (This avoids hard coding I/O pins within individual programs).
 */

//#define             0
#define LED_VR_PIN    1                 // Green EasyVR Sampling LED (Blink with ERR_LED for Trigger)
#define LED_ERR_PIN   2                 // Red EasyVR Error LED (Blink with VR_LED for Trigger)
#define	EMIC_SOUT_PIN 3					// EMIC Serial Out to proppeller pin
#define ENC_L_PIN     4                 // Left wheel encoder data pin
#define ENC_R_PIN     5                 // Right wheel encoder data pin
#define	EMIC_SIN_PIN  6					// EMIC Serial In to EMIC board pin
#define VR_RX_PIN     7                 // EasyVR Recv pin (recv from EasyVR)
#define VR_TX_PIN     8                 // EasyVR Xmit pin (xmit to EasyVR)
#define CMPS_SCL_PIN  9                 // Compass SCL pin
#define CMPS_SDA_PIN  10                // Compass SDA pin
#define PING_PIN      11                // Ultrasonic Ping))) sensor
//#define             12
#define ACCL_Y_PIN    13                // Memsic 2125 Dual-axis Accelerometer Y-Axis
#define ACCL_X_PIN    14                // Memsic 2125 Dual-axis Accelerometer X-Axis
#define HEAD_PIN      15                // Head servo pin
#define WHEEL_L_PIN   16                // Left wheel continuos rotation servo
#define WHEEL_R_PIN   17                // Right wheel continuos rotation servo
#define	ADC_DI_PIN	  18				// Analog - Digital converter Data In
#define	ADC_DO_PIN	  19				// Analog - Digital converter Data Out
#define ADC_SCL_PIN	  20				// Analog - Digital converter Serial Clock
#define	ADC_CS_PIN	  21				// Analog - Digital converter Chip Select
#define	SD_DO_PIN	  22				// MicroSD card Data Out
#define	SD_CLK_PIN	  23				// MicroSD card Clock
#define SD_DI_PIN	  24				// MicroSD card Data In
#define SD_CS_PIN	  25				// MicroSD card Chip Select
#define DAC_L_PIN     26				// DAC 0 (Left Stereo) & LED
#define DAC_R_PIN     27				// DAC 1 (Right Stereo) & LED
#define EE_SCL_PIN	  28				// EEPROM Serial Clock
#define EE_SDA_PIN	  29				// EEPROM Serial Data
#define	USB_TX_PIN	  30				// Serial Transmit data
#define USB_RX_PIN	  31				// Serial Receive data

//#define PIR_PIN                       // Passive IR (PIR) sensor
//#define LCD_PIN                       // LCD display data pin
//#define XBEE_DI_PIN					// Xbee Data Input pin
//#define XBEE_DO_PIN					// Xbee Data Output pin

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
