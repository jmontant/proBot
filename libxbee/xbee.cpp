#include "xbee.h"
#include "io_pins.h"
#include "fdserial.h"
#include "simpletools.h"

fdserial *xbee;
char c;
char prevC;

void InitXBee(void)
{
  c='1';
  prevC='1';
  xbee = fdserial_open(XBEE_DO_PIN, XBEE_DI_PIN, 0, 9600);
  pause(100);
}

void XBee(void *arg)
{
  //dprint(xbee,"\n------XBeeComm\n")  ; 
   while(1)
    { 
      if(fdserial_rxReady(xbee) != -1)
      {
        c = fdserial_rxChar(xbee);
        if(c != prevC)
        {
          switch(c)
          {
            case 'a':
            { 
              break;
            }    
            case 'd':
            {
  
              break;
            }
            case 'w':
            {
          
              break;
            }
            case 's':
            {

              break;
            }
            case 'e':
            {
         
              break;
            }  
            case 'q':
            {
              break;
            }         
          } 
          prevC=c;
          pause(20);          
        }      
      }      
    } 
}
