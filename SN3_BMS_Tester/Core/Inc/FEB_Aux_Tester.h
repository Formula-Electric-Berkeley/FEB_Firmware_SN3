#ifndef INC_FEB_AUX_TESTER_H_
#define INC_FEB_AUX_TESTER_H_

// ******************************** Includes ********************************

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"

// ******************************** Functions ********************************

void FEB_Aux_Tester_Test_Aux(void);
void FEB_Aux_Tester_Init(void);
void FEB_Aux_Tester_Test_Cell_Voltages(void);
void FEB_Aux_Tester_Test_Cell_Voltage(void);
void FEB_Aux_Tester_Test_Cell_Temps(void);
void FEB_Aux_Tester_Test_Cell_Temp(void);
void FEB_Aux_Tester_Read_Outputs(void);
void FEB_Aux_Tester_Validate_Voltage_Reading(void);
void FEB_Aux_Tester_Set_Potentiometer_Resistance(uint8_t resistance);
void FEB_Aux_Tester_Validate_Temp_Reading(void);
void FEB_Aux_Tester_selectMuxChannel(uint8_t channel);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

#endif /* INC_FEB_AUX_TESTER_H_ */
