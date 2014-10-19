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
 * but this waythey are defined only once.
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
 * @brief Global definitions for standard values that need ot be past between functions
 * (ie. What Action to take, What direction to go, etc.).
 */

// Robot Action Words
#define   ACTION    0
#define   MOVE      1
#define   TURN      2
#define   RUN       3
#define   LOOK      4
#define   ATTACK    5
#define   STOP      6
#define   HELLO     7
  // Robot Direction words
#define   LEFT      0
#define   RIGHT     1
#define   UP        2
#define   DOWN      3
#define   FORWARD   4
#define   BACKWARD  5


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
#define VR_RX_PIN     7                 // EasyVR Recv pin
#define VR_TX_PIN     8                 // EasyVR Xmit pin
#define CMPS_SCL_PIN  9                 // Compass SCL pin
#define CMPS_SDA_PIN  10                // Compass SDA pin
#define PING_PIN      11                // Ultrasonic Ping))) sensor
//#define             12
#define ACCL_Y_PIN    13                // Memsic 2125 Dual-axis Accelerometer Y-Axis
#define ACCL_X_PIN    14                // Memsic 2125 Dual-axis Accelerometer X-Axis
#define HEAD_PIN      15                // Head servo pin
#define WHEEL_L_PIN   16                // Left wheel continuos rotation servo
#define WHEEL_R_PIN   17                // Right wheel continuos rotation servo
#define	AD_DIN_PIN	  18				// Analog - Digital converter Data In
#define	AD_DOUT_PIN	  19				// Analog - Digital converter Data Out
#define AD_SCL_PIN	  20				// Analog - Digital converter Serial Clock
#define	AD_CS_PIN	  21				// Analog - Digital converter Chip Select
#define	SD_DO_PIN	  22				// MicroSD card Data Out
#define	SD_CLK_PIN	  23				// MicroSD card Clock
#define SD_DI_PIN	  24				// MicroSD card Data In
#define SD_CS_PIN	  25				// MicroSD card Chip Select
#define DA_LEFT_PIN	  26				// DAC 0 (Left Stereo) & LED
#define DA_RIGHT_PIN  27				// DAC 1 (Right Stereo) & LED
#define EE_SCL_PIN	  28				// EEPROM Serial Clock
#define EE_SDA_PIN	  29				// EEPROM Serial Data
#define	SR_RX_PIN	  30				// Serial Receive data
#define SR_TX_PIN	  31				// Serial Transmit data

//#define PIR_PIN                       // Passive IR (PIR) sensor
//#define LCD_PIN                       // LCD display data pin


/**
 * @brief Propeller EEPROM data storage definitions.
 * EEPROM memory area to store long term data values.
 */
#define _EE_ADDR_START	63400			// Starting address of EEPROM data storage area.
#define	_EE_CMPS_ID		0				// Address offset to Compass Calibration ID string.
#define _EE_CMPS_XCAL	10				// Address offset to Compass X-axis calibration value.
#define	_EE_CMPS_YCAL	15				// Address offset to Compass Y-axis calibration value.


#if defined(__cplusplus)
}
#endif
/* __cplusplus */ 
#endif
/* ROBOT_DEFS_H */ 
