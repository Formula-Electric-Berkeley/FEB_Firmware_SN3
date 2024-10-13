/*
 * FEB_Normalized.h
 *
 *  Created on: Nov 5, 2023
 *      Author: viploverahate
 */

#ifndef INC_FEB_NORMALIZED_H_
#define INC_FEB_NORMALIZED_H_

//************************* INCLUDES *****************
//#include "FEB_CAN.h"
//#include "FEB_Main.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"



extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;


extern float normalized_brake;
extern float normalized_acc;


//************************* FUNCTIONS ****************
float FEB_Normalized_Acc_Pedals();
float FEB_Normalized_Brake_Pedals();


#endif /* INC_FEB_NORMALIZED_H_ */
