/*
 * FEB_Main.h
 *
 *  Created on: Jan 21, 2024
 *      Author: viploverahate
 */

#ifndef INC_FEB_MAIN_H_
#define INC_FEB_MAIN_H_

//************************* INCLUDES *****************
#include "stdio.h"
#include "stdbool.h"
#include "FEB_CAN.h"
#include "FEB_CAN_ID.h"

extern CAN_HandleTypeDef hcan1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;
extern UART_HandleTypeDef huart2;

#endif /* INC_FEB_MAIN_H_ */

//************************* FUNCTIONS ****************
void FEB_Main_User2(void);
void FEB_Main_While(void);
void FEB_Main_User4(void);