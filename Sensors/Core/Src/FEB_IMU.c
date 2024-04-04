#include <FEB_IMU.h>

uint8_t imu_data[6]; // Byte array to store IMU data



void IMU_ReadData(uint8_t reg_addr, uint8_t *data_buffer, uint16_t size) {
	HAL_I2C_Mem_Read(&hi2c1, IMU_DEVICE_ADDRESS << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data_buffer, size, HAL_MAX_DELAY);
}

static void CAN_IMU_Transmit(void) {

    FEB_CAN_Tx_Header.DLC = IMU_DATA_SIZE;
    FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_IMU; // Need to define ID
    FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
    FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
    FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

    // Read data
    uint8_t imu_data[IMU_DATA_SIZE];
    IMU_ReadData(IMU_REG_ADDR, imu_data, IMU_DATA_SIZE);

    // Copy data to buffer
    for (int i = 0; i < IMU_DATA_SIZE; i++) {
        FEB_CAN_Tx_Data[i] = imu_data[i];
    }

    // Delay until mailbox available
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}


    // Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}
}
