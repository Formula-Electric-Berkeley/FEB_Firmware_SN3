#include <FEB_COOLANT_TEMP.h>

uint16_t ReadCoolantTempFromADC() {
    uint16_t coolant_temp_value = 0;
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    coolant_temp_value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return coolant_temp_value;
}



static void CAN_CoolantTemp_Transmit(void) {
    // Initialize transmission header
    FEB_CAN_Tx_Header.DLC = 2;
    FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_COOLANT_TEMP; // NEED ID
    FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
    FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
    FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

    uint16_t coolant_temp_value = ReadCoolantTempFromADC();

    FEB_CAN_Tx_Data[0] = (uint8_t)(coolant_temp_value >> 8); // MSB
    FEB_CAN_Tx_Data[1] = (uint8_t)(coolant_temp_value & 0xFF); // LSB

    // Delay until mailbox available
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

    // Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}
