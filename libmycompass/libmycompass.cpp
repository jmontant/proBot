/*
* @file libmycompass.c
*
* @author Paul Bammel
*
* @copyright
* Copyright (C) Parallax, Inc. 2013. All Rights MIT Licensed.
*
* @brief HMC5883L Compass Library and test harness.
*   Modified by Paul Bammel Jun 2014
*     - so calling programs don't need to open an I2C bus.
*   Modified by Paul Bammel Oct 2014
*     - to allow robot to calibarate compass and store values in EEPROM.
*
*/

#include  "mycompass.h"                         // Include the myCompass header file.
#include  "simpletools.h"                       // Needed for debug print and EEprom management.
#include  "robot_defs.h"                        // This will provide global robot definitions.
#include  "servo.h"                             // Need the ability to work the servos.

int main()                                      // Main function
{
  int x, y, z;                                  // Declare x, y, & z axis variables
  compass_init(MODE_CONT);                      // Initialize compass to continuos read mode.
  
  /* Calibrate the compass module */
  servo_set(WHEEL_L_PIN, 1490);                 // Rotate Left wheel slowly.
  servo_set(WHEEL_R_PIN, 1490);                 // Rotate Right wheel slowly in the opposite direction.
  
  compass_calibrate();                          // Call compass calibration routine.

  servo_set(WHEEL_L_PIN, 1500);                 // Stop the Left wheel
  servo_set(WHEEL_R_PIN, 1500);                 // Stop the Right wheel
  
  while(1)                                      // Repeat indefinitely
  {
    print("%c", HOME);
    print("smplHeading = %d\n", compass_smplHeading());

    compass_read(&x, &y, &z);                   // Compass vals -> variables

    float heading = atan2(x, y);
    
    if(heading < 0)
    {
      heading += 2.0 * PI;
    }
     
    float headingDegrees = heading * 180/PI; 

    print("\nx=%d, y=%d, z=%d\n", x, y, z);     // Display raw compass values
    print("heading = %f, \n", headingDegrees);  // Display compass heading
          
    pause(500);                                 // Wait 1/2 second
  }
}

