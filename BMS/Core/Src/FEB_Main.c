// ******************************** Includes ********************************

#include "FEB_Main.h"

// TODO: REMOVE TEST CODE
#include "stm32f4xx_hal.h"
#include <string.h>
extern UART_HandleTypeDef huart2;
// TODO: REMOVE TEST CODE

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {
	// Core
	FEB_Relay_Init();
	FEB_State_Init();

	// Other
//	FEB_CAN_Init();
	FEB_LTC6811_Init();
}

void FEB_Main_Loop(void) { // TODO: Remove
	// State-Relay
	FEB_State_Process();
//	FEB_State_UART_Transmit();
//	FEB_Relay_UART_Transmit();

	// Voltage
	FEB_LTC6811_Poll_Voltage();
	FEB_LTC6811_Validate_Voltage();
//	FEB_LTC6811_UART_Transmit_Voltage();

	// Temperature
	FEB_LTC6811_Poll_Temperature();
	FEB_LTC6811_Validate_Temperature();
//	FEB_LTC6811_UART_Transmit_Temperature();
	FEB_LTC6811_CAN_Transmit_Temperature();

	// IVT
	FEB_CAN_IVT_Process();
//	FEB_CAN_IVT_UART_Transmit();

	// TODO: Charger
	// FEB_CAN_Charger_Process();

	// TODO: Balance

	// TODO: Fans
}

void FEB_Main_Task1_VT(void) {
	uint8_t counter = 0;
	while (1) {
		char str[32];
		sprintf(str, "Poll voltage: %d\n", counter);
		HAL_UART_Transmit(&huart2, (uint8_t *) str, strlen(str), 100);
		counter++;

		// Voltage
		FEB_LTC6811_Poll_Voltage();
//		FEB_LTC6811_Validate_Voltage();
//		osDelay(100);

		// Temperature
		FEB_LTC6811_Poll_Temperature();
//		FEB_LTC6811_Validate_Temperature();
		osDelay(100);
	}
}

void FEB_Main_Task2_State(void) {
	while (1) {
//		FEB_State_Process();
		osDelay(100);
	}
}

void FEB_Main_Task3_Charge(void) {
	while (1) {
		// TODO: FEB_CAN_Charger_Process();
		osDelay(1000);
	}
}

void FEB_Main_Task4_Balance(void) {
	while (1) {
		// TODO: FEB_LTC6811_Balance();
		osDelay(500);
	}
}

void FEB_Main_Task5_IVT(void) {
	while (1) {
//		FEB_CAN_IVT_Process();
		osDelay(100);
	}
}

void FEB_Main_Task6_CAN(void) {
	while (1) {
		// TODO: FEB_LTC6811_CAN_Transmit_Voltage();
//		FEB_LTC6811_CAN_Transmit_Temperature();
		// TODO: FEB_LTC6811_CAN_Transmit_Balance();
		// TODO: FEB_State_CAN_Transmit();
		// TODO: FEB_Relay_CAN_Transmit();

		// TODO: REMOVE TEST CODE

		// TODO: REMOVE TEST CODE

		osDelay(200);
	}
}



