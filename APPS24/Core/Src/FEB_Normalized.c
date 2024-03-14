// **************************************** Includes & External ****************************************

#include "FEB_Normalized.h"

extern CAN_HandleTypeDef hcan1;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

const uint16_t Sensor_Min = 4095.0/5.0*0.5;
const uint16_t Sensor_Max = 4095.0/5.0*4.5;

// **************************************** Configuration ****************************************


float normalized_acc;
float normalized_brake;
bool isImpl = false;

// **************************************** Functions ****************************************

float FEB_Normalized_getAcc(){
	return normalized_acc;
}

void FEB_Normalized_setAcc0(){
	normalized_acc = 0.0;
}

void FEB_Normalized_updateAcc(){
	normalized_acc = FEB_Normalized_Acc_Pedals();
}



float FEB_Normalized_Acc_Pedals(){
	// raw ADC readings of the two acc pedal sensors
	uint16_t acc_pedal_1 = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
	uint16_t acc_pedal_2 = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_3);
	char buf[128];
	uint8_t buf_len;
	buf_len = sprintf(buf, "acc1:%d acc2:%d\n", acc_pedal_1, acc_pedal_2);
	//HAL_UART_Transmit(&huart2,(uint8_t *)buf, buf_len, HAL_MAX_DELAY);


	// check implausibility for shorting
	if (acc_pedal_1 < Sensor_Min || acc_pedal_1 > Sensor_Max
			|| acc_pedal_2 < Sensor_Min || acc_pedal_2 > Sensor_Max
			|| abs(acc_pedal_1 - acc_pedal_2) < 100) {
		isImpl = true;
		return 0.0;
	}

	//convert to % travel
	// sensor 1 has positive slope
	float ped1_normalized = (acc_pedal_1 - ACC_PEDAL_1_START)/ (ACC_PEDAL_1_END - ACC_PEDAL_1_START);
	// sensor 2 has negative slope
	float ped2_normalized = (acc_pedal_2 - ACC_PEDAL_2_START) / (ACC_PEDAL_2_END - ACC_PEDAL_2_START);

	// sensor measurements mismatch by more than 10%
	if(abs(ped1_normalized - ped2_normalized) > 0.1 ){
		isImpl = true;
		return 0.0;
	}

	float final_normalized = 0.5*(ped1_normalized + ped2_normalized);

	// Implausiblity check if both pedals are stepped
	if (normalized_brake > 0.2 && normalized_acc > 0.1) {
		isImpl = true;
	}

	// recover from implausibility if acc pedal is not 5% less
	if (final_normalized < 0.05 && isImpl) {
		isImpl = false;
	}

	if (!isImpl) {
		final_normalized = final_normalized > 1 ? 1 : final_normalized;
		final_normalized = final_normalized < 0.05 ? 0 : final_normalized;
		return final_normalized;
	} else {
		return 0.0;
	}
}

float FEB_Normalized_getBrake(){
	return normalized_brake;
}

float FEB_Normalized_Brake_Pedals(){
	uint16_t brake_pedal_1 = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
	char buf[128];
	uint8_t buf_len;
	buf_len = sprintf(buf, "brake%d\n", brake_pedal_1);
	HAL_UART_Transmit(&huart2,(uint8_t *)buf, buf_len, HAL_MAX_DELAY);

	float final_normalized = (brake_pedal_1 - BRAKE_PEDAL_1_START)/ (BRAKE_PEDAL_1_END - BRAKE_PEDAL_1_START);
	final_normalized = final_normalized > 1 ? 1 : final_normalized;
	final_normalized = final_normalized < 0.05 ? 0 : final_normalized;

	if (brake_pedal_1 < 10 || brake_pedal_1 > 4085) {
		return 0.0;
	}

	return final_normalized;
}

void FEB_Normalized_CAN_sendBrake(){
	// Initialize transmission header
	FEB_CAN_Tx_Header.DLC = 4;
	FEB_CAN_Tx_Header.ExtId = FEB_CAN_ID_APPS_BRAKE_PEDAL;
	FEB_CAN_Tx_Header.IDE = CAN_ID_EXT;
	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;

	// Copy data to Tx buffer
	memcpy(FEB_CAN_TxData, normalized_brake, sizeof(float));


	// Delay until mailbox available
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}

	// Add Tx data to mailbox
	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
	}

}



