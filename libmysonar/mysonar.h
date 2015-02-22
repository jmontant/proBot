/*
 *  MySonar - Ultrasonic Ping))) sensor handler
 *
 *  written by Paul Bammel April 2014
 *
 */

#define PING_PRESENT                        // Identify Ping))) sonar as being present.

// General definitions
#define CURRENT     1
#define CLOSEST     2
#define FARTHEST    3
#define SINGLE      4
#define CONTINUOUS  5

struct target {
  int dist;                                 // Distance to selected target.
  int dir;                                  // Direction to selected target.
};  

int   initSonarControl(void);              // Start sonar_control in separate cog.
void  sonarPointAt(int angle);              // Turn ping sensor to face a given direction.
void  sonarScan(int range=90);              // Scan a particular range either side of straight ahead.
void  sonarPing(void);                      // Repeatedly ping whatever direction head is facing.
int   sonarPingNow(void);                   // Perform a single ping and update pingDist variable.
void  sonarSetInc(int inc);                 // Set sweep/scan increment value.
target  sonarGetTarget(int type);           // Return Selected Dist & Dir values.
void  sonarStop(void);                      // Stop any sonar activity at next interval.
int   sonarGetFunction(void);               // Return current sonar function being performed.
int   sonarFindTarget(int type);            // Return angle to target center (pos=right, neg=left)
