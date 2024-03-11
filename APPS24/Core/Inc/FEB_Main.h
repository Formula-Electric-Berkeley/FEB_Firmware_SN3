#ifndef INC_FEB_MAIN_H_
#define INC_FEB_MAIN_H_

// ******************************** Includes ********************************

#include <stm32f4xx_hal.h>
#include <stdint.h>

#include "FEB_Timer.h"
#include "FEB_CAN.h"
#include "FEB_CAN_RMS.h"
#include "FEB_CAN_SW.h"
#include "FEB_Normalized.h"
#include "FEB_TPS2482.h"
#include "FEB_HECS.h"


// ******************************** Functions ********************************

void FEB_Main_Setup(void);
void FEB_Main_While(void);

#endif /* INC_FEB_MAIN_H_ */
