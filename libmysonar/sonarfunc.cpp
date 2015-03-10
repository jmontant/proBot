/*
 *  Sonar Functions - General sonar interface functions
 */

#include  "mysonar.h"
#include  "servo.h"
#include  "ping.h"
#include  "robot_defs.h"
#include  "simpletools.h"

/* Make up to a 180 degree single scan looking for the closest & farthest objects */
void sonarScan(int range){
  if (range > 90) range = 90;               // Make sure range doesn't exceed 90 degrees.
  panStart = 90 - range;                    // Start scan at range degrees left of center.  
  panEnd = 90 + range;                      // End scan at range degrees right of zero.
  sFunc = SCAN;                             // Select sweep function.
}

/* Have the sonar do continuous pings */
void  sonarPing(void){
	sFunc = PING;                              // Select continuous ping function.
}

/* Return result of an immediate ping */
int sonarPingNow(void){
  pingDist = ping_cm(PING_PIN);
  return(pingDist);                         // Return current distance to target.
}  

void  sonarSetInc(int inc){                 // Set sweep/scan increment value.
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

/* Find distance to nearby ostacles (clearance) (Left, Right, & straight ahead) */
void sonarFindClearance(void){
  sFunc = AVOID;
}  

/* Stop (at next sonar interval) any function the sonar is currently performing */
void sonarStop(void){
  servo_set(HEAD_PIN, 0);                 // Relax servo without killing servo cog to save on batteries.
  sFunc = STOP;                           // Stop during next pass through Sonar control
}  

/* Stop any sonar functions, relax the servo and kill the cog running sonar_control */
void sonarEnd(void){
  sonarStop();
  
}  

// Returns current value of sFunc to see what sonar control is doing */
int sonarGetFunction(void){
  return(sFunc);                          // What function is sonar control currently performing.
}
