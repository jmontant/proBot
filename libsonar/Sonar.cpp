/*
  Sonar - Ultrasonic Ping))) sensor handler

  written by Paul Bammel April 2014

*/

#include "sonar.h"
#include "servo.h"
#include "ping.h"
#include "io_pins.h"
#include "motor.h"
#include "simpletools.h"

// Add enough for stack & calcs (50 bytes declared as int 
unsigned int s_stack[(160 + (50 * 4))];

int   sonar_interval = 125;             // Run control loop this often
int   sonar_function = STOP;
int   lftDist   = 0;
int   rghtDist  = 0;
int   frntDist  = 0;

void init_sonarControl(void)
{
  int cog = cogstart(&sonar_control, NULL, s_stack, sizeof(s_stack));
}

void sonar_control(void *par)
{
  while(1)
  {
    switch(sonar_function)
    {
      case SCAN:
        pointAt(5);
        rghtDist = ping_cm(PING_PIN);
        pointAt(90);
        frntDist = ping_cm(PING_PIN);
        pointAt(170);
        lftDist = ping_cm(PING_PIN);
        pointAt(90);
        frntDist = ping_cm(PING_PIN);
        adj_bias(lftDist, rghtDist);         // Adjust motor Bias to avoid nearby objects
        break;
      case PING:
        frntDist = ping_cm(PING_PIN);
      case STOP:
      default:
        servo_set(HEAD_PIN, 0);         // Relax servo without killing servo cog to save on batteries.
        break;
    }
    pause(sonar_interval);
  }
}

void pointAt(int dir)
{
  servo_angle(HEAD_PIN, dir * 10);      // Direction times 10 beacuse servo_set is in tenths of a degree.
  pause(700);                           // Give the servo a moment to get there.
}

int scan(void)
{
  sonar_function = SCAN;
}

int getLeft(void)
{
  return lftDist;
}

int getRight(void)
{
  return rghtDist;
}

int getCenter(void)
{
  return frntDist;
}
