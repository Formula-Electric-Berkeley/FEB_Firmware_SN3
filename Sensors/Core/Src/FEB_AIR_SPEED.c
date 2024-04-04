#include <FEB_AIR_SPEED.h>

uint16_t ReadAirspeedFromADC() {
    uint16_t airspeed_value = 0;
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    airspeed_value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return airspeed_value;
}


static void FEB_CAN_Airspeed_Transmit(void) {
    // Initialize transmission header
    FEB_CAN_Tx_Header.DLC = 2;
    FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_AIRSPEED;
    FEB_CAN_Tx_Header.IDE = CAN_ID_EXT; // Need ID
    FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
    FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

    // Read airspeed data from ADC
    uint16_t airspeed_value = ReadAirspeedFromADC();

    FEB_CAN_Tx_Data[0] = (uint8_t)(airspeed_value >> 8); // MSB
    FEB_CAN_Tx_Data[1] = (uint8_t)(airspeed_value & 0xFF); // LSB

    // Delay until mailbox available
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

    // Add Tx data to mailbox
   	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
   		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
   	}

}
