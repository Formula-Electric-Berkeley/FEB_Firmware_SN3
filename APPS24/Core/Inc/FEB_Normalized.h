#ifndef INC_FEB_NORMALIZED_H_
#define INC_FEB_NORMALIZED_H_

// **************************************** Includes ****************************************


#include "FEB_Const.h"
#include "FEB_CAN_ID.h"
#include "FEB_Const.h"

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// **************************************** Functions ****************************************


float FEB_Normalized_getAcc();
void FEB_Normalized_setAcc0();
void FEB_Normalized_updateAcc();
float FEB_Normalized_Acc_Pedals();
float FEB_Normalized_getBrake();
float FEB_Normalized_Brake_Pedals();
void FEB_Normalized_CAN_sendBrake();
void FEB_Normalized_Test_Acc();

#endif /* INC_FEB_NORMALIZED_H_ */