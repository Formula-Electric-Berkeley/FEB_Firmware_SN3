// ********************************** Includes & External **********************************

#include "FEB_BMS_AIR.h"

extern CAN_HandleTypeDef hcan1;
extern UART_HandleTypeDef huart2;

// ********************************** Functions **********************************

void FEB_BMS_AIR_Open(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
}

void FEB_BMS_AIR_Close(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
}
