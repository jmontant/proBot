/*
 *  ParserTbl.h - English sentence parser table structure and definitions
 *  
 */

struct tblParser {
  int token;                    // Token we are looking for
  int match;                    // Next array index if token matches
  int noMatch;                  // Next array index if no match
};  

struct tblParser tblMove[]    = /* 0 */ {WS2_FORWARD,   2, 1,
                                /* 1 */  WS2_BACKWARD,  2, 6,
                                /* 2 */  ANY_NUM,       3, 6,
                                /* 3 */  WG2_STEPS,     7, 4,
                                /* 4 */  WG2_INCHES,    7, 5,
                                /* 5 */  WG2_FEET,      7, 6,
                                /* 6 */  TIMEOUT,       7, 8,
                                /* 7 */  EXECUTE,       7, 7,
                                /* 8 */  BAD_CMD,       8, 8};

struct tblParser tblTurn[]    = /* 0 */ {WS2_LEFT,      3, 1,
                                /* 1 */  WS2_RIGHT,     3, 2,
                                /* 2 */  WS2_BACKWARD,  3, 6,
                                /* 3 */  ANY_DEG,       4, 6,
                                /* 4 */  WG2_DEGREES,   7, 6,
                                /* 5 */  0,             8, 8,
                                /* 6 */  TIMEOUT,       7, 8,
                                /* 7 */  EXECUTE,       7, 7,
                                /* 8 */  BAD_CMD,       8, 8};

struct tblParser tblLook[]    = /* 0 */ {WS2_FORWARD,   6, 1,
                                /* 1 */  WS2_LEFT,      3, 2,
                                /* 2 */  WS2_RIGHT,     3, 6,
                                /* 3 */  ANY_DEG,       4, 6,
                                /* 4 */  WG2_DEGREES,   7, 6,
                                /* 5 */  0,             8, 8,
                                /* 6 */  TIMEOUT,       7, 8,
                                /* 7 */  EXECUTE,       7, 7,
                                /* 8 */  BAD_CMD,       8, 8};

struct tblParser tblFace[]    = {0,0,0};
struct tblParser tblReport[]  = {0,0,0};
struct tblParser tblScan[]    = {0,0,0};

