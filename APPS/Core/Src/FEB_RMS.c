/*
 * FEB_RMS.c
 *
 *  Created on: Nov 12, 2023
 *      Author: viploverahate
 */
//************************ INCLUDES *********************


#include "FEB_RMS.h"

// *********************** VARIABLES *********************

#define INIT_VOLTAGE 242 // initial voltage of accumulator
#define PEAK_CURRENT 95  // max current (in amps) we want to pull
#define MAX_TORQUE 230
#define RPM_TO_RAD_S .10472

RMSControlType RMSControl;

// *********************** FUNCTIONS *********************

void FEB_RMS_updateTorque() {
  uint8_t message_data[8] = {RMSControl.torque & 0xFF, RMSControl.torque >> 8, 0, 0, 0, RMSControl.enabled, 0, 0};
  FEB_CAN_Transmit(&hcan1, 0x0C0, message_data, 8);
}

void FEB_RMS_setTorque(uint16_t torque) {
  RMSControl.torque = torque * 10;
  FEB_RMS_updateTorque();
}

void FEB_RMS_enable() {
  RMSControl.enabled = 1;
  FEB_RMS_updateTorque();
}

void FEB_RMS_disable() {
  RMSControl.enabled = 0;
  FEB_RMS_updateTorque();
}

void FEB_RMS_Init(){
	// Clear fault in case inverter is powered up before disable command is sent
	uint8_t fault_clear_addr = 20;
	uint8_t fault_clear_data = 0;
	// param msg format (little-endian):
	// 0,1: param addr
	// 2: r/w cmd
	// 3: NA
	// 4,5: data
	// 6,7: NA
	uint8_t param_msg[8] = {fault_clear_addr, 0, 1, 0, fault_clear_data, 0, 0, 0};
	FEB_CAN_Transmit(&hcan1, 0x0C1, param_msg, 8);

	// send disable command to remove lockout
	uint8_t message_data[8] = {0,0,0,0,0,0,0};
	for (int i = 0; i < 10; i++) {
		FEB_CAN_Transmit(&hcan1, 0x0C0, message_data, 8);
		HAL_Delay(10);
	}

	// Select CAN msg to broadcast
	uint8_t param_addr = 148;
//	uint8_t CAN_active_msg_byte4 = 0b10100000; // motor position, input voltage
//	uint8_t CAN_active_msg_byte5 = 0b00010101; // flux info (dq axes), torque/timer info, internal states
	uint8_t CAN_active_msg_byte4 = 0xff; // literally log everything
	uint8_t CAN_active_msg_byte5 = 0xff;
	uint8_t broadcast_msg[8] = {param_addr, 0, 1, 0, CAN_active_msg_byte4, CAN_active_msg_byte5, 0, 0};
	FEB_CAN_Transmit(&hcan1, 0x0C1, broadcast_msg, 8);
}

int16_t FEB_min(int16_t x1, int16_t x2) {
	if (x1 < x2) {
		return x1;
	}
	return x2;
}
// returns a maximum torque that can be given from the motor at a given RPM and voltage
uint16_t FEB_getMaxTorque(int16_t voltage, int16_t RPM) {
	int16_t accumulator_voltage = FEB_min(INIT_VOLTAGE, (voltage-50) / 10);
	int16_t motor_speed = -1 * RPM * RPM_TO_RAD_S;
  // If speed is less than 15, we should command max torque
  // This catches divide by 0 errors and also negative speeds (which may create very high negative torque values)
	if (motor_speed < 15) {
		return MAX_TORQUE;
	}
	uint16_t maxTorque = FEB_min(MAX_TORQUE, (accumulator_voltage * PEAK_CURRENT) / motor_speed);
	return maxTorque;
}


