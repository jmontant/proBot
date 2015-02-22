/**
* This is the test harness for the emic handler.
* Used to validate that all of the handler functions are working properly.
*/

#include "emic_hndlr.h"
#include "simpletools.h"

int main(){
  initEmic();                             // Initialize EMIC 2 module.
  
  voice(4);
  
  say("Wouldn't you like to be a pepper to?");
/*
  sayInt(321);
  say("four out of three people have difficulty with fractions, and some robots.");

  demo(0);                                // English demo of what EMIC ccan do.
  
  say("Now let me sing you a song!");
  demo(1);                                // Sing Daisy.

  for(int i = 0; i < 9; i++)
  {
    voice(i);                             // Select each of the built-in voices.

    say("Here is one of my nine voices.");

    pause(500);
  }    

  say("two times three is.");
  sayInt(6);
*/  
  return 0;
}
