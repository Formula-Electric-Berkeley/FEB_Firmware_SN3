#include <FEB_STRAIN_GAUGE.h>

uint8_t ReadStrainGaugeData(void) {
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
}

static void CAN_StrainGauge_Transmit(void) {
    FEB_CAN_Tx_Header.DLC = 1;
    FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_STRAIN_GAUGE; // Need ID
    FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
    FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
    FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

    uint8_t strain_gauge_data = ReadStrainGaugeData();

    FEB_CAN_Tx_Data[0] = strain_gauge_data;

    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

    // Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}
