/*
 * FEB_RMS.h
 *
 *  Created on: Nov 12, 2023
 *      Author: viploverahate
 */

#ifndef INC_FEB_RMS_H_
#define INC_FEB_RMS_H_

//************************* INCLUDES *****************
//#include "FEB_CAN.h"
//#include "FEB_Main.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "FEB_CAN.h"


extern CAN_HandleTypeDef hcan1;

typedef struct RMSControlType{
  uint16_t torque;
  uint8_t enabled;
} RMSControlType;

extern RMSControlType RMSControl;

//************************* FUNCTIONS ****************
void FEB_RMS_updateTorque();
void FEB_RMS_setTorque(uint16_t torque);
void FEB_RMS_enable();
void FEB_RMS_disable();
void FEB_RMS_Init();
int16_t FEB_min(int16_t x1, int16_t x2);
uint16_t FEB_getMaxTorque(int16_t voltage, int16_t RPM);

#endif /* INC_FEB_RMS_H_ */
