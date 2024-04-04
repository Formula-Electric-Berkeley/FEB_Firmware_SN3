#include <FEB_COOLANT_FLOW.h>

uint8_t ReadCoolantFlowFromADC() {
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
}

static void CAN_CoolantFlow_Transmit(void) {
    FEB_CAN_Tx_Header.DLC = 1;
    FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_COOLANT_FLOW; // Need ID
    FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
    FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
    FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

    uint16_t coolant_flow_value = ReadCoolantFlowFromADC();

	FEB_CAN_Tx_Data[0] = (uint8_t)(coolant_flow_value >> 8); // MSB
	FEB_CAN_Tx_Data[1] = (uint8_t)(coolant_flow_value & 0xFF); // LSB

	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}

