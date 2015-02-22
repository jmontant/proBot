/**
 *  EMIC 2 Text to Speech handler
 *  written by Paul Bammel  September 2014
 *
*/

#define COLON 58

#include "simpletools.h"
#include "emic_hndlr.h"
#include "robot_defs.h"

// Private Templates
char emic_read(void);
int emic_write(char ch);
char* itoa(int n);

  serial *emic = serial_open(EMIC_SOUT_PIN, EMIC_SIN_PIN, 0, 9600);

/*
 *  initEmic - Validate unit is ready to receive commands, and
 *  reset emic to power up defaults
*/
int initEmic(void)
{
  char ch;
  if(isIdle() == TRUE)                  // If EMIC is currently idle.
  {
    reset();                          // Reset EMIC to power-up defaults.
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // Failure
  }  
}

/*
 *  : Check to see that emic is ready to recv a command
 *
 *  Send a ':' to emic and wait for a ':' to come back.
 *  Indicates that the emic is ready to receive commands.
*/
int isIdle(void)
{
  char ch;
  
  emic_write(CR);                     // Write Carriage Return out to EMIC
  do
  {
    ch = emic_read();
  } while(ch != COLON);
/*
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }
*/
}

/*
 *  Sx Convert text-to-speech:
 *
 *  Convert the passed text string into synthesized speech.
 *  The text string is limited to 1023 characters and should terminate on a clause or sentence
 *  boundary as indicated by a full stop '.' or comma ',' punctuation mark.
 *  If the text is longer than the allowable limit, it will be truncated and may result in
 *  unintelligible speech output.
*/
int say(char* msg)
{
  char ch;

  dprint(emic, "S%s\n", msg);    // Select desired demo message.
  do{
    ch = emic_read();
  } while(ch != COLON);
}

int sayInt(int value)
{
  char ch;
  
  dprint(emic, "S%s.\n", itoa(value));    // Select desired demo message.
  do{
    ch = emic_read();
  } while(ch != COLON);
}
/*
 *  Dx Play demonstration message:
 *
 *  Play one of Emic 2’s built-in demonstration messages:
 *  x = 0 (Speaking), 1 (Singing), 2 (Spanish)
*/
int demo(int value)
{
  dprint(emic, "D%s\n", itoa(value));    // Select desired demo message.
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

/*
 *  X Stop playback (while message is playing)
 *
 *  Immediately stop the currently playing text-to-speech message.
 *  This command is only valid while a message is playing.
*/
int stop(void)
{
  emic_write('X');                    // Write 'X' out to EMIC to stop playback.
  emic_write('\n');
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

/*
 *  Z Pause/un-pause playback (while message is playing)
 *
 *  Immediately pause or unpause the currently playing text-to-speech message.
 *  Emic 2 will respond with a “.” indicating that the command has successfully
 *  been received. While the playback is paused, the LED will remain RED.
 *  This command is only valid while a message is playing.
*/
int rest(void)
{
  emic_write('Z');                    // Write 'Z' out to EMIC to pause current speech.
  emic_write('\n');
  if(emic_read() == '.')              // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

int resume(void)
{
  emic_write('Z');                    // Write 'Z' out to EMIC to resume current speech.
  emic_write('\n');
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

/*
 *  Nx Select voice:
 *
 *   x = 0 to 8
 *   x = 0 (default)
*/
int voice(int value)
{
  char ch;

  dprint(emic, "N%s\n", itoa(value));    // Select desired built-in voice.
      do
      {
        ch = emic_read();
      } while(ch != COLON);
/*
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
*/
  return TRUE;
}

/*
 *  Vx Set audio volume (dB):
 *
 *  x = -48 (Softest) to 18 (Loudest)
 *  x = 0 (default) 
*/
int volume(int value)
{
  dprint(emic, "V%s\n", itoa(value));    // Select desired audio volume.
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

/*
 *  Wx Set speaking rate (words/minute): x = 75 to 600
 *
 *  Set the speaking rate in words per minute from 75 (slowest) to 600 (fastest).
 *  x = 200 (default)
*/
int rate(int value)
{
  dprint(emic, "W%s\n", itoa(value));    // Select desired talking speed.
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

/*
 *  Lx Select language:
 *
 *  Select the language and/or dialect to be used for text-to-speech conversion:
 *  x = 0 (US English), 1 (Castilian Spanish), 2 (Latin Spanish)
 *  x = 0 (default)
*/
int language(int value)
{
  dprint(emic, "L%s\n", itoa(value));    // Select desired language.
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

/*
 *  Px Select parser:
 *
 *  Select the text parsing engine to be used during text-to-speech conversion:
 *  x = 0 (DECtalk), 1 (Epson)
 *  x = 1 (default)
*/
int parser(int value)
{
  dprint(emic, "P%s\n", itoa(value));    // Select desired parser.
  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

/*
 *  R Revert to default text-to-speech settings
 *
 *  Resets the user-configurable text-to-speech settings to their default, power-up configuration:
 *  Voice = 0 (Paul)
 *  Volume = 0 dB
 *  VolRate = 200 words/minute
 *  Language = 0 (US English)
 *  Parser = 1 (Epson)
*/
int reset(void)
{
  emic_write('R');                    // Write 'R' out to EMIC to reset unit.
  emic_write('\n');

  if(emic_read() == COLON)            // If we get a COLON back...
  {
    return TRUE;                      // Success!
  }  
  else
  {
    return FALSE;                     // else Failure.
  }  
}

// Below this point are Private functions used internally by the handler.

// Read a character back from the emic
char emic_read(void)
{
  char result;
  
  result = serial_rxChar(emic);
  return result;
}

// Write a single character out to the emic
int emic_write(char ch)
{
  serial_txChar(emic, ch);
}

// itoa:  convert n to characters in s
char* itoa(int n)
{
  char str[5] = {'\0','\0','\0','\0','\0'};
  
  sprintf(str, "%d", n);
  return str;
}

