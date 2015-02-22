/*
 *  EasyVRhndlr.h - EasyVR handler .h header file
 *  Required I/O pin definitions expected in robot_defs.h
 *    VR_RX_PIN - Propeller to recv data from EasyVR.
 *    VR_TX_PIN - Propeller to xmit data to EasyVR.
 *
 *    LED_VR_PIN  - LED to blink when waiting for a command word
 *    LED_ERR_PIN - LED to blink when an error occurs listening for a command word
 *    Note: Both of these LED's will blink together while waiting for a Trigger word
 *    Note: The LED pins MUST be two consecutive I/O pins.
 *    
 */

#ifndef EASYVR_HNDLR_H
#define EASYVR_HNDLR_H

#if defined(__cplusplus)
extern "C" {
#endif

// Easy VR Commands
#define CMD_BAUDRATE   'a'           // Set serial communication baud-rate
#define CMD_BREAK      'b'           // Abort recognition, training, or playback if in progress
#define CMD_COUNT_SD   'c'           // Request count of SD commands in the specified group
#define CMD_DELAY      'y'           // Set serial communication transmit delay
#define CMD_DUMP_SD    'p'           // Read SD command data (label and training)
#define CMD_DUMP_SX    'h'           // Read wave table data
#define CMD_ERASE_SD   'e'           // Erase training of SD command
#define CMD_GROUP_SD   'g'           // Insert a new SD command
#define CMD_ID         'x'           // Request firmware identification
#define CMD_KNOB       'k'           // Set SI recognition level
#define CMD_LANGUAGE   'l'           // Set SI language
#define CMD_LEVEL      'v'           // Set SD recognition level
#define CMD_MASK_SD    'm'           // Request bit map of non-empty groups
#define CMD_NAME_SD    'n'           // Label SD command
#define CMD_PLAY_SX    'w'           // Wave table entry playback
#define CMD_QUERY_IO   'q'           // Configure, query or modify general purpose I/O pins
#define CMD_RECOG_SD   'd'           // Activate SD recognition for specified WordGroup
#define CMD_RECOG_SI   'i'           // Activate SI recognition for specified WordSet
#define CMD_RESETALL   'r'           // Reset all commands and groups
#define CMD_SLEEP      's'           // Go into specified Power-Down mode
#define CMD_TIMEOUT    'o'           // Set recognition Timeout period
#define CMD_TRAIN_SD   't'           // Train specified SD command
#define CMD_UNGROUP_SD 'u'           // Remove SD command

  // Baudrate Enumerations
#define   B115200  1            
#define   B57600   2
#define   B38400   3
#define   B19200   6
#define   B9600    12
  // SI recognition Knob setting enumerations
#define   LOOSER   0
#define   LOOSE    1
#define   TYPICAL  2
#define   STRICT   3
#define   STRICTER 4
  // SD recognition Level enumerations
#define   EASY     1
#define   NORMAL   2
#define   HARD     3
#define   HARDER   4
#define   HARDEST  5
  // SI Language enumerations
#define   ENGLISH   0
#define   ITALIAN   1
#define   JAPENESE  2
#define   GERMAN    3
#define   SPANISH   4
#define   FRENCH    5
  // Sleep command enumerations
#define   Wake_on_char      0
#define   Wake_on_whistle   1
#define   Wake_on_loudsound 2
#define   Wake_on_2claps    3
#define   Wake_on_3claps    6
#define   Clap_sense_low    0
#define   Clap_sense_mid    1
#define   Clap_sense_high   2
  // SI Word Sets and Words
#define   WS_TRIGGER    0
#define   WS_ACTION     1
#define   WS_DIRECTION  2
#define   WS_NUMBERS    3
// Word Set 0 Trigger Words
#define   WS0_ROBOT     0
// Word Set 1 Action Words
#define   WS1_ACTION    0
#define   WS1_MOVE      1
#define   WS1_TURN      2
#define   WS1_RUN       3
#define   WS1_LOOK      4
#define   WS1_ATTACK    5
#define   WS1_STOP      6
#define   WS1_HELLO     7
  // SI Word Set 2 Direction words
#define   WS2_LEFT      0
#define   WS2_RIGHT     1
#define   WS2_UP        2
#define   WS2_DOWN      3
#define   WS2_FORWARD   4
#define   WS2_BACKWARD  5
  // SI Word Set 3 Number words
#define   WS3_ZERO      0
#define   WS3_ONE       1
#define   WS3_TWO       2
#define   WS3_THREE     3
#define   WS3_FOUR      4
#define   WS3_FIVE      5
#define   WS3_SIX       6
#define   WS3_SEVEN     7
#define   WS3_EIGHT     8
#define   WS3_NINE      9
#define   WS3_TEN       10
  // Easy VR valid ID values
#define   VRBOT         0
#define   EASYVR        2
  // EasyVR I/O line enumerations
#define   IO1           1
#define   IO2           2
#define   IO3           3
#define   OUT_LOW       0
#define   OUT_HIGH      1
#define   IN_HIZ        2
#define   IN_STRONG     3
#define   IN_WEAK       4
  // Play .wav file volume enumerations
#define   VOL_MIN       0
#define   VOL_HALF      7
#define   VOL_FULL      15
#define   VOL_DOUBLE    31

#define   STS_AWAKEN      'w'             // Wake-up (back from power-down mode)
#define   STS_COUNT       'c'             // Count of commands
#define   STS_DATA        'd'             // Provide command data
#define   STS_ERROR       'e'             // Signal recognition error
#define   STS_ID          'x'             // Provide firmware identification
#define   STS_INTERR      'i'             // Interrupted recognition
#define   STS_INVALID     'v'             // Invalid command or argument
#define   STS_MASK        'k'             // Mask of non-empty groups
#define   STS_OUT_OF_MEM  'm'             // Memory error
#define   STS_PIN         'p'             // Provide I/O pin input status
#define   STS_RESULT      'r'             // Recognized SD command or Training similar to SD command
#define   STS_SIMILAR     's'             // Recognized SI word; or Training similar to SD word
#define   STS_SUCCESS     'o'             // Okay, or no errors status
#define   STS_TABLE_SX    'h'             // Provide sound table data
#define   STS_TIMEOUT     't'             // Timeout expired

#define   ARG_MIN         '@'
#define   ARG_MAX         'l'
#define   ARG_ZERO        'A'
#define   ARG_ACK         ' '             // ACK sent to read additional status arguments

#define   ERRNOISE        3               // Acquired sound too noisy
#define   ERRSOFT         4               // Spoke too soft
#define   ERRLOUD         5               // Spoke too loud
#define   ERRSOON         6               // Spoke too soon
#define   ERRCHOPPY       7               // Too many segments/too complex
#define   ERRFAIL         17              // Recognition failed
#define   ERRLCONF        18              // Recognition result doubtful
#define   ERRMCONF        19              // Recognition result maybe
#define   ERRBTEMP        20              // Invalid SD command stored in memory
#define   ERRDURR         23              // Bad pattern durations

#define   TIMEOUT		  -1
#define   BREAK_TRIES     8
#define   VR              1
#define   ERR             2
#define   TRIG            3

int vrGetTrigger(int word);               // Listen for particular trigger word
int vrInit(void);                         // Initialize serial port and send Break to EasyVR module
int vrBreak(void);                        // Wake up VR module and stop any operations
int vrSleep(int mode);                    // Put EasyVR module into specified power-down state
int vrKnob(int setting);                  // Set SI recognition level
int vrLevel(int setting);                 // Set SD recognition level
int vrSetLanguage(int si_lang);           // Set SI recognition language
int vrTimeout(int Value);                 // Recognition Timeout value
int vrRecogSi(int wordset);               // Recognize a SI word from Wordset
int vrRecogSd(int wordgroup);             // Recognize a SD word from Wordgroup
int vrGetId(void);                        // Get EasyVR ID code
int vrBaudrate(int baud);                 // Set baudrate on EasyVR module

#if defined(__cplusplus)
}
#endif
/* __cplusplus */  
#endif
/* EASYVR_HNDLR_H */  

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


