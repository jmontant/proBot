/*
  Sonar - Ultrasonic Ping))) sensor handler

  written by Paul Bammel April 2014

*/

#define STOP  0
#define SCAN  1
#define SWEEP 2
#define PING  3

void init_sonarControl(void);
void sonar_control(void *par);
void pointAt(int dir);
int scan(void);
int getLeft(void);
int getRight(void);
int getCenter(void);