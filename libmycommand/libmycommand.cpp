/*
 *  Voice Command recognition using EasyVR
 *  
 *  Written by Paul & Amy Bammel
 *  Version: 0.01 Dec 2014 Initial release
 *  
 */
#include  "EasyVRhndlr.h"
#include  "EmicHndlr.h"

#include  "mycommand.h"           // Speech command recognition library

int main(){
  init_myCommand();               // Intialize myCommand EasyVR speech recognition

  return 0;
}
