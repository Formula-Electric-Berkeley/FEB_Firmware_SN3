/*
 * FEB_BMS_Tester_Hardware.h
 *
 *  Created on: Jan 21, 2024
 *      Author: andyyang
 */

#ifndef INC_FEB_BMS_TESTER_HARDWARE_H_
#define INC_FEB_BMS_TESTER_HARDWARE_H_

// ********************************** Includes & External **********************************

#include "stdint.h"
#include <stdbool.h>
#include "stm32f4xx_hal.h"

// ********************************** Functions **********************************

void FEB_BMS_Tester_Hardware_Configure_MUX(uint8_t cell);
void FEB_BMS_Tester_Hardware_Set_DAC_CS_n(uint8_t cell, uint8_t status);
bool FEB_BMS_Tester_Hardware_Read_Start();

#endif /* INC_FEB_BMS_TESTER_HARDWARE_H_ */
