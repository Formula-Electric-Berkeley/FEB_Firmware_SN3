#include <FEB_WHEEL_SPEED.h>

uint8_t ReadWheelSpeedSensor(void) {
    uint8_t sensor_value = 0;
    // Read cos and sin from the GPIO pins
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET) {
        sensor_value |= (1 << 0); // Set bit 0
    }
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET) {
        sensor_value |= (1 << 1); // Set bit 1
    }
    return sensor_value;
}


static void CAN_WheelSpeedSensor_Transmit(void) {
    FEB_CAN_Tx_Header.DLC = 1;
    FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_WHEEL_SPEED_SENSOR; // Need ID
    FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
    FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
    FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

    uint8_t sensor_value = ReadWheelSpeedSensor();


    FEB_CAN_Tx_Data[0] = sensor_value;

    // Delay until mailbox available
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

    // Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}
