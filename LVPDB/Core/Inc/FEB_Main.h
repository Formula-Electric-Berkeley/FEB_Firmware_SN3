//#ifndef INC_FEB__MAIN_H_
//#define INC_FEB__MAIN_H_


void FEB_Main_Setup(void);
void FEB_Main_Loop(void);

#include "FEB_CAN.h"
#include "FEB_CAN_APPS.h"
#include "FEB_CAN_SW.h"
#include "stm32f4xx_hal.h"

#include "FEB_TPS2482.h"
#include "stdint.h"

#define SLEEP_TIME 10
#define BRAKE_THRE 0.2
