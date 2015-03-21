/*
  Preliminary library code based on David Betz' code from
  compass code from code.google.com/p/propgcc.
*/

#include "compass3d.h"
#include "mx2125.h"
#include "simpletools.h"
#include "simplei2c.h"

int gCal_x = 0;                                 // Compass calibration X value.
int gCal_y = 0;                                 // Compass calibration y value.
i2c *mybus;


void compass_init(i2c *bus)
{

  mybus = bus;
  /* set to continuous mode */
  unsigned char modeReg = 0x02;
  unsigned char contMode = 0x00;
  unsigned char snglMode = 0x00;
  int n = i2c_out(bus, 0x3C, &modeReg, 1, &contMode, 1);
}

void compass_read(i2c *bus, int *px, int *py, int *pz)
{
  int16_t x16, y16, z16;
  uint8_t data[6];
  char datRegTo3 = 0x03;
  i2c_in(bus, 0x3D, &datRegTo3, 1, data, 6);

  x16 = (data[0] << 8) | data[1];
  z16 = (data[2] << 8) | data[3];
  y16 = (data[4] << 8) | data[5];

  *px = x16;
  *py = y16;
  *pz = z16;
}

void compass_cal(int calx, int caly)
{
  gCal_x = calx;
  gCal_y = caly;
}



int compass_smplHeading()
{
  int x, y, z;                                // Raw Compass reading

  compass_read(mybus, &x, &y, &z);            // Compass vals -> variables

  float fy = (float) y + gCal_y;              // Ints to floats and add in calibration value
  float fx = (float) x + gCal_x;

  float heading = atan2(fy, fx) * 180.0/PI;    // Calculate heading with floats
     
  heading = heading - 90;                       // Correct for sensor mounting orientation
  if (heading < 0)
    heading = heading + 360;
  
  return heading;
}

int compass_diff(int curHead, int newHead)
{

  if ((curHead > newHead) && ((curHead - newHead) < 180))
  {
    return curHead - newHead;
  }
  if ((curHead > newHead) && ((curHead - newHead) >= 180))
  {
    return ((curHead - newHead)-360);
  }
  if ((curHead < newHead) && ((newHead - curHead) < 180))
  {
    return (curHead - newHead);
  }
  if ((curHead < newHead) && ((newHead - curHead) >= 180))
  {
    return (360 - (newHead-curHead));
  }
  if (curHead == newHead) return 0;
}
