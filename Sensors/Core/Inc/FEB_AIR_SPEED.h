#ifndef INC_FEB_AIR_SPEED_H_
#define INC_FEB_AIR_SPEED_H_

#include "stm32f4xx_hal.h"

uint16_t ReadAirspeedFromADC();
void FEB_CAN_Airspeed_Transmit(IMU_HandleTypeDef *hdev);

#endif /* INC_FEB_AIR_SPEED_H_ */
