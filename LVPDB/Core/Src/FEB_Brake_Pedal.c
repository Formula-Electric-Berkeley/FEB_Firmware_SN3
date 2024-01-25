#include "FEB_Brake_Pedal.h"


void FEB_Brake_Light_On(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);// PA1 high
}

void FEB_Brake_Light_Off(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);// PA1 low
}
