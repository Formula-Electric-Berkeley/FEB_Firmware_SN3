//#ifndef INC_FEB__MAIN_H_
//#define INC_FEB__MAIN_H_


void FEB_Main_Setup(void);
void FEB_Main_Loop(void);

#include "FEB_CAN.h"
#include "FEB_CAN_APPS.h"
#include "FEB_CAN_ICS.h"
#include "FEB_Hardware.h"
#include "FEB_TPS2482.h"

#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdio.h>

#define SLEEP_TIME 10
