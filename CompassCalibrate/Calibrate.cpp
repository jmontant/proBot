/**
* This program will calibrate the HMC5883L 3-axis compass
* for operation in our propeller bots
* 
* written by Paul Bammel 10/19/2014
* version 1.0.0 -- Initial program
* 
*/

#define CMPS_CAL_DEBUG                        // Enable debug print statements if defined.

#include  "mycompass.h"                       // Include the myCompass header file.
#include  "servo.h"                           // Need the ability to work the servos.
#include  "robot_defs.h"                      // This will provide global robot definitions.
#include  "simpletools.h"                     // Needed for debug print and EEprom management.

int compCalc(int max, int min){
  max = abs(max);                             // Absolute value of submitted max.
  min = abs(min);                             // Absolute value of submitted min.
  int truMax = (max + min) / 2;               // Average of raw max and min values submitted,
                                              //  which is the desired max after calibration.
  int calVal = 0;                             // Actual calibration value.
  
  if(max > min){                              // If abs(max) was larger than abs(min).
    calVal = max - truMax;                    // Difference between raw max and true max.
    return -calVal;                           // Return negative calVal to be added to raw readings.
  } else {                                    // Else when abs(min) is larger than abs(max).
    calVal = min - truMax;                    // Difference between raw max and true max.
    return calVal;                            // Return positive calVal to be added to raw readings.
  }        
}  

int main(){
  int x, y, z;                                // Raw Compass values.
  int xMax = 0, xMin = 0;                     // Max and Min 'x' axis values.
  int yMax = 0, yMin = 0;                     // Max and Min 'y' axis values.
  int xCal = 0, yCal = 0;                     // Calibration values for x & y compass axis.

#if defined(CMPS_CAL_DEBUG)                   // If debug mode has been defined
  int chk, xc, yc;
  chk = ee_getByte(_EE_ADDR_START + _EE_CMPS_ID);
  print("EEProm check byte = %c\n", chk);
  if(chk == 'C'){
    xc = ee_getInt(_EE_ADDR_START + _EE_CMPS_XCAL);  // Store integer value of X-axis calibration.
    print("EEProm xCal = %d\n", xc);
    yc = ee_getInt(_EE_ADDR_START + _EE_CMPS_YCAL);  // Store integer value of Y-axis calibration.
    print("EEProm yCal = %d\n", yc);
  }    
#endif
  
  compass_init(MODE_CONT);                    // Initialize compass for continuous reading mode.
  
  servo_set(WHEEL_L_PIN, 1490);               // Rotate Left wheel slowly.
  servo_set(WHEEL_R_PIN, 1490);               // Rotate Right wheel slowly in the opposite direction.

  for(int j = 0; j < 3500; j++){              // Let's spin around for a few seconds
                                              //  and take some raw compass readings.
    compass_read(&x, &y, &z);                 // Raw Compass vals -> variables
    if (y > yMax) yMax = y;                   // Adjust yMax as necessary.
    if (y < yMin) yMin = y;                   // Adjust yMin as necessary.
    if (x > xMax) xMax = x;                   // Adjust xMax as necessary.
    if (x < xMin) xMin = x;                   // Adjust xMin as necessary.
  }    
  
  servo_set(WHEEL_L_PIN, 1500);               // Stop the Left wheel
  servo_set(WHEEL_R_PIN, 1500);               // Stop the Right wheel

#if defined(CMPS_CAL_DEBUG)                   // If debug mode has been defined
  print("xMax = %d\n", xMax);                 // Display xMax value on the terminal.
  print("xMin = %d\n", xMin);                 // Display xMin value on the terminal.
  print("yMax = %d\n", yMax);                 // Display yMax value on the terminal.
  print("yMin = %d\n", yMin);                 // Display yMin value on the terminal.
#endif

  xCal = compCalc(xMax, xMin);                // Calculate compass x axis calibration value.
  yCal = compCalc(yMax, yMin);                // Calculate compass x axis calibration value.

#if defined(CMPS_CAL_DEBUG)                   // If debug mode has been defined
  print("xCal = %d\n", xCal);                 // Display xCal value on the terminal.
  print("yCal = %d\n", yCal);                 // Display yCal value on the terminal.
#endif

  ee_putByte('C', _EE_ADDR_START + _EE_CMPS_ID);    // Store Compass ID to indicate
                                                    //  valid calibration values exist.
  ee_putInt(xCal, _EE_ADDR_START + _EE_CMPS_XCAL);  // Store integer value of X-axis calibration.
  ee_putInt(yCal, _EE_ADDR_START + _EE_CMPS_YCAL);  // Store integer value of Y-axis calibration.

  return 0;
}
