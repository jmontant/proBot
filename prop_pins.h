/**
* @file prop_pins.h
*
* @author Paul Bammel
*
* @version 2.00
*
* @brief Global propeller pin definitions used in all robot applications.
*
*/

#ifndef PROP_PINS_H
#define PROP_PINS_H

#if defined(__cplusplus)
extern "C" {
#endif

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



#if defined(__cplusplus)
}
#endif
/* __cplusplus */ 
#endif
/* PROP_PINS_H */ 
