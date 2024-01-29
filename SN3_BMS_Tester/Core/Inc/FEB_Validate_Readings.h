/*
 * FEB_Validate_Readings.h
 *
 *  Created on: Jan 12, 2024
 *      Author: andyyang
 */

#ifndef INC_FEB_VALIDATE_READINGS_H_
#define INC_FEB_VALIDATE_READINGS_H_

// ********************************** Includes **********************************

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "bms_hardware.h"
#include "FEB_LTC6811.h"
#include "FEB_Constant.h"


// ********************************** Functions **********************************

void FEB_Validate_Readings_Validate_Voltages(uint8_t ic, float expected_voltage, uint8_t curr_cell);
void FEB_Validate_Readings_Validate_Temperatures(uint8_t ic, float expected_voltage, uint8_t cell);
void FEB_Validate_Readings_Validate_Cell(uint8_t ic, uint8_t cell, float expected_voltage, float polled_voltage, float threshold_high, float threshold_low);

#endif /* INC_FEB_VALIDATE_READINGS_H_ */
