/*
 *  Sonar - Ultrasonic Ping))) sensor handler
 *
 *  written by Paul Bammel April 2014
 *
 */

#include "mysonar.h"
#include "servo.h"
#include "ping.h"
#include "robot_defs.h"
#include "simpletools.h"

// Function names
#define SWEEP 1
#define SCAN  2
#define PING  3
#define EDGES 4

/* Add enough for stack, Return address, & local variables */ 
unsigned int mysnr_stack[(40 + (50 * 4))];

int   sonar_cogID = 0;                      // Address of sonar_control function running in cog
static volatile int sonar_interval = 250;   // Run control loop this often
static volatile int sFunc     = STOP;       // Current sonar function being performed
static volatile int pingDist  = 0;          // Current distance returned by Ping sensor.
static volatile int pingDir   = 0;          // Current direction Ping sensor (head) is facing.
static volatile int minDist   = 0;          // Distance to closest object ping can see
static volatile int minDir    = 0;          // Direction to closest object seen
static volatile int maxDist   = 0;          // Distance to farthest object ping can see
static volatile int maxDir    = 0;          // Direction to farthest object seen
static volatile int sweepInc  = 5;          // Number of degrees to advance while scanning/sweeping
static volatile int panStart  = 0;          // Right limit(edge) of Sweep/Scan operation.
static volatile int panEnd    = 180;        // Left limit(edge) of Sweep/Scan operation.

/* Private sonar function prototypes */
void  sonar_control(void *par);           // Independent cog sonar control template definition.
int   findLeftEdge(int type);             // Left Edge detection function template definition.
int   findRightEdge(int type);            // Right Edge detection function template definition.

/* Launch ping control in a separate cog */
int initSonarControl(void)
{
  sonar_cogID = cogstart(&sonar_control, NULL, mysnr_stack, sizeof(mysnr_stack));
  sonarPointAt(90);                       // Start out with ping sensor facing forward.
  return sonar_cogID;                     // Return the cog number sonar_control is running in.
}

/* Ping control routine that runs continuously in a separate cog */
void sonar_control(void *par)
{
  while(1){
    switch(sFunc){
      case SCAN:
      case SWEEP:
        minDist = 1000;                   // Start with minimum distance set really large.
        maxDist = 0;                      // Start scan with max distance really small.
        
        sonarPointAt(panStart);           // Start scan with head facing far right.
        while(pingDir <= panEnd){         // Until a full 180 degree sweep is complete,
          pingDist = ping_cm(PING_PIN);   // Get distance for current direction.
          if(pingDist < minDist){         // If the current distance is smallest ever seen
            minDist = pingDist;           // Set minDist to this new value
            minDir = pingDir;             //  and remember the current direction the head is facing.
          }
          if(pingDist > maxDist){         // If the current distance is largest ever seen
            maxDist = pingDist;           // Set maxDist to this new value
            maxDir = pingDir;             //  and remember the current direction the head is facing.
          }
          sonarPointAt(pingDir + sweepInc);    // Now turn the head and repeat the process.
        }
        if(sFunc == SCAN){
          sFunc = STOP;                     // When we're done, Stop so we don't do it again.
          break;
        }          

      case PING:
        pingDist = ping_cm(PING_PIN);     // Ping once every servo interval.
        break;
      
      default:
        break;
    }
    pause(sonar_interval);                // Wait a moment and run through the loop again.
  }
}

void sonarPointAt(int angle){
  if (angle == pingDir) return;
  servo_angle(HEAD_PIN, angle * 10);      // Angle * 10 beacuse servo_set is in tenths of a degree.
  pause(abs((angle - pingDir) * 5));      // Give the servo a moment to get there.
  pingDir = angle;                        // Set pingDir equal to the angle (head) is now facing.
}

/* Make up to a 180 degree single scan looking for the closest & farthest objects */
void sonarScan(int range){
  if (range > 90) range = 90;             // Make sure range doesn't exceed 90 degrees.
  panStart = 90 - range;                  // Start scan at range degrees left of center.  
  panEnd = 90 + range;                    // End scan at range degrees right of zero.
  sFunc = SCAN;                           // Select sweep function.
}

/* Have the sonar do continuous pings */
void  sonarPing(void){
	sFunc = PING;                            // Select continuous ping function.
}

/* Return result of an immediate ping */
int sonarPingNow(void){
  pingDist = ping_cm(PING_PIN);
  return(pingDist);                       // Return current distance to target.
}  

void  sonarSetInc(int inc){               // Set sweep/scan increment value.
  sweepInc = inc;
}  

/* Return requested Dist & Dir values */
target sonarGetTarget(int type){
  struct target ping;
  
  switch(type){
    case CLOSEST:
      ping.dist = minDist;
      ping.dir  = minDir;
      break;
    case FARTHEST:
      ping.dist = maxDist;
      ping.dir  = maxDir;
      break;
    default:
      ping.dist = pingDist;
      ping.dir  = pingDir;
      break;
  }    
  return(ping);                           // Return latest ping distance.
}

/* Stop (at next sonar interval) any function the sonar is currently performing */
void sonarStop(void){
  servo_set(HEAD_PIN, 0);                 // Relax servo without killing servo cog to save on batteries.
  sFunc = STOP;                           // Stop during next pass through Sonar control
}  

/* Stop any soar functions, relax the servo and kill the cog running sonar_control */
void sonarEnd(void){
  sonarStop();
  
}  

// Returns current value of sFunc to see what sonar control is doing */
int sonarGetFunction(void){
  return(sFunc);                          // What function is sonar control currently performing.
}

/* Return angle robot needs to turn to face center of object */
int sonarFindTarget(int type){
  int   leftEdge  = 180;                  // Angle in degrees to left edge of object.
  int   rightEdge = 0;                    // Angle in degrees to right edge of object.
  float hAngle    = 0.0;                  // Angle head needs to face for center of object.
  float num       = 0.0;                  // Numerator
  float den       = 0.0;                  // Denominator
  float rAngle    = 0.0;                  // Angle robot needs to face to point at center of object.
  
  leftEdge = findLeftEdge(type);          // Find the left edge of the object.
  rightEdge = findRightEdge(type);        // Find the right edge of the object.
  
  hAngle = ((leftEdge + rightEdge) / 2);  // Angle for center of the object (head perspective).
  sonarPointAt((int) hAngle);             // Turn head toward center of object.
  sonarPingNow();                         // Confirm distance to target.
  
  num = ((pingDist * sin(hAngle*PI/180)) + 6.5); // Establish the numerator of our trig calculation.
  den = (pingDist * cos(hAngle*PI/180));  // Establish the denominator of the triq calculation.
  rAngle = num / den;                     // Divide the numerator by the denominator.
  rAngle = atan(rAngle)*180/PI;           // Determine the ArcTan of the resulting angle.
  if(rAngle > 0) rAngle = 90 - rAngle;    // If angle positive turn right 90-angle degrees.
  if(rAngle < 0) rAngle = -1 * (90 + rAngle); // If negative, turn left 90-angle degrees.
  return((int) rAngle);                   // Return integer value of rAngle.
}  

/* Find the left edge of either the closest or farthest object */
int findLeftEdge(int type){
  int tarDir = 0;                         // Direction to target.
  int tarDist = 0;                        // Distance to target.
  
  if(type == CLOSEST){                    // If looking for edge of the closest object
    tarDir = minDir;                      //  Use the short distance values.
    tarDist = minDist;
  } else {                                // Else if looking for the edge of the farthest object
    tarDir = maxDir;                      //  Use the long distance values.
    tarDist = maxDist;
  }    
  
  while(tarDir < 180){                    // Until we reach or exceed 180 degrees (Left)
    sonarPointAt(++tarDir);               // Keep looking further left (1 degree at a time)
    pingDist = ping_cm(PING_PIN);         // Checking distance to target
    if(type == CLOSEST && pingDist >= tarDist+10){  // Difference of 10cm determines edge.
      return tarDir;
    } else {
      if(type == FARTHEST && pingDist <= tarDist-10){
        return tarDir;
      }        
    }
  }          
  return(tarDir);                         // Target edge not found, use 180 degrees.
}


/* Find the right edge of either the closest or farthest object */
int findRightEdge(int type){
  int tarDir = 0;                         // Direction to target.
  int tarDist = 0;                        // Distance to target.
  
  if(type == CLOSEST){                    // If looking for edge of the closest object
    tarDir = minDir;                      //  Use the short distance values.
    tarDist = minDist;
  } else {                                // Else if looking for the edge of the farthest object
    tarDir = maxDir;                      //  Use the long distance values.
    tarDist = maxDist;
  }    
  
  while(tarDir > 0){                      // Until we reach or exceed 0 degrees (Right)
    sonarPointAt(--tarDir);               // Keep looking further left (1 degree at a time)
    pingDist = ping_cm(PING_PIN);         // Checking distance to target
    if(type == CLOSEST && pingDist >= tarDist+10){  // Difference of 10cm determines edge.
      return tarDir;
    } else {
      if(type == FARTHEST && pingDist <= tarDist-10){
        return tarDir;
      }
    }
  }
  return(tarDir);                         // Target edge not found, use zero degrees.
}

