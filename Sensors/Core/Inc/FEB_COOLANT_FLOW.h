#ifndef INC_FEB_COOLANT_FLOW_H_
#define INC_FEB_COOLANT_FLOW_H_

#include "stm32f4xx_hal.h"

uint16_t ReadCoolantFlowFromADC();
static void CAN_CoolantFlow_Transmit(void);

#endif /* INC_FEB_FEB_COOLANT_FLOW_H_ */
