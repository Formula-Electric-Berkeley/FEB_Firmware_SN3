// ********************************** Includes & External **********************************

#include "FEB_Buttons.h"

extern uint8_t data2;


// ********************************** Variables **********************************

static bool Button_Checking = false;
static bool Button_Timer_Flag = false;

//coolant pump
bool lastButton_4 = 0;
bool lock_4 = 0;
bool coolant_pump = 0;


//accumulator fans
bool lastButton_5 = 0;
bool lock_5 = 0;
bool accumulator_fans = 0;

//extra
bool lastButton_6 = 0;
bool lock_6 = 0;
bool extra = 0;

//ready to drive
bool ready_to_drive = false;
bool last_button_state = false;
bool lock = false;

// ********************************** Functions **********************************

FEB_Buttons_coolantPump(){
	if (!(data2 & (1<<1))) { //BUT_4 --> IO2 P1, coolant pump
		if (!lastButton_4){
			lock_4 = true;
			coolant_pump = !coolant_pump;
			//FEB_CAN_Transmit(&hcan1, SW_COOLANT_PUMP, (uint8_t *) &coolant_pump, 1);
		}
		lastButton_4 = true;
	} else {
		if (lastButton_4){
			lock_4 = false;
		}
		lastButton_4 = false;

	}
}

FEB_Buttons_accumulatorFans(){
	if(!(data2 & (1<<2))) { //BUT_5 -> IO@ P2, accumulator fans
		if (!lastButton_5){
			lock_5 = true;
			accumulator_fans = !accumulator_fans;
			//FEB_CAN_Transmit(&hcan1, SW_ACUMULATOR_FANS, (uint8_t *) &accumulator_fans, 1);
		}
		lastButton_5 = true;
	} else {
		if (lastButton_5){
			lock_5 = false;
		}
		lastButton_5 = false;
	}

}

FEB_Buttons_Extra(){
	if(!(data2 & (1<<3))) { //BUT_6 -> IO@ P3, extra
		if (!lastButton_6){
			lock_6 = true;
			extra = !extra;
			//FEB_CAN_Transmit(&hcan1, SW_ACUMULATOR_FANS, (uint8_t *) &extra, 1);
		}
		lastButton_6 = true;
	} else {
		if (lastButton_6){
			lock_6 = false;
		}
		lastButton_6 = false;
	}

}

FEB_Buttons_readyToDrive(){
	if (!(data2 & (1<<0))){ //BuT_3 -> IO2 P0, ready to drive
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
	buf_len = sprintf((char*)buf, "ready: %d coolant: %d accumulator: %d extra: %d\r\n", ready_to_drive, coolant_pump, accumulator_fans, extra);
	HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
}


FEB_Buttons_stopChecking(){
	Button_Checking = false;
	Button_Timer_Flag = true;
}

