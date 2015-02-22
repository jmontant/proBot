/**
 * @file mycompass.h
 *
 * @author Paul Bammel
 *
 * @version v2.00
 *
 * @brief This library provides convenience 
 * functions for reading measurements from the Parallax Compass Module
 * 3-Axis HMC5883L.
 */

#ifndef COMPASS3D_H
#define COMPASS3D_H

#if defined(__cplusplus)
extern "C" {
#endif


#define MODE_SNGL	  0x01
#define	 MODE_CONT  0x00

#define NORTH 0
#define EAST  90
#define SOUTH 180
#define WEST  270

/**
 * @brief Initialize the Compass
 *
 * @details This function will initialize an I2C bus
 * to communicate with the compass module and then
 * initalize the compass module itself into the read
 * mode (single/continuous)specified by the caller.  
 *
 * @returns void, but it will display an error message if the
 * compass module does not respond. 
 */
void compass_init(unsigned char cMode);

/**
 * @brief Read values from compass.
 *
 * @details This function reads the raw x, y, and z
 * values and loads them into variables that are
 * passed by address.
 *
 * @param *px A pointer to a variable to receive the 
 * x-value measurement.
 *
 * @param *py A pointer to a variable to receive the 
 * y-value measurement.
 *
 * @param *pz A pointer to a variable to receive the 
 * z-value measurement.
 *
 * @returns void, but it will display an error message if the
 * compass module does not respond. 
 */
void compass_read(int *px, int *py, int *pz);     // Raw compass value reading.

void compass_setCal(int calx, int caly);          // Force calibration values.

int compass_smplHeading();                        // Return current adjusted heading 

int compass_diff(int curHead, int desHead);       // Return angle delta between current & destination

int compass_calibrate(void);                      // Determine compass calibration values.

#if defined(__cplusplus)
}
#endif
/* __cplusplus */  
#endif
/* COMPASS3D_H */  

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


