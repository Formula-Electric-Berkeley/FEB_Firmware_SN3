// ********************************** Includes & External **********************************

#include "FEB_Buttons.h"

extern uint8_t data2;
extern CAN_HandleTypeDef hcan1;


// ********************************** Variables **********************************

static bool Button_Checking = false;
static bool Button_Timer_Flag = false;

//TODO: coolant_pump and accumulator_fans, radiator_fans need to be transmitted from swithces 1-3

//coolant pump
bool lastButton_1 = 0;
bool lock_1 = 0;
bool coolant_pump = 0;


//accumulator fans
bool lastButton_2 = 0;
bool lock_2 = 0;
bool accumulator_fans = 0;

//extra_1
bool lastButton_3 = 0;
bool lock_3 = 0;
bool extra_1 = 0;

//extra_2
bool lastButton_4 = 0;
bool lock_4 = 0;
bool extra_2 = 0;

//switch_1
bool lastSwitch_1 = 0;
bool lock_5 = 0;
bool switch_1 = 0;

//switch_2
bool lastSwitch_2 = 0;
bool lock_6 = 0;
bool switch_2 = 0;

//switch_3
bool lastSwitch_3 = 0;
bool lock_7 = 0;
bool switch_3 = 0;

//ready to drive
bool ready_to_drive = false;
bool last_button_state = false;
bool lock = false;

// ********************************** Functions **********************************

//TODO: could reassign all buttons through this skeleton function
FEB_Button_Skeleton(bool buttonName, bool buttonLock, bool lastButton, uint8_t arrLocation, ){ //arrlocation should be number in lock variable
	if (!(data2 & (1<<arrLocation))) { //BUT_1 --> IO P1, coolant pump
		if (!buttonName){
			buttonLock = true;
			buttonName = !buttonName;
			switch (arrLocation){ // 5, 6, 7 should be coded to transmit functions for coolant_pump and accumulator_fans, radiator_fans
			case 5:
				//FEB_CAN_Transmit(&hcan1, SW_COOLANT_PUMP, (uint8_t *) &buttonName, 1);
			case 6:
				//FEB_CAN_Transmit(&hcan1, SW_COOLANT_PUMP, (uint8_t *) &buttonName, 1);
			case 7:
				//FEB_CAN_Transmit(&hcan1, SW_COOLANT_PUMP, (uint8_t *) &buttonName, 1);
			}
		}
		lastButton = true;
	} else {
		if (lastButton){
			buttonLock = false;
		}
		lastButton = false;
	}
	*out_buttonName = buttonName;
	*out_buttonLock = buttonLock;
	*out_lastButton = lastButton;
}


FEB_Buttons_coolantPump(){
	if (!(data2 & (1<<1))) { //BUT_1 --> IO P1, coolant pump
		if (!lastButton_1){
			lock_1 = true;
			coolant_pump = !coolant_pump;
			//FEB_CAN_Transmit(&hcan1, SW_COOLANT_PUMP, (uint8_t *) &coolant_pump, 1);
		}
		lastButton_1 = true;
	} else {
		if (lastButton_1){
			lock_1 = false;
		}
		lastButton_1 = false;

	}
}

FEB_Buttons_accumulatorFans(){
	if(!(data2 & (1<<2))) { //BUT_2 -> IO P2, accumulator fans
		if (!lastButton_2){
			lock_2 = true;
			accumulator_fans = !accumulator_fans;
			//FEB_CAN_Transmit(&hcan1, SW_ACUMULATOR_FANS, (uint8_t *) &accumulator_fans, 1);
		}
		lastButton_2 = true;
	} else {
		if (lastButton_2){
			lock_2 = false;
		}
		lastButton_2 = false;
	}

}

FEB_Buttons_extra_1(){
	if(!(data2 & (1<<3))) { //BUT_3 -> IO P3, extra_1
		if (!lastButton_3){
			lock_3 = true;
			extra_1 = !extra_1;
			//FEB_CAN_Transmit(&hcan1, SW_EXTRA, (uint8_t *) &extra, 1);
		}
		lastButton_3 = true;
	} else {
		if (lastButton_3){
			lock_3 = false;
		}
		lastButton_3 = false;
	}

}

FEB_Buttons_extra_2(){
	if(!(data2 & (1<<4))) { //BUT_4 -> IO P4, extra_2
		if (!lastButton_4){
			lock_4 = true;
			extra_2 = !extra_2;
			//FEB_CAN_Transmit(&hcan1, SW_EXTRA, (uint8_t *) &extra, 1);
		}
		lastButton_4 = true;
	} else {
		if (lastButton_4){
			lock_4 = false;
		}
		lastButton_4 = false;
	}

}

FEB_Buttons_switch_1(){
	if(!(data2 & (1<<5))) { //SWITCH_1 -> IO P5, switch_1
		if (!lastSwitch_1){
			lock_5 = true;
			switch_1 = !switch_1;
			//FEB_CAN_Transmit(&hcan1, SW_EXTRA, (uint8_t *) &extra, 1);
		}
		lastSwitch_1 = true;
	} else {
		if (lastSwitch_1){
			lock_5 = false;
		}
		lastSwitch_1 = false;
	}

}

FEB_Buttons_switch_2(){
	if(!(data2 & (1<<6))) { //SWITCH_2 -> IO P6, switch_2
		if (!lastSwitch_2){
			lock_6 = true;
			switch_2 = !switch_2;
			//FEB_CAN_Transmit(&hcan1, SW_EXTRA, (uint8_t *) &extra, 1);
		}
		lastSwitch_2 = true;
	} else {
		if (lastSwitch_2){
			lock_6 = false;
		}
		lastSwitch_2 = false;
	}

}

FEB_Buttons_switch_3(){
	if(!(data2 & (1<<7))) { //SWITCH_3 -> IO P7, switch_3
		if (!lastSwitch_3){
			lock_7 = true;
			switch_3 = !switch_3;
			//FEB_CAN_Transmit(&hcan1, SW_EXTRA, (uint8_t *) &extra, 1);
		}
		lastSwitch_3 = true;
	} else {
		if (lastSwitch_3){
			lock_7 = false;
		}
		lastSwitch_3 = false;
	}

}

FEB_Buttons_readyToDrive(){
	if (!(data2 & (1<<0))){ //RTD -> IO P0, ready to drive
		//buf_len = sprintf((char*)buf, "BUT_3\r\n");
		//HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);

		//if system not checking a button, start timer to check timer
		if (!Button_Checking && (last_button_state == false) && !lock){
			//Start timer to count 1 sec hold time
			FEB_Timer_start1sec();
			Button_Checking = true;

			//buf_len = sprintf((char*)buf, "starting timer 13\r\n");
			//HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);

		}

		// if 1 sec elapsed, and this button still pressed, it is a valid action
		// turn on buzzer and send CAN msg, reset states
		if (Button_Timer_Flag){
			ready_to_drive = !ready_to_drive;

			//when ready_to_drive state is just changed, lock from changing again
			lock = true;

			//start timer for buzzer
			FEB_Timer_startBuzzer();

			FEB_CAN_Transmit(); //for sending ready_to_drive; TODO: implement

			Button_Timer_Flag = false;
		}

		last_button_state = true;

	} else {
		Button_Time_Flag = false;
		if (last_button_state){
			last_button_state = false;
			lock = false;
		}
	}

	//buf_len = sprintf((char*)buf, "check:%d flag:%d\r\n", Button_Checking, Button_Timer_Flag);
	//HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
	buf_len = sprintf((char*)buf, "ready: %d coolant: %d accumulator: %d extra_1: %d\r\n", ready_to_drive, coolant_pump, accumulator_fans, extra_1);
	HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
}


FEB_Buttons_stopChecking(){
	Button_Checking = false;
	Button_Timer_Flag = true;
}

