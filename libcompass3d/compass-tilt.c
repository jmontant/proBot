int compass_heading()
{
  int x, y, z;                                // Raw Compass reading
  float xh, yh;                               // X & Y rotated back to horizontal orientation

  compass_read(mybus, &x, &y, &z);            // Compass vals -> variables
  int mx = mx_tilt(14);                       // X-axis tilt angle
  int my = mx_tilt(13);                       // Y-axis tilt angle

  float fy = (float) y + gCal_y;              // Ints to floats and add in calibration value
  float fx = (float) x + gCal_x;

  xh = (x * cos(mx * PI/180)) +
        (y * sin(my * PI/180) * sin(mx * PI/180)) -
        (z * cos(my * PI/180) * sin(mx * PI/180));
  yh = (y * cos(my * PI/180)) +
        (z * sin(my *PI/180));

  float heading = atan2(yh, xh) * 180.0/PI;    // Calculate heading with floats
     
  heading = heading - 90;                       // Correct for sensor mounting orientation
  if (heading < 0)
    heading = heading + 360;
  
  return heading;
}

void compass_calcCal(void)            // Calculate Calibration values
{
  int minx, miny, maxx, maxy;
  int x, y, z;                                // Raw Compass reading
  float calXval, calYval;
  calXval = 0.0;
  calYval = 0.0;

  while(1)
  {
    compass_read(mybus, &x, &y, &z);            // Compass vals -> variables
    if (x < minx) minx = x;
    if (x > maxx) maxx = x;
    if (y < miny) minx = y;
    if (y > maxy) maxx = y;
    calXval = (minx + maxx) / 2;
    calYval = (miny + maxy) / 2;
    print("%c", HOME);
    print("x=%d, y=%d, z=%d\n", x, y, z);   // Display raw compass values
    print("minx - maxx - calx %d  %d  %f\n", minx, maxx, calXval);
    print("miny - maxy - caly %d  %d  %f\n", miny, maxy, calYval);
  }
}
