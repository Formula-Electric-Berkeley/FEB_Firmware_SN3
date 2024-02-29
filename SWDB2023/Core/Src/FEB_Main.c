// ********************************** Includes & External **********************************

#include "FEB_Main.h"


// ********************************** Variables **********************************


static const uint8_t VOLUME = 2; // volume of the buzzer (10%)


uint8_t data2;

// ********************************** Functions **********************************

FEB_Main_Setup(){
	FEB_Timer_Init();
	FEB_CAN_Init(); //TODO: double check implementation?
}


FEB_Main_While(){
	HAL_Delay(10);
	FEB_IOExpander_Read();
	FEB_Buttons_coolantPump(); //TODO: create standard transmit function that can switch buttons/processes
	FEB_Buttons_accumulatorFans();
	FEB_Buttons_readyToDrive();
}

FEB_Main_HALCallBack(TIM_HandleTypeDef *htim){
	//char buf[10];
	//int buf_len;

	//timer for button
	//if 1 sec has passed since first detection of button press, go back to not checking state
	if (FEB_Timer_equals13(TIM_HandleTypeDef *htim)){
		FEB_Buttons_stopChecking();
		FEB_Timer_stop1sec();

		//buf_len = sprintf((char*)buf, "Timer13\r\n");
		//HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
	}
	//timer for buzzer
	if (FEB_Timer_equals14(TIM_HandleTypeDef *htim)){
		FEB_Timer_stopBuzzer();
	}

}
