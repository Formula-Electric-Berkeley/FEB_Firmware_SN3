#include "FEB_main.h"
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim2;

char uart_tx_buffer[5];
uint32_t tach_val;


void FEB_Init(void){
	//FEB_CAN_Init();
	//FEB_ADC_Init();
	FEB_Fan_Init();
	//HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	//HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);

}

void FEB_Main_Loop(void){

	// fan 1
	FEB_Fan_1_Speed_Set(255 * 0.3);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);


	// fan 2
	FEB_Fan_All_Speed_Set(255 * 0);

	HAL_Delay(5000);

	FEB_Fan_2_Speed_Set(255 * 0.3);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(500);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);


	FEB_Fan_All_Speed_Set(255 * 0);

	HAL_Delay(5000);



	//HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	//HAL_Delay(800);
	//FEB_CAN_Transmit();
	//FEB_Update_PWM_All_Fans();
	//memset(uart_tx_buffer, 0, sizeof(uart_tx_buffer));
	//FEB_Test_PWM();
	//tach_val = 0x0;
	//sprintf(uart_tx_buffer, "%lu\n", tach_val);
	//HAL_UART_Transmit(&huart2, (uint8_t *)uart_tx_buffer, sizeof(uart_tx_buffer), HAL_MAX_DELAY);
//
//	tach_val = FEB_Read_Tachometer_Fan_2();
//	sprintf(uart_tx_buffer, "%lu\n", tach_val);
//	HAL_UART_Transmit(&huart2, (uint8_t *)uart_tx_buffer, sizeof(uart_tx_buffer), HAL_MAX_DELAY);
//
//	tach_val = FEB_Read_Tachometer_Fan_3();
//	sprintf(uart_tx_buffer, "%lu\n", tach_val);
//	HAL_UART_Transmit(&huart2, (uint8_t *)uart_tx_buffer, sizeof(uart_tx_buffer), HAL_MAX_DELAY);
//
//	tach_val = FEB_Read_Tachometer_Fan_4();
//	sprintf(uart_tx_buffer, "%lu\n", tach_val);
//	HAL_UART_Transmit(&huart2, (uint8_t *)uart_tx_buffer, sizeof(uart_tx_buffer), HAL_MAX_DELAY);
//
//	tach_val = FEB_Read_Tachometer_Fan_5();
//	sprintf(uart_tx_buffer, "%lu\n", tach_val);
//	HAL_UART_Transmit(&huart2, (uint8_t *)uart_tx_buffer, sizeof(uart_tx_buffer), HAL_MAX_DELAY);

//HAL_Delay(1000);

}
