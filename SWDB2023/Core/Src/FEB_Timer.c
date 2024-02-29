// ********************************** Includes & External **********************************

#include "FEB_Timer.h"

extern TIM_HandleTypeDef htim2; //timer for buzzer
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;

// ********************************** Initialize **********************************

// ********************************** Functions **********************************
void FEB_Timer_Init(){
	HAL_TIM_Base_Start_IT(&htim13);
	HAL_TIM_Base_Stop_IT(&htim13);

	HAL_TIM_Base_Start_IT(&htim14);
	HAL_TIM_Base_Stop_IT(&htim14);

	//0% PWM cycle to turnoff buzzer
	htim2.Instance->CCR2 = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

}

void FEB_Timer_start1sec(){
	HAL_TIM_Base_Start_IT(&htim13);
}

void FEB_Timer_stop1sec(){
	HAL_TIM_Base_Stop_IT(&htim13);
}

void FEB_Timer_startBuzzer(){
	htim2.Instance->CCR2 = VOLUME;
}

bool FEB_Timer_equals13(TIM_HandleTypeDef *htim){
	if (htim == &htim13){
		return true;
	}
	return false;
}

bool FEB_Timer_equals14(TIM_HandleTypeDef *htim){
	if (htim == &htim14){
		return true;
	}
	return false;
}

void FEB_Timer_stopBuzzer(){
	htim2.Instance->CCR2 = 0;
	HAL_TIM_Base_Stop_IT(&htim14);
}
