#ifndef INC_FEB_DAUGHTER_TESTER_H_
#define INC_FEB_DAUGHTER_TESTER_H_

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

void FEB_Daughter_Tester_Test_Daughter(void);
void FEB_Daughter_Tester_Test_Daughter_Voltages(void);
void FEB_Daughter_Tester_Test_Daughter_Voltage(void);

#endif /* INC_FEB_DAUGHTER_TESTER_H_ */
