#include "FEB_CAN.h"
#include "FEB_CAN_Charger.h"
#include "FEB_CAN_IVT.h"
#include "stm32f4xx_hal.h"

extern CAN_HandleTypeDef hcan1;

CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
static CAN_RxHeaderTypeDef FEB_CAN_Rx_Header;

uint8_t FEB_CAN_Tx_Data[8];
uint8_t FEB_CAN_Rx_Data[8];

uint32_t FEB_CAN_Tx_Mailbox;

// ******** CAN Functions ********

static void CAN_filter_config(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

/* Returns success status of CAN initialization. */
bool FEB_CAN_Init(void) {
	CAN_filter_config();
	if (HAL_CAN_Start(&hcan1) != HAL_OK ||
		HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING != HAL_OK))
		return false;
	return true;
}

static void CAN_filter_config(void) {
	uint8_t filter_bank = 0;
//	filter_bank = FEB_CAN_Charger_Filter_Config(&hcan1, CAN_RX_FIFO0, filter_bank);
	filter_bank = FEB_CAN_IVT_Filter_Config(&hcan1, CAN_RX_FIFO0, filter_bank);
}

extern UART_HandleTypeDef huart2;
#include "string.h"
#include "stdio.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	char UART_Str[64];
	sprintf(UART_Str, "Recv\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) UART_Str, strlen(UART_Str), 100);

	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &FEB_CAN_Rx_Header, FEB_CAN_Rx_Data) == HAL_OK) {
		FEB_CAN_IVT_Store_Msg(&FEB_CAN_Rx_Header, FEB_CAN_Rx_Data);
//		FEB_CAN_Charger_Store_Msg(&FEB_CAN_Rx_Header, FEB_CAN_Rx_Data);
	}
}
