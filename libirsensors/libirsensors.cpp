/**
 * This is the main libirsensors program file.
 */
#include "simpletools.h"
#include "irsensors.h"

int main(void)
{
  InitIRSensors();
  while(1){
    low(26); 
    low(27);
  if (GetLeftSensorVal()==1)
  {
    high(27);
  } 
 
  if (GetRightSensorVal()==1)
  {
    high(26);
  }

  low(26);
  low(27);

  if (GetFrontSensorVal()>2000)
  {
    high(26); 
    high(27);
  }
}
  return 0;
}

