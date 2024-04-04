// **************************************** Includes & External ****************************************

#include "FEB_CAN.h"

extern CAN_HandleTypeDef hcan1;

// **************************************** CAN Configuration ****************************************

CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
static CAN_RxHeaderTypeDef FEB_CAN_Rx_Header;

uint8_t FEB_CAN_Tx_Data[8];
uint8_t FEB_CAN_Rx_Data[8];

uint32_t FEB_CAN_Tx_Mailbox;

// **************************************** Functions ****************************************


void FEB_CAN_Init(void) { /* Initialize CAN */
	FEB_CAN_Filter_Config();
	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void FEB_CAN_Transmit(CAN_HandleTypeDef* hcan) { /* Transmit Messages */
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &FEB_CAN_Rx_Header, FEB_CAN_Rx_Data) == HAL_OK) {
		FEB_CAN_IVT_Store_Msg(&FEB_CAN_Rx_Header, FEB_CAN_Rx_Data);
		FEB_CAN_Charger_Store_Msg(&FEB_CAN_Rx_Header, FEB_CAN_Rx_Data);
		/* Store Thermocouples (SPI) */
		FEB_CAN_Thermocouple();
		/* Store IMU (I2C) */
		FEB_CAN_IMU();
		/* Store Wheel Speed (Wheel Speed Sensor) */
		FEB_CAN_Wheel_Speed();
		/* Coolant Flow () */
		FEB_CAN_Coolant();
		/* Strain Gauge () */
		FEB_CAN_Strain();
		/* Store Airspeed + Coolant Temp (ADC)  */
		FEB_CAN_Airspeed();
		FEB_CAN_Coolant();

	}
}
