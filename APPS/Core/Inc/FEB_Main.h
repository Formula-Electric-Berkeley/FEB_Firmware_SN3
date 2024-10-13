#ifndef INC_FEB_MAIN_H_
#define INC_FEB_MAIN_H_
//************************* INCLUDES *****************

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "main.h"

#include "FEB_Store.h"
#include "FEB_RMS.h"
#include "FEB_NORMALIZED.h"

extern CAN_HandleTypeDef hcan1;
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim5;
extern SW_MESSAGE_TYPE SW_MESSAGE;
extern RMS_MESSAGE_TYPE RMS_MESSAGE;
extern RMSControlType RMSControl;


//************************* FUNCTIONS ****************
void FEB_APPS_sendBrake();
void FEB_Main_User2(void);
void FEB_Main_User3(void);



#endif /* INC_FEB_MAIN_H_ */
