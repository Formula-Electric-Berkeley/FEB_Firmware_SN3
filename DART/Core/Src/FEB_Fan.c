// ********************************** Includes & External **********************************
#include "FEB_Fan.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;

extern UART_HandleTypeDef huart2;

extern uint16_t FEB_Current_Fan_Speed;

uint8_t FEB_CAN_Measured_Speed[8];

// ********************************** Testing **********************************

volatile uint8_t gu8_MSGGG[64] = {'\0'};
uint8_t counter = 0;

void FEB_Test_PWM(){
	//HAL_Delay(2000);
	counter = 50;
	//counter = counter % 125;
	FEB_Fan_All_Speed_Set(counter);
}

// ********************************** Initialize **********************************

void FEB_Fan_Init(){
	//FEB_Fan_PWM_Init();
	//FEB_Fan_All_Speed_Set(255 * 0);
}

// ********************************** Sensor Reading **************************

uint16_t FEB_Get_F1_Speed(){

	 sprintf(gu8_MSGGG, "1");
	 HAL_UART_Transmit(&huart2, gu8_MSGGG, sizeof(gu8_MSGGG), 100);

	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK){Error_Handler();}

	sprintf(gu8_MSGGG, "2");
	 HAL_UART_Transmit(&huart2, gu8_MSGGG, sizeof(gu8_MSGGG), 100);
	if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4) != HAL_OK){Error_Handler();}

	sprintf(gu8_MSGGG, "3");
	HAL_UART_Transmit(&huart2, gu8_MSGGG, sizeof(gu8_MSGGG), 100);

	FEB_Current_Fan_Speed = 0;
	HAL_Delay(50);
	uint8_t measured_speed = FEB_Current_Fan_Speed;

	if (HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_4) != HAL_OK){Error_Handler();}
	if (HAL_TIM_Base_Stop_IT(&htim3) != HAL_OK){Error_Handler();}

	return measured_speed;
}

uint16_t FEB_Get_F2_Speed(){

	sprintf(gu8_MSGGG, "Starting 2");
	HAL_UART_Transmit(&huart2, gu8_MSGGG, sizeof(gu8_MSGGG), 100);

	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK){Error_Handler();}

	if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3) != HAL_OK){Error_Handler();}

	FEB_Current_Fan_Speed = 0;
	HAL_Delay(50);
	uint8_t measured_speed = FEB_Current_Fan_Speed;

	if (HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_3) != HAL_OK){Error_Handler();}

	if (HAL_TIM_Base_Stop_IT(&htim3) != HAL_OK){Error_Handler();}

	return measured_speed;
}

uint16_t FEB_Get_F3_Speed(){

	sprintf(gu8_MSGGG, "Starting 3");
	HAL_UART_Transmit(&huart2, gu8_MSGGG, sizeof(gu8_MSGGG), 100);

	if (HAL_TIM_Base_Start_IT(&htim14) != HAL_OK){FEB_Error_Handler();}

	if (HAL_TIM_IC_Start_IT(&htim14, TIM_CHANNEL_1) != HAL_OK){FEB_Error_Handler();}

//	FEB_Current_Fan_Speed = 0;
	HAL_Delay(100);
	uint16_t measured_speed = 0;

	if (HAL_TIM_IC_Stop_IT(&htim14, TIM_CHANNEL_1) != HAL_OK){FEB_Error_Handler();}

	if (HAL_TIM_Base_Stop_IT(&htim14) != HAL_OK){FEB_Error_Handler();}

	return measured_speed;
}

uint16_t FEB_Get_F4_Speed(){

	sprintf(gu8_MSGGG, "Starting 4");
	HAL_UART_Transmit(&huart2, gu8_MSGGG, sizeof(gu8_MSGGG), 100);

	if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK){Error_Handler();}
	if (HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2) != HAL_OK){Error_Handler();}

	FEB_Current_Fan_Speed = 0;
	HAL_Delay(50);
	uint8_t measured_speed = FEB_Current_Fan_Speed;

	if (HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_2) != HAL_OK){Error_Handler();}
	if (HAL_TIM_Base_Stop_IT(&htim2) != HAL_OK){Error_Handler();}

	return measured_speed;
}

uint16_t FEB_Get_F5_Speed(){

	sprintf(gu8_MSGGG, "Starting 5");
	HAL_UART_Transmit(&huart2, gu8_MSGGG, sizeof(gu8_MSGGG), 100);

	if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK){FEB_Error_Handler();}
	if (HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK){FEB_Error_Handler();}

//	FEB_Current_Fan_Speed = 0;
	HAL_Delay(100);
	uint8_t measured_speed = 0;

	if (HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1) != HAL_OK){FEB_Error_Handler();}
	if (HAL_TIM_Base_Stop_IT(&htim2) != HAL_OK){FEB_Error_Handler();}

	return measured_speed;
}


// ********************************** CAN **********************************

void FEB_Update_Fan_Speed_Measurements(){
	FEB_CAN_Measured_Speed[0] = FEB_Get_F1_Speed();
	FEB_CAN_Measured_Speed[2] = FEB_Get_F2_Speed();
	FEB_CAN_Measured_Speed[1] = FEB_Get_F3_Speed();
	FEB_CAN_Measured_Speed[3] = FEB_Get_F4_Speed();
	FEB_CAN_Measured_Speed[4] = FEB_Get_F5_Speed();
}

void FEB_Fan_CAN_Process(uint8_t* FEB_CAN_Rx_Data){
	FEB_Fan_1_Speed_Set(FEB_CAN_Rx_Data[0]);
	FEB_Fan_2_Speed_Set(FEB_CAN_Rx_Data[1]);
	FEB_Fan_3_Speed_Set(FEB_CAN_Rx_Data[2]);
	FEB_Fan_4_Speed_Set(FEB_CAN_Rx_Data[3]);
	FEB_Fan_5_Speed_Set(FEB_CAN_Rx_Data[4]);
}

// ********************************** PWM **********************************
void FEB_Fan_PWM_Init(void) {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //fan 1
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //fan 2
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //fan 3
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); //fan 4
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //fan 5
}

void FEB_Fan_All_Speed_Set(uint8_t speed) {
	FEB_Fan_1_Speed_Set(speed);
	FEB_Fan_2_Speed_Set(speed);
	FEB_Fan_3_Speed_Set(speed);
	FEB_Fan_4_Speed_Set(speed);
	FEB_Fan_5_Speed_Set(speed);
}

void FEB_Fan_1_Speed_Set(uint8_t speed) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, speed);
}

void FEB_Fan_2_Speed_Set(uint8_t speed) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, speed);
}

void FEB_Fan_3_Speed_Set(uint8_t speed) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed);
}

void FEB_Fan_4_Speed_Set(uint8_t speed) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);
}

void FEB_Fan_5_Speed_Set(uint8_t speed) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
}
