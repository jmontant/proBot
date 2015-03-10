/*
 *  Sonar Functions - General sonar interface functions
 */

#include  "mysonar.h"
#include  "servo.h"
#include  "ping.h"
#include  "robot_defs.h"
#include  "simpletools.h"



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
