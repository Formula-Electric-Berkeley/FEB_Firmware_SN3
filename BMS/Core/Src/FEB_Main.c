// ******************************** Includes ********************************

#include "FEB_Main.h"


// TODO: REMOVE
#include <string.h>
#include <stdio.h>
extern UART_HandleTypeDef huart2;

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {
	FEB_SM_Init();
	FEB_CAN_Init();
//	FEB_CAN_Charger_Init();
 	FEB_LTC6811_Init();
}

void FEB_Main_Task1_VT(void) {
	while (1) {
//		char UART_Str[64];
//		sprintf(UART_Str, "Temperature reading\n");
//		HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);

		// Voltage
//		FEB_LTC6811_Poll_Voltage();
//		FEB_LTC6811_Validate_Voltage();

		// Temperature
//		FEB_LTC6811_Poll_Temperature();
//		FEB_LTC6811_Validate_Temperature();

		osDelay(3000);
	}
}

void FEB_Main_Task2_State(void) {
	while (1) {
		// TODO: REMOVE TASK / DEBUG Task (UART)
		osDelay(100);
	}
}

void FEB_Main_Task3_Charge(void) {
	while (1) {
//		FEB_CAN_Charger_Process();
		osDelay(100);
	}
}

void FEB_Main_Task4_Balance(void) {
	while (1) {
//		FEB_LTC6811_Balance_Process();
		osDelay(500);
	}
}

void FEB_Main_Task5_IVT(void) {
	while (1) {
//		char UART_Str[64];
//		sprintf(UART_Str, "Debug 1\n");
//		HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);
//		FEB_Relay_Buzzer(FEB_RELAY_ST_CLOSE);

		FEB_CAN_IVT_Process();
		osDelay(1000);
	}
}

void FEB_Main_Task6_CAN(void) {
	while (1) {
//		char UART_Str[64];
//		sprintf(UART_Str, "CAN Monitor\n");
//		HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);

//		FEB_LTC6811_CAN_Transmit_Voltage();
//		FEB_LTC6811_CAN_Transmit_Temperature();
//		FEB_SM_CAN_Transmit();
		// TODO: CAN transmit LTC6811 balance, add bit to indicate currently balancing
		// TODO: CAN transmit LTC6811 enabled temperature sensors
		osDelay(200);
	}
}



