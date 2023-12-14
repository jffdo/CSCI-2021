#include "thermo.h"

// Uses the two global variables (ports) THERMO_SENSOR_PORT and
// THERMO_STATUS_PORT to set the fields of `temp`. If
// THERMO_SENSOR_PORT is negative or above its maximum trusted value
// (associated with +45.0 deg C), this function sets the
// tenths_degrees to 0 and the temp_mode to 3 for `temp` before
// returning 1.  Otherwise, converts the sensor value to deg C using
// shift operations.  Further converts to deg F if indicated from
// THERMO_STATUS_PORT. Sets the fields of `temp` to appropriate
// values. `temp_mode` is 1 for Celsius, and 2 for Fahrenheit. Returns
// 0 on success. This function DOES NOT modify any global variables
// but may access them.
//
// CONSTRAINTS: Uses only integer operations. No floating point
// operations are used as the target machine does not have a FPU. Does
// not use any math functions such as abs().
// int set_temp_from_ports(temp_t *temp){
//     if (THERMO_SENSOR_PORT < 0 || THERMO_SENSOR_PORT > 28800 || THERMO_STATUS_PORT & (1 << 2)){ // Check for Out of Range, if 2th bit is 1 for Status Err
//         temp->tenths_degrees = 0;
//         temp->temp_mode = 3;
//         return 1;
//     }
//     temp->temp_mode = 1;                        // Assume temp is Celcius
//     temp->tenths_degrees = THERMO_SENSOR_PORT >> 5;
//     if (THERMO_SENSOR_PORT & (1 << 4)){         // 4th bit includes 16, remainer is rounded up
//         temp->tenths_degrees += 1;
//     }
//     temp->tenths_degrees -= 450;          
//     if (THERMO_STATUS_PORT & (1 << 5)){         // If 5th bit is 1, then thermo is F
//         temp->temp_mode = 2;
//         temp->tenths_degrees = (temp->tenths_degrees*9)/5 + 320;
//     }
//     return 0;
// }

// Alters the bits of integer pointed to by display to reflect the
// temperature in struct arg temp.  If temp has a temperature value
// that is below minimum or above maximum temperature allowable or if
// the temp_mode is not Celsius or Fahrenheit, sets the display to
// read "ERR" and returns 1. Otherwise, calculates each digit of the
// temperature and changes bits at display to show the temperature
// according to the pattern for each digit.  This function DOES NOT
// modify any global variables except if `display` points at one.
// 
// CONSTRAINTS: Uses only integer operations. No floating point
// operations are used as the target machine does not have a FPU. Does
// not use any math functions such as abs().
// int set_display_from_temp(temp_t temp, int *display){
//     *display = 0;
//     if (temp.temp_mode != 1 && temp.temp_mode != 2){ // Check Temp mode for 1,2
//         *display = *display |  (0b011011110111111011111 << 7);
//         return 1;
//     }
//     if (temp.temp_mode == 1){ // Check for Celsius
//         if (temp.tenths_degrees < -450 || temp.tenths_degrees > 450){ // Check for Out of Range
//             *display = *display |  (0b011011110111111011111 << 7);
//             return 1;
//         }
//         *display = *display | (1 << 28);
//     } else {                  // Otherwise Fahrenhiet
//         if (temp.tenths_degrees < -490 || temp.tenths_degrees > 1130){ // Check for Out of Range
//             *display = *display |  (0b011011110111111011111 << 7);
//             return 1;
//         }
//         *display = *display | (1 << 29);
//     }
//     int digit[10] = {0b1111011, 0b1001000, 0b0111101, 0b1101101, 0b1001110,
//                      0b1100111, 0b1110111, 0b1001001, 0b1111111, 0b1101111};
//     int abs_temp = temp.tenths_degrees;
//     if(~(temp.tenths_degrees >> 31)+1){ // Checks if temp is negative
//         abs_temp = ~(temp.tenths_degrees)+1; // 2s complement conversion
//     }
//     int temp_hundreds = (abs_temp / 1000) % 10;
//     int temp_tens = (abs_temp / 100) % 10;
//     int temp_ones = (abs_temp / 10) % 10;
//     int temp_tenths = abs_temp % 10;
//     int negative = (0b0000100 << 21);
//     if(temp_hundreds != 0){    // Check for leading 0s
//         *display = *display | (digit[temp_hundreds] << 21);
//     }
//     if(temp_hundreds != 0 || temp_tens != 0){ // Check for 2 leading 0s
//         *display = *display | (digit[temp_tens] << 14);
//     } else{
//         negative >>= 7; // Otherwise blank space is used for the negative
//     }
//     *display = *display | (digit[temp_ones] << 7);
//     *display = *display | digit[temp_tenths];
//     if (temp.tenths_degrees < 0){ // Apply negative if necessary 
//         *display = *display | negative;
//     }
//     return 0;
// }

// Called to update the thermometer display.  Makes use of
// set_temp_from_ports() and set_display_from_temp() to access
// temperature sensor then set the display. Always sets the display
// even if the other functions returns an error. Returns 0 if both
// functions complete successfully and return 0. Returns 1 if either
// function or both return a non-zero values.
// 
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only.
// int thermo_update(){
//     temp_t temp;
//     if(set_temp_from_ports(&temp) + set_display_from_temp(temp, &THERMO_DISPLAY_PORT)){
//         return 1;
//     }
//     return 0;
// }
