/*
* @file libcompass3d.c
*
* @author Andy Lindsay
*
* @copyright
* Copyright (C) Parallax, Inc. 2013. All Rights MIT Licensed.
*
* @brief Temporary project and test harness for the name library.
*
* SCL connected to P3 and SDA connected to P2. Display measurement results in 
* SimpleIDE Terminal
*/

#include "simpletools.h"                      // Include simpletools header
#include "compass3d.h"                        // Include compass3d header
#include "simplei2c.h"


int main()                                  // Main function
{
  int cur, new, diff;
  
  int x, y, z;                              // Declare x, y, & z axis variables
  pause(1000);
  i2c *bus = i2c_newbus(9, 10, 0);          // New I2C bus SCL=P3, SDA=P2

  compass_init(bus);                        // Initialize compass on bus.

  compass_cal(-61, 142);                    // Compass Calibration values for x & y

  
  while(1)                                  // Repeat indefinitely
  {
    print("%c", HOME);
    compass_read(bus, &x, &y, &z);          // Compass vals -> variables
  
    print("x=%d, y=%d, z=%d\n", x, y, z);   // Display raw compass values
    print("  Smpl heading = %d \n", compass_smplHeading());       // Display adjusted heading    
//    print("  True heading = %d \n", compass_heading());       // Display adjusted heading  
    pause(200);  
  }
/*
  cur = 186;
  new = 276;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 350;
  new = 20;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 20;
  new = 320;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 30;
  new = 0;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 20;
  new = 185;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 270;
  new = 190;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 10;
  new = 190;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 190;
  new = 190;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 270;
  new = 180;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 0;
  new = 180;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 45;
  new = 150;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 270;
  new = 90;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
  cur = 180;
  new = 0;
  diff = compass_diff(cur, new);
  print("Cur = %d, New = %d, Diff = %d\n", cur, new, diff);
*/
}


