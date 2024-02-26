// ******************************** Includes ********************************

#include "FEB_Main.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {
	FEB_SM_Init();
	FEB_CAN_Init();
	FEB_CAN_Charger_Init();
}

void FEB_Main_Task1_VT(void) {
	while (1) {
		// Voltage
		FEB_LTC6811_Poll_Voltage();
		FEB_LTC6811_Validate_Voltage();

		// Temperature
		FEB_LTC6811_Poll_Temperature();
		FEB_LTC6811_Validate_Temperature();

		osDelay(100);
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
		FEB_CAN_Charger_Process();
		osDelay(1000);
	}
}

void FEB_Main_Task4_Balance(void) {
	while (1) {
		FEB_LTC6811_Balance_Process();
		osDelay(500);
	}
}

void FEB_Main_Task5_IVT(void) {
	while (1) {
		FEB_CAN_IVT_Process();
		osDelay(100);
	}
}

void FEB_Main_Task6_CAN(void) {
	while (1) {
		FEB_LTC6811_CAN_Transmit_Voltage();
		FEB_LTC6811_CAN_Transmit_Temperature();
		FEB_SM_CAN_Transmit();
		// TODO: CAN transmit LTC6811 balance, add bit to indicate currently balancing
		// TODO: CAN transmit LTC6811 enabled temperature sensors
		osDelay(200);
	}
}



