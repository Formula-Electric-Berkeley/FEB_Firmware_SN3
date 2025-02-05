#include "FEB_Main.h"
#include "FEB_SM.h"
#include "FEB_LTC6811.h"
#include "FEB_CAN_Charger.h"
#include "FEB_CAN_IVT.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"

// ******** Functions ********

void FEB_Main_Setup(void) {
	FEB_SM_Init();
}

void FEB_Main_Task1_VT(void) {
	while (1) {
		FEB_LTC6811_Poll_Voltage();
		FEB_LTC6811_Poll_Temperature();
		FEB_LTC6811_UART_Transmit();
		FEB_LTC6811_Cell_Data_CAN_Transmit();
		FEB_LTC6811_Accumulator_Voltage_CAN_Transmit();
		FEB_LTC6811_Accumulator_Temperature_CAN_Transmit();
		osDelay(100);
	}
}

void FEB_Main_Task2_State(void) {
	while (1) {
		FEB_SM_Process();
		FEB_SM_UART_Transmit();
		FEB_SM_CAN_Transmit();
		osDelay(100);
	}
}

void FEB_Main_Task3_Charge(void) {
	while (1) {
		FEB_CAN_Charger_Process();
		FEB_CAN_Charger_UART_Transmit();
		osDelay(1000);
	}
}

void FEB_Main_Task4_Balance(void) {
	while (1) {
		FEB_LTC6811_Balance_Process();
		osDelay(1000);
	}
}

void FEB_Main_Task5_IVT(void) {
	while (1) {
		FEB_CAN_IVT_Process();
		FEB_CAN_IVT_UART_Transmit();
		osDelay(200);
	}
}

void FEB_Main_Task6_CAN(void) { // TODO: Remove Function
	while (1) {
		osDelay(1000);
	}
}

