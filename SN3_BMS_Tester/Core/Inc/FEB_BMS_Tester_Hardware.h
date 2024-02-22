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
#include <stdio.h>
#include <string.h>
#include "FEB_Constant.h"

// ********************************** Functions **********************************

void FEB_BMS_Tester_Hardware_Configure_MUX(uint8_t cell);
void FEB_BMS_Tester_Hardware_Set_DAC_CS_n(uint8_t cell, bool status);
bool FEB_BMS_Tester_Hardware_Read_Start();
bool FEB_BMS_Tester_Hardware_Read_Mode();
void FEB_BMS_Tester_Hardware_Transmit_Start_Testing(char *board, char *mode);
void FEB_BMS_Tester_Hardware_Transmit_Done_Testing(char *board, char *mode);


typedef struct {
	GPIO_TypeDef* pin_type;
	uint16_t pin_num;
} cs_pins_t;
//define struct for all CS pins

#endif /* INC_FEB_BMS_TESTER_HARDWARE_H_ */
