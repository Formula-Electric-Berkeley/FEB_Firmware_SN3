/*
 * FEB_Input_Voltage.h
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */

#ifndef INC_FEB_INPUT_VOLTAGES_H_
#define INC_FEB_INPUT_VOLTAGES_H_

// ********************************** Includes **********************************

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "bms_hardware.h"

// ********************************** Functions **********************************

void FEB_Input_Voltages_Input_Cell_Voltage(uint8_t cell, float voltage);
void FEB_Input_Voltages_Input_Temp_Voltage(uint8_t voltage);
void FEB_Aux_Tester_Set_Potentiometer_Resistance(uint8_t resistance);

#endif /* INC_FEB_INPUT_VOLTAGES_H_ */
