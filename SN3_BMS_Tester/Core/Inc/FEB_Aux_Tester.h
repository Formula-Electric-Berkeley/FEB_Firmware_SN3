#ifndef INC_FEB_AUX_TESTER_H_
#define INC_FEB_AUX_TESTER_H_

// ******************************** Includes ********************************

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "FEB_Input_Voltages.h"
#include "FEB_Validate_Readings.h"
#include "FEB_LTC6811.h"
#include "FEB_Constant.h"
#include "FEB_BMS_Tester_Hardware.h"
#include "stm32f4xx_hal.h"

// ******************************** Functions ********************************

void FEB_Aux_Tester_Test_Aux(void);
void FEB_Aux_Tester_Init(void);
void FEB_Aux_Tester_Test_Cell_Voltages(void);
void FEB_Aux_Tester_Test_Cell_Temps(void);

#endif /* INC_FEB_AUX_TESTER_H_ */
