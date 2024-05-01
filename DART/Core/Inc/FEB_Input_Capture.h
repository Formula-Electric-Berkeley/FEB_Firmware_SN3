#ifndef INC_FEB_INPUT_CAPTURE_H_
#define INC_FEB_INPUT_CAPTURE_H_

#include "stm32f0xx_hal.h"

void FEB_IC_Process(TIM_HandleTypeDef* htim);
void FEB_Period_Elapsed();

#endif /* INC_FEB_INPUT_CAPTURE_H_ */
