//
// Recreated EasyVR Handler from SPIN code on Sat Jan 25 10:11:14 2014
// Both written by Paul Bammel 
//

#include "simpletools.h"
#include "fdserial.h"
#include "robot_defs.h"
#include "EasyVR_Hndlr.h"


//#define VR_DEBUG

int   vrWrite(char Ch);                           // Write to EasyVR module
char  vrRead(void);                               // Read from EasyVR module
int   vrBlink(int led);                           // Blink LED appropriately

fdserial *vr;                                     // Pointer to serial EasyVR port

/* Loop forever waiting for the valid trigger word */
int vrGetTrigger(int word){
  vrBlink(TRIG);                                  // Blink that we are listening for a trigger word
  switch(vrRecogSi(WS_TRIGGER)){                  // Listen for trigger word to be spoken
    case -1:
      break;                                      // Timeout - Nothing heard, let's keep listening
    case WS0_ROBOT:
      return 0;                                   // Somebody said the secret word
      break;
    default :
      vrBlink(ERR);                               // Error listening for trigger. Keep trying.
      break;
  }
}

/* Intitialize the serial port, the VR LED, and send the break command to the EasyVR module */
int vrInit(void){
  vr = fdserial_open(VR_RX_PIN, VR_TX_PIN, 0, 9600);
  pause(100);
  set_direction(LED_VR_PIN, 1);
  set_direction(LED_ERR_PIN, 1);
  set_output(LED_VR_PIN, 0);
  set_output(LED_ERR_PIN, 0);
  
  return vrBreak();
}

/* Interrupt any active processes and return module to the idle state */
int vrBreak(void){
  char	status;
  int x;

  for(x = 1; x <= BREAK_TRIES; x++){
    vrWrite(CMD_BREAK);
    status = vrRead();
#ifdef VR_DEBUG
    print("Break status = %c\n", status);
#endif
    if (status == STS_SUCCESS){
      return TRUE;
    } else if (status == STS_INTERR || status == STS_AWAKEN){
      continue;
    }
  }
  return FALSE;
}

/* Put EasyVR module to sleep with particular wake mode */
int vrSleep(int mode){
  vrWrite(CMD_SLEEP);
  vrWrite((ARG_ZERO + mode));
  if (vrRead() == STS_SUCCESS)
    return TRUE;
  else
    return FALSE;
}

int vrKnob(int setting){
  vrWrite(CMD_KNOB);
  vrWrite((setting + ARG_ZERO));
  if (vrRead() == STS_SUCCESS) {
    return -1;
  } else {
    return 0;
  }
}

int vrLevel(int setting){
  vrWrite(CMD_LEVEL);
  vrWrite((setting + ARG_ZERO));
  if (vrRead() == STS_SUCCESS) {
    return -1;
  } else {
    return 0;
  }
}

int vrSetLanguage(int si_lang){
  vrWrite(CMD_LANGUAGE);
  vrWrite((si_lang + ARG_ZERO));
  if (vrRead() == STS_SUCCESS) {
    return -1;
  } else {
    return 0;
  }
}

int vrTimeout(int value){
  vrWrite(CMD_TIMEOUT);
  vrWrite((value + ARG_ZERO));
  if (vrRead() == STS_SUCCESS) {
    return -1;
  } else {
    return 0;
  }
}

int vrRecogSi(int wordset){
  int	err;
  char ch;

#ifdef  VR_DEBUG
  print("Recog_si - Wordset %c\n", wordset + ARG_ZERO);
#endif
  vrBlink(VR);
  vrWrite(CMD_RECOG_SI);
  vrWrite((wordset + ARG_ZERO));
  ch = vrRead();
#ifdef  VR_DEBUG
  print(" Recog_si Result = %c\n", ch);
#endif
  if (ch == STS_SIMILAR){
    vrWrite(ARG_ACK);
    return (vrRead() - ARG_ZERO);
  } else if (ch == STS_TIMEOUT){
    return (-1);
  } else if (ch == STS_ERROR){
    vrWrite(ARG_ACK);
    err = ((vrRead() - ARG_ZERO) * 16);
    vrWrite(ARG_ACK);
    err = (err + (vrRead() - ARG_ZERO));
    vrBlink(ERR);
    return (-err);
  }
}

int vrRecogSd(int wordgroup){
  int	err;
  char ch;
  vrBlink(VR);
  vrWrite(CMD_RECOG_SD);
  vrWrite((wordgroup + ARG_ZERO));
  ch = vrRead();
  if (ch == STS_RESULT) {
    vrWrite(ARG_ACK);
    return (vrRead() - ARG_ZERO);
  } else if (ch == STS_TIMEOUT) {
    return 0;
  } else if (ch == STS_ERROR) {
    vrWrite(ARG_ACK);
    err = ((vrRead() - ARG_ZERO) * 16);
    vrWrite(ARG_ACK);
    err = (err + (vrRead() - ARG_ZERO));
    vrBlink(ERR);
    return (-err);
  }
}

int vrGetId(void){
  vrWrite(CMD_ID);
  if (vrRead() == STS_ID) {
    vrWrite(ARG_ACK);
    return (vrRead() - ARG_ZERO);
  } else {
    return (-1);
  }
}

int vrBaudrate(int baud){
  vrWrite(CMD_BAUDRATE);
  vrWrite((baud + ARG_ZERO));
  if (vrRead() == STS_SUCCESS) {
    return -1;
  } else {
    return 0;
  }
}

int vrWrite(char ch){
#ifdef VR_DEBUG
  print(" vrWrite = %c\n", ch);
#endif
  fdserial_txChar(vr, ch);
}

char vrRead(void){
  char result;
  result = fdserial_rxChar(vr);
#ifdef VR_DEBUG
  print(" vrRead = %c\n", result);
#endif
  return result;
}

int vrBlink(int led){
  switch(led) {
    case TRIG:
      set_outputs(LED_VR_PIN, LED_ERR_PIN, TRIG);
      pause(600);
      set_outputs(LED_VR_PIN, LED_ERR_PIN, 0);
      break;
    case VR:
      set_output(LED_VR_PIN, 1);
      pause(600);
      set_output(LED_VR_PIN, 0);
      break;
    case ERR:
      set_output(LED_ERR_PIN, 1);
      pause(600);
      set_output(LED_ERR_PIN, 0);
      break;
  }
}