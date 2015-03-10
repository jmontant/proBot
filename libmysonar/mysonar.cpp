/*
 *  Sonar - Ultrasonic Ping))) sensor handler
 *
 *  written by Paul Bammel April 2014
 *
 */

#include  "mysonar.h"
#include  "servo.h"
#include  "ping.h"
#include  "robot_defs.h"
#include  "simpletools.h"


/* Add enough for stack, Return address, & local variables */ 
unsigned int mysnr_stack[(40 + (50 * 4))];

int   sonar_cogID = 0;                      // Sonar_control function cog ID.
int sonar_interval = 250;                   // Run control loop this often
static struct obstacle lastCheck;           // Previous clearance check Left, Right, & Center.
static struct obstacle slope;               // Use structure to log slope between checks

/* Launch ping control in a separate cog */
int initSonarControl(void)
{
  sonar_cogID = cogstart(&sonar_control, NULL, mysnr_stack, sizeof(mysnr_stack));
  sonarPointAt(90);                         // Start out with ping sensor facing forward.
  return sonar_cogID;                       // Return the cog number sonar_control is running in.
}

/* Ping control routine that runs continuously in a separate cog */
void sonar_control(void *par)
{
  while(1){
    switch(sFunc){
      case SCAN:
      case SWEEP:
        minDist = 1000;                     // Start with minimum distance set really large.
        maxDist = 0;                        // Start scan with max distance really small.
        
        sonarPointAt(panStart);             // Start scan with head facing far right.
        while(pingDir <= panEnd){           // Until a full 180 degree sweep is complete,
          
          if(pingDist < minDist){           // If the current distance is smallest ever seen
            minDist = pingDist;             // Set minDist to this new value
            minDir = pingDir;               //  and remember the direction the head is facing.
          }
          if(pingDist > maxDist){           // If the current distance is largest ever seen
            maxDist = pingDist;             // Set maxDist to this new value
            maxDir = pingDir;               //  and remember the direction the head is facing.
          }
          sonarPointAt(pingDir + sweepInc); // Now turn the head and repeat the process.
        }
        if(sFunc == SCAN){
          sFunc = STOP;                     // When we're done, Stop so we don't do it again.
          break;
        }          

      case PING:
        pingDist = ping_cm(PING_PIN);       // Ping once every servo interval.
        break;
      
      case CHECK:
        sonarPointAt(0);                            // Look to far right side (0 degrees).
        if(pingDist < 15){                          // AS long as we're close enough.
          slope.right = pingDist - lastCheck.right; // Calculate slope since last check.
        } else {
          slope.right = 0;                          // Ignore slope if clearance is okay.
        }          
        lastCheck.right = pingDist;                 // Remember distance for next time.
        
        sonarPointAt(180);                          // Look to far left side (180 degrees).
        if(pingDist < 15){                          // AS long as we're close enough.
          slope.left = pingDist - lastCheck.left;   // Calculate slope since last check.
        } else {
          slope.left = 0;                           // Ignore slope if clearance is okay.
        }          
        lastCheck.left = pingDist;                  // Remember distance for next time.
        
        sonarPointAt(90);                           // Look straight ahead (90 degrees).
        slope.center = pingDist;                    // Save distance for "stop" check.
        
        
        
        sFunc = STOP;                               // Only check clearance once.
        break;
      
      case POINT:
        if (newDir != pingDir){
          servo_angle(HEAD_PIN, newDir * 10);       // Angle*10 servo_set is in tenths of a degree.
          pause(abs((newDir - pingDir) * 5));      // Give the servo a moment to get there.
          pingDir = newDir;                         // Set pingDir equal to "head" angle.
        }    
        pingDist = ping_cm(PING_PIN);               // Get distance for current direction.
        sFunc = STOP;                               // Only turn head once.
        break;
        
      default:
        servo_set(HEAD_PIN, 0);                     // Relax servo without killing servo cog.
        sFunc = STOP;                               // Just stop and wait for new request.
        break;
    }
    pause(sonar_interval);                          // Wait a moment then loop again.
  }
}

/*
 * Point head "ping)))" in a particular direction
 *  and update distance value.
 */
void sonarPointAt(int angle){
  if (angle != pingDir){
    servo_angle(HEAD_PIN, angle * 10);      // Angle*10 servo_set is in tenths of a degree.
    pause(abs((angle - pingDir) * 5));      // Give the servo a moment to get there.
    pingDir = angle;                        // Set pingDir equal to "head" angle.
  }    
  pingDist = ping_cm(PING_PIN);             // Get distance for current direction.
}

/* Generic robot command structure interface */
struct cmd_struct sonarCommand(struct cmd_struct cmdRequest){
  int range;
  struct cmd_struct cmdResult;
  
  switch(cmdRequest.action){
    case SWEEP:
    case SCAN:
      range = cmdRequest.value;
      if (range > 90) range = 90;               // Make sure range doesn't exceed 90 degrees.
      panStart = 90 - range;                    // Start scan at range degrees left of center.  
      panEnd = 90 + range;                      // End scan at range degrees right of center.
      sFunc = cmdRequest.action;                // Select single scan or continuous sweep.
      break;
    case POINT:
      newDir = cmdRequest.value;                // Set newDir to value provided
      sFunc = cmdRequest.action;                // Turn head
      break;
    case GETFUNC:
      cmdResult.action = sFunc;
      return(cmdResult);                        // Return current value of sFunc.
      break;
    case TARGET:
      switch(cmdRequest.value){
        case CLOSEST:
          cmdResult.direction  = minDir;
          cmdResult.value = minDist;
          break;
        case FARTHEST:
          cmdResult.direction  = maxDir;
          cmdResult.value = maxDist;
          break;
        default:
          cmdResult.direction  = pingDir;
          cmdResult.value = pingDist;
          break;
      }
      return(cmdResult);                        // Return target results
      break;
    case SETINC:
      sweepInc = cmdRequest.value;              // Set sweep/scan increment value.
      break;
    default:
      sFunc = cmdRequest.action;                // Simple command, no special actions.
      break;
  }    
}

/* Return struct with distance to both edges */
obstacle sonarGetClearance(void){
  return(slope);
}  

