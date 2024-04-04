#include <FEB_THERMOCOUPLE.h>


uint16_t ReadThermocoupleData(uint8_t thermocouple_number) {
    uint16_t thermocouple_data = 0;
    // Need to determine actual CS pins
    if (thermocouple_number == 1) {
        HAL_GPIO_WritePin(TC1_CS_GPIO_Port, TC1_CS_Pin, GPIO_PIN_RESET);
    } else if (thermocouple_number == 2) {
        HAL_GPIO_WritePin(TC2_CS_GPIO_Port, TC2_CS_Pin, GPIO_PIN_RESET);
    }

    // Check function
    HAL_SPI_TransmitReceive(&hspi, tx_buffer, rx_buffer, buffer_size, timeout);
    thermocouple_data = (rx_buffer[0] << 8) | rx_buffer[1]; // Assuming 16-bit data

    if (thermocouple_number == 1) {
        HAL_GPIO_WritePin(TC1_CS_GPIO_Port, TC1_CS_Pin, GPIO_PIN_SET);
    } else if (thermocouple_number == 2) {
        HAL_GPIO_WritePin(TC2_CS_GPIO_Port, TC2_CS_Pin, GPIO_PIN_SET);
    }

    return thermocouple_data;
}

static void CAN_Thermocouple1_Transmit(void) {
    FEB_CAN_Tx_Header.DLC = 2;
    FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_THERMOCOUPLE_1; // Need ID
    FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
    FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
    FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

    uint16_t thermocouple1_data = ReadThermocoupleData(1);

    FEB_CAN_Tx_Data[0] = (uint8_t)(thermocouple1_data >> 8);
    FEB_CAN_Tx_Data[1] = (uint8_t)(thermocouple1_data & 0xFF);

    // Delay until mailbox available
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

    // Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}


static void CAN_Thermocouple2_Transmit(void) {
    FEB_CAN_Tx_Header.DLC = 2;
    FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_THERMOCOUPLE_1; // Need ID
    FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
    FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
    FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

    uint16_t thermocouple1_data = ReadThermocoupleData(2);

    FEB_CAN_Tx_Data[0] = (uint8_t)(thermocouple1_data >> 8);
    FEB_CAN_Tx_Data[1] = (uint8_t)(thermocouple1_data & 0xFF);

    // Delay until mailbox available
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

    // Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}
