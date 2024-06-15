#include "FEB_Main.h"
#include "FEB_SM.h"
#include "FEB_LTC6811.h"
#include "FEB_CAN_Charger.h"
#include "FEB_CAN_IVT.h"
#include "stm32f4xx_hal.h"

// ******** Functions ********

void FEB_Main_Setup(void) {
	FEB_SM_Init();
}

void FEB_Main_Loop(void) {
	FEB_LTC6811_Poll_Voltage();
	FEB_LTC6811_Poll_Temperature();
//	FEB_LTC6811_UART_Transmit();
//	FEB_LTC6811_Cell_Data_CAN_Transmit();
//	FEB_LTC6811_Accumulator_Voltage_CAN_Transmit();
//	FEB_LTC6811_Accumulator_Temperature_CAN_Transmit();

//	FEB_CAN_Charger_Process();
//	FEB_CAN_Charger_UART_Transmit();

//	FEB_LTC6811_Balance_Process();

	FEB_SM_Process();
//	FEB_SM_UART_Transmit();
//	FEB_SM_CAN_Transmit();

	FEB_CAN_IVT_Process();
//	FEB_CAN_IVT_UART_Transmit();

	HAL_Delay(10);
}
