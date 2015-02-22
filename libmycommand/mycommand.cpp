/*
 *  Voice Command recognition using EasyVR
 *  
 *  Written by Paul & Amy Bammel
 *  Version: 0.01 Dec 2014 Initial release
 *  
 */

#include  "robot_defs.h"        // General robot definitions and I/O pin assignments
#include  "EasyVRhndlr.h"       // EasyVR module handler
#include  "EmicHndlr.h"         // Emic text to speech module handler
#include  "mycommand.h"         // Speech command recognition library
#include  "ParserTbl.h"         // English sentence parser tables
#include  "simpletools.h"       // Provides cogStart and other basic functions


int sentence[10]  = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int lastSent[10]  = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

/* Private function prototypes */
void myCommand(void *par);      // Speech Reocognition & Synthesis handler

/* Global variables */
int mycmd_cogID = 0;            // CogId running emic, easyvr and mycommand routines
int vrStatus = FALSE;           // Status of VR module

/* Add enough for stack, Return address, & local variables used in independent cog*/ 
unsigned int mycmd_stack[(40 + (50 * 4))];


/* Launch myCommand EasyVR voice recognition in a separate cog */
int init_myCommand(void){
  mycmd_cogID = cogstart(&myCommand, NULL, mycmd_stack, sizeof(mycmd_stack));
  return mycmd_cogID;           // Return the cog number myCommand is running in.
}


/* myCommand EasyVR voice recognition routine that runs continuously in a separate cog */
void myCommand(void *par){
  vrStatus = vrInit();          // Initialize the EasyVR module
  initEmic();                   // Intialize Emic Text to Speech module

  if(vrStatus){                 // If vr initialization was successful
    vrSetLanguage(ENGLISH);     // Set the Speaker Independent language to English.
    vrTimeout(CMD_TIME);        // Tell him to wait x seconds for a command and then time out.
    vrKnob(CMD_SI_KNOB);        // Set the Speaker Independent recognition level
    vrLevel(CMD_SD_LEVEL);      // Set the Speaker Dependent recognition level

    while(1){
      vrGetTrigger(WS0_ROBOT);  // Wait for someone to say "Robot"
      
      if(getSentence()){        // If we heard a valid sentence (command)
          /* execute */
      }
    }      
  }    
}  


int Parse(struct tblParser table[]){
  
}


/* Listen for a complete sentence. Return FALSE if response is a timeout */
int getSentence(void){
  int idx = 0;                                // Index into Sentence array
  int word = -1;                              // Value "word" returned by recognition request
  
  sentence[idx++] = vrRecogSd(WG_COMMAND);    // Get the action command word
  switch(sentence[0]){
    case WG1_MOVE:                            // Move Forward or Backward
      sentence[1] = WS2_FORWARD;              // Default command: Move Forward 1 step
      sentence[2] = WS3_ONE;
      sentence[3] = WG2_STEPS;
      
      word = vrRecogSi(WS_DIRECTION);         // Get direction robot should move
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command
      
      word = vrRecogSi(WS_NUMBERS);           // Get the number of units to move
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command
      
      word = vrRecogSd(WG_DETERMINER);        // Determine what unit of measure
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command
      
      break;
      
    case WG1_TURN:                            // Turn Left or Right
      sentence[1] = WS2_RIGHT;                // Default command: Turn Right 90 degrees
      sentence[2] = WG3_NINETY;
      sentence[3] = WG2_DEGREES;
      
      word = vrRecogSi(WS_DIRECTION);         // Get direction robot should move
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command
      
      word = vrRecogSd(WG_DEGREES);           // Get the number of units to move
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command
      
      word = vrRecogSd(WG_DETERMINER);        // Determine what unit of measure
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command

      break;
      
    case WG1_LOOK:                            // Turn "head" Left or Right
      sentence[1] = WS2_FORWARD;              // Default command: Look Forward
      
      word = vrRecogSi(WS_DIRECTION);         // Get direction robot should turn head
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command

      word = vrRecogSd(WG_DEGREES);           // Get the number of units to turn head
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command
      
      word = vrRecogSd(WG_DETERMINER);        // Determine what unit of measure
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command

      break;
      
    case WG1_FACE:                            // Rotate to a particular direction or object
      sentence[1] = WG4_NORTH;                // Default command: Face North
      
      word = vrRecogSd(WG_NOUNS);             // Tell robot what to face (North, Object)
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command

      break;
      
    case WG1_AGAIN:                           // Repeat last successful command
      for(idx = 0; idx < 10; idx++){
        sentence[idx] = lastSent[idx];        // Copy last successful command into current command
      }        
      
      break;
      
    case WG1_REPORT:                          // Provide report on requested information
      sentence[1] = WG4_STATUS;               // Default command: Report Status
      
      word = vrRecogSd(WG_NOUNS);             // Get the type of report
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command

      break;
      
    case WG1_SCAN:                            // Perform a sweep of area (180 degree default)
      sentence[1] = WG3_NINETY;               // Default command: Scan 90 degrees (each way)
      sentence[2] = WG2_DEGREES;
      
      word = vrRecogSd(WG_DEGREES);           // Get the number of degrees to scan
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command
      
      word = vrRecogSd(WG_DETERMINER);        // Determine what unit of measure (better be Degrees)
      if(word >= 0) sentence[idx++] = word;   // Retain id of spoken word
      if(word == TIMEOUT) break;              // Assume defaults for this and rest of command

      break;
  }      
}
