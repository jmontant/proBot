/*
 *  EmicHndlr.h - EMIC Text-to-Speech Synthesizer
 */

#ifndef EMIC_HNDLR_H
#define EMIC_HNDLR_H

#if defined(__cplusplus)
extern "C" {
#endif

// Demonstration messages
#define ENGLISH_INTRO 0
#define SING_DAISY    1
#define SPANISH_INTRO 2

// Nx Voice Options
#define PERFECT_PAUL      0
#define HUGE_HARRY        1
#define BEUTIFUL_BETTY    2
#define UPPITY_URSULA     3
#define DOCTOR_DENNIS     4
#define KIT_THE_KID       5
#define FRAIL_FRANK       6
#define ROUGH_RITA        7
#define WHISPERING_WENDY  8

// Select the language and/or dialect
#define ENGLISH   0
#define CSPANISH  1
#define LSPANISH  2

// Language Parser
#define DECTALK 0
#define EPSON   1

// Initialize emic board and handler
/*
 *  Open serial port to emic,
 *  validate unit is ready to receive commands, and
 *  reset emic to power up defaults
*/
int initEmic(void);

// : Check to see that emic is readyto recv a command
/*
 *  Send a CR to emic and wait for a ':' to come back.
 *  Indicates that the emic is ready to receive commands.
*/
int isIdle(void);

// Sx Convert text-to-speech: x = message (1023 characters maximum)
/*
 *  Convert the passed text string into synthesized speech.
 *  The text string is limited to 1023 characters and should terminate on a clause or sentence
 *  boundary as indicated by a full stop '.' or comma ',' punctuation mark.
 *  If the text is longer than the allowable limit, it will be truncated and may result in
 *  unintelligible speech output.
*/
int say(char *msg);

int sayInt(int value);

// Dx Play demonstration message: 
/*
 *  Play one of Emic 2’s built-in demonstration messages:
 *  x = 0 (Speaking), 1 (Singing), 2 (Spanish)
*/
int demo(int value);

// X Stop playback (while message is playing)
/*
 *  Immediately stop the currently playing text-to-speech message.
 *  This command is only valid while a message is playing.
*/
int stop(void);

// Z Pause/un-pause playback (while message is playing)
/*
 *  Immediately pause or unpause the currently playing text-to-speech message.
 *  Emic 2 will respond with a “.” indicating that the command has successfully
 *  been received. While the playback is paused, the LED will remain RED.
 *  This command is only valid while a message is playing.
*/
int rest(void);
int resume(void);

// Nx Select voice:
/*
 *   x = 0 to 8
 *   x = 0 (default)
*/
int voice(int value);

// Vx Set audio volume (dB):
/*
 *  x = -48 (Softest) to 18 (Loudest)
 *  x = 0 (default) 
*/
int volume(int value);

// Wx Set speaking rate (words/minute): x = 75 to 600
/*
 *  Set the speaking rate in words per minute from 75 (slowest) to 600 (fastest).
 *  x = 200 (default)
*/
int rate(int value);

// Lx Select language:
/*
 *  Select the language and/or dialect to be used for text-to-speech conversion:
 *  x = 0 (US English), 1 (Castilian Spanish), 2 (Latin Spanish)
 *  x = 0 (default)
*/
int language(int value);

// Px Select parser:
/*
 *  Select the text parsing engine to be used during text-to-speech conversion:
 *  x = 0 (DECtalk), 1 (Epson)
 *  x = 1 (default)
*/
int parser(int value);

// R Revert to default text-to-speech settings
/*
 *  Resets the user-configurable text-to-speech settings to their default, power-up configuration:
 *  Voice = 0 (Paul)
 *  Volume = 0 dB
 *  VolRate = 200 words/minute
 *  Language = 0 (US English)
 *  Parser = 1 (Epson)
*/
int reset(void);

// C Print current text-to-speech settings
/*
 *  Displays the current values of the user-configurable text-to-speech settings.
 *  THIS COMMAND HAS NOT BEEN IMPLEMENTED IN THIS DRIVER
*/

// I Print version information
/*
 *  Displays version information for the Emic 2 Text-to-Speech Module.
 *  This data is useful for troubleshooting and debugging.
 *  THIS COMMAND HAS NOT BEEN IMPLEMENTED IN THIS DRIVER
*/

// H Print list of available commands
/*
 *  Lists all available commands for the Emic 2 Text-to-Speech Module.
 *  THIS COMMAND HAS NOT BEEN IMPLEMENTED IN THIS DRIVER
*/

#if defined(__cplusplus)
}
#endif
/* __cplusplus */  
#endif
/* EMIC_HNDLR_H */  

/**
 * TERMS OF USE: MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
