#ifndef INC_FEB_COOLANT_TEMP_H_
#define INC_FEB_COOLANT_TEMP_H_

#include "stm32f4xx_hal.h"

uint16_t ReadCoolantTempFromADC();
static void CAN_CoolantTemp_Transmit(void);

#endif /* INC_FEB_FEB_COOLANT_TEMP_H_ */
