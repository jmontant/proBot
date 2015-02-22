/*
 *  HMC5883L compass code based on David Betz' code from
 *  code.google.com/p/propgcc.
 */

#include  "mycompass.h"                         // Include the myCompass header file.
#include  "simplei2c.h"                         // Need the I2C library to talk to the compass module.
#include  "robot_defs.h"                        // This will provide global robot & I/O definitions.
#include  "simpletools.h"                       // Needed for debug print and eeprom management.

int gCal_x = 0;                                 // Compass calibration X value.
int gCal_y = 0;                                 // Compass calibration y value.
i2c *hmcBus;                                    // Compass I2C bus to communicate with module.

int compCalc(int max, int min);                 // Calculate axis calibration value.

/* Initialize Compass */
void compass_init(unsigned char cMode){

  hmcBus = i2c_newbus(CMPS_SCL_PIN, CMPS_SDA_PIN, 0);   // New I2C bus for module communication.

  /* initialize compass module */
  int modeReg = 0x02;
  int n = i2c_out(hmcBus, 0x3C >> 1, modeReg, 1, &cMode, 1);
  
  if(ee_getByte(_EE_ADDR_START + _EE_CMPS_ID) == 'C'){  // If EEProm contains valid values.
    gCal_x = ee_getInt(_EE_ADDR_START + _EE_CMPS_XCAL); // Store integer value of X-axis calibration.
    gCal_y = ee_getInt(_EE_ADDR_START + _EE_CMPS_YCAL); // Store integer value of Y-axis calibration.
  }  
}

/* Return raw compass values */
void compass_read(int *px, int *py, int *pz){

  int16_t x16, y16, z16;
  uint8_t data[6];
  int datRegTo3 = 0x03;
  
  i2c_in(hmcBus, 0x3D >> 1, datRegTo3, 1, data, 6);

  x16 = (data[0] << 8) | data[1];
  z16 = (data[2] << 8) | data[3];
  y16 = (data[4] << 8) | data[5];

  *px = x16;
  *py = y16;
  *pz = z16;
}

/* Provide specific calibration values */
void compass_setCal(int calx, int caly){

  gCal_x = calx;
  gCal_y = caly;
}

/* Return calibration adjusted heading */
int compass_smplHeading(){

  int x, y, z;                                  // Local Compass variables

  compass_read(&x, &y, &z);                     // Compass vals -> variables

  float fy = (float) y + gCal_y;                // Ints to floats and add in calibration value
  float fx = (float) x + gCal_x;

  float heading = atan2(fy, fx) * 180.0/PI;     // Calculate heading with floats
     
  heading = heading - 90;                       // Correct for sensor mounting orientation
  if (heading < 0)
    heading = heading + 360;
  
  return heading;
}

/* Return differencce between Current heading and Desired Heading */
/* Positive values indicates desired heading is Left of Current heading
 * Negative values indicate desired heading is right of Current heading */
int compass_diff(int curHead, int desHead){

  if ((curHead > desHead) && ((curHead - desHead) <= 180)){
    return curHead - desHead;
  }
  if ((curHead > desHead) && ((curHead - desHead) >= 180)){
    return ((curHead - desHead)-360);
  }
  if ((curHead < desHead) && ((desHead - curHead) <= 180)){
    return (curHead - desHead);
  }
  if ((curHead < desHead) && ((desHead - curHead) >= 180)){
    return (360 - (desHead-curHead));
  }
  if (curHead == desHead) return 0;
}

/* Determine proper calibration values based on raw data collection */
/* Robot needs to be rotating at a slow speed during this operation */
int compass_calibrate(void){
  
  int x, y, z;                                // Local Compass values.
  int xMax = 0, xMin = 0;                     // Max and Min 'x' axis values.
  int yMax = 0, yMin = 0;                     // Max and Min 'y' axis values.
  int xCal = 0, yCal = 0;                     // Calibration values for x & y compass axis.

  for(int j = 0; j < 3500; j++){              // Let's spin around for a few seconds
                                              //  and take some raw compass readings.
    compass_read(&x, &y, &z);                 // Raw Compass vals -> variables
    if (y > yMax) yMax = y;                   // Adjust yMax as necessary.
    if (y < yMin) yMin = y;                   // Adjust yMin as necessary.
    if (x > xMax) xMax = x;                   // Adjust xMax as necessary.
    if (x < xMin) xMin = x;                   // Adjust xMin as necessary.
  }    

  xCal = compCalc(xMax, xMin);                // Calculate compass x axis calibration value.
  yCal = compCalc(yMax, yMin);                // Calculate compass x axis calibration value.

  ee_putByte('C', _EE_ADDR_START + _EE_CMPS_ID);    // Store Compass ID to indicate
                                                    //  valid calibration values exist.
  ee_putInt(xCal, _EE_ADDR_START + _EE_CMPS_XCAL);  // Store integer value of X-axis calibration.
  ee_putInt(yCal, _EE_ADDR_START + _EE_CMPS_YCAL);  // Store integer value of Y-axis calibration.
  
  return(TRUE);
}
  
/* Return proper calibration value based on a given max/min range */
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

