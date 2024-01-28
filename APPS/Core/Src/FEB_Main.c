
//******************* INCLUDES & EXTERNS ****************
#include "FEB_Main.h"

// *********************** VARIABLES *********************
# define SLEEP_TIME 10

float normalized_acc;
float normalized_brake;

static char buf[128];
static uint8_t buf_len;

//******************  FUNCTIONS ****************************

//this is a homeless function that i think needs to live here
void FEB_APPS_sendBrake(){
	FEB_CAN_Transmit(&hcan1);
}


void FEB_Main_User2(void){
	HAL_ADCEx_InjectedStart(&hadc1);
	HAL_TIM_Base_Start(&htim5);


	//char buf[128]; use static variable instead
	//uint8_t buf_len; use static variable insetad

	FEB_CAN_Init(); // The transceiver must be connected otherwise you get sent into an infinite loop
	RMSControl.enabled = 0;
	RMSControl.torque= 0.0;
}

void FEB_Main_User3(void){
	  //ready to drive
	  if (SW_MESSAGE.ready_to_drive == 1) {
		  normalized_acc = FEB_Normalized_Acc_Pedals();
		  if (!RMSControl.enabled) {  // when the car just powered on, driver commands ready to drive but rms is not enabled
			  FEB_RMS_Init();
			  RMSControl.enabled = 1;
		  }
	  } else {
		  // normalized_acc = FEB_Normalized_Acc_Pedals(); redundant
		  normalized_acc = 0.0;
		  RMSControl.enabled = 0;
	  }
	  // Top line is Evan's new function, Bottom line is old APPS function
	  uint16_t torque = normalized_acc * FEB_getMaxTorque(RMS_MESSAGE.HV_Bus_Voltage, RMS_MESSAGE.Motor_Speed);
//	  uint16_t torque = normalized_acc * 130;
	  normalized_brake = FEB_Normalized_Brake_Pedals();
	  FEB_RMS_setTorque(torque);
	  FEB_APPS_sendBrake();

//	  buf_len = sprintf(buf, "rtd:%d, enable:%d lockout:%d impl:%d acc: %.3f brake: %.3f Bus Voltage: %d Motor Speed: %d\n", SW_MESSAGE.ready_to_drive, Inverter_enable, Inverter_enable_lockout, isImpl, normalized_acc, normalized_brake, RMS_MESSAGE.HV_Bus_Voltage, RMS_MESSAGE.Motor_Speed);

	  buf_len = sprintf(buf, "Evan's Max Torque: %d\n", FEB_getMaxTorque(RMS_MESSAGE.HV_Bus_Voltage, RMS_MESSAGE.Motor_Speed));

	  HAL_UART_Transmit(&huart2,(uint8_t *)buf, buf_len, 1000);

	  HAL_Delay(SLEEP_TIME);
}





