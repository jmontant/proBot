/*
* @file libcompass3d.c
*
* @author Andy Lindsay
*
* @copyright
* Copyright (C) Parallax, Inc. 2013. All Rights MIT Licensed.
*
* @brief Temporary project and test harness for the compass library.
*   Modified by Paul Bammel Jun 2014 so calling programs don't need an I2C bus.
*
*/

#include "simpletools.h"                      // Include simpletools header
#include "mycompass.h"                        // Include compass3d header

int main()                                    // Main function
{
  int x, y, z;                                // Declare x, y, & z axis variables
  compass_init(0);                            // Initialize compass to continuos read mode.
  compass_cal(-47, 114);                      // Compass calibration values x & y.
  while(1)                                    // Repeat indefinitely
  {
    print("%c", HOME);
    print("smplHeading = %d\n", compass_smplHeading());

    compass_read(&x, &y, &z);            // Compass vals -> variables

    int *px, *py, *pz;

    px = &x;
    py = &y;
    pz = &z;
 
    *px = x;
    *py = y;
    *pz = z;

    float heading = atan2(x, y);
    if(heading < 0)
    {
      heading += 2.0 * 3.14;
    }
     
    float headingDegrees = heading * 180/3.14; 

    print("\nx=%d, y=%d, z=%d\n",              // Display raw compass values
          x, y, z);
    print("heading = %f, \n",                    // Display raw compass values
          headingDegrees);
    pause(500);                                  // Wait 1/2 second
  }
}

