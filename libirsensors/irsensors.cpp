#include "irsensors.h"
#include "adcDCpropab.h"
#include "simpletools.h"
#include "io_pins.h"

struct Sensors{
  int left;
  int right;
  int front;
  int ping;
}sensors;

unsigned int stack[360];

/* Name: InitIRSensors
 * Purpose:
 * Inputs:
 * Outputs:
*/
void InitIRSensors(void)
{
  sensors.left=0;
  sensors.right=0;
  sensors.front=0;
  adc_init(ADC0_CS_PIN, ADC0_SCL_PIN, ADC0_DO_PIN, ADC0_DI_PIN);
  int stacksize = sizeof(_thread_state_t)+sizeof(int)*25;
  int cog = cogstart(MonitorIRSensors,NULL, stack, sizeof(stack));
}

/* Name: CheckSensors
 * Purpose:
 * Inputs:
 * Outputs:
*/
void MonitorIRSensors(void *arg)
{
  sensors.left=0;
  sensors.right=0;
  sensors.front=0;

  while(1)
  {
    freqout(4,1,30000);
    sensors.left = (input(0)==0);
    freqout(5,1,30000);
    sensors.right = (input(1)==0); 
    sensors.front = adc_in(0);
    pause(50);
  }
}

/* Name: GetLeftSensorVal
 * Purpose:
 * Inputs:
 * Outputs:
*/
int GetLeftSensorVal(void)
{
  return sensors.left;
}

/* Name: GetRightSensorVal
 * Purpose:
 * Inputs:
 * Outputs:
*/
int GetRightSensorVal(void)
{
  return sensors.right;
}

/* Name: GetFrontSensorVal
 * Purpose:
 * Inputs:
 * Outputs:
*/
int GetFrontSensorVal(void)
{
  return sensors.front;
}


/* Name: MonitorIR
 * Purpose:
 * Inputs:
 * Outputs:
*/
/*void MonitorIR(void *arg)
{  
 if(sensors.front > 1000){
      danger = true;
  //    high(27);      
    }
    else{
      danger = false;
  //    low(27);      
    }
    //servoDir +=90;
    pause(380);
  }
}
*/