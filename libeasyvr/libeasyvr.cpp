/**
* This is the EasyVR speech recognition handler test harness.
* This code will exercise some of teh basic functionality provided in the handler.
*/

#include "simpletools.h"              // Here to provide suport for the print statements
#include "robot_defs.h"               // General robot definitions and I/O pin assignments
#include "EasyVR_Hndlr.h"             // EasyVR Speach Recognition Module Handler


int main(){
  int value;                          // Place to hold the return values from the handler calls.


  vrInit();                           // Initialize the EasyVR module.

  value = vrGetId();                  // Get the EasyVR module version ID.
  print("GetID = %d\n", value);

  vrSetLanguage(ENGLISH);             // Set the Speaker Independent language to English.
  vrTimeout(2);                       // Tell him to wait 2 seconds for a command and then time out.
  vrKnob(TYPICAL);                    // Set the Speaker Independent recognition level
  vrLevel(NORMAL);                    // Set the Speaker Dependent recognition level

  while(1){
    value = vrRecogSi(WS_ACTION);     // Listen for one of the pre-defined Action words
    switch(value){
      case(WS1_ACTION):
        print("Action\n");
        break;
      case(WS1_MOVE):
        print("Move\n");
        break;
      case(WS1_TURN):
        print("Turn\n");
        break;
      case(WS1_RUN):
        print("Run\n");
        break;
      case(WS1_LOOK):
        print("Look\n");
        break;
      case(WS1_ATTACK):
        print("Attack\n");
        break;
      case(WS1_STOP):
        print("Stop\n");
        break;
      case(WS1_HELLO):
        print("Hello\n");
        break;
      default:
        if (value<0)
          print("Recognition Error = %d\n", value);         // Error codes returned as negative numbers
        else
          print("I didn't understand that = %d\n", value);  // Returned an invalid command index.
        break;
    }
  }

  return 0;
}
