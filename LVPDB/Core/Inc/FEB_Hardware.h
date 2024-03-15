#ifndef INC_FEB_HARDWARE_H_
#define INC_FEB_HARDWARE_H_

#include "FEB_CAN_APPS.h"
#include "FEB_CAN_SW.h"

#include <stm32f4xx_hal.h>
#include <stdbool.h>

//void Enable_Coolant_Pump();
//void Disable_Coolant_Pump();
//void Enable_Accumulator_Fans();
//void Disable_Accumulator_Fans();
//void Enable_Extra();
//void Disable_Extra();
void FEB_Brake_Light_Control();
void FEB_Peripherals_Control();

#define BRAKE_THRE 0.2

#endif /* INC_FEB_HARDWARE_H_ */
