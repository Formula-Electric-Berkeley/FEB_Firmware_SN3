// ********************************** Includes & External **********************************

#include "FEB_Buttons.h"

extern uint8_t data2;


// ********************************** Variables **********************************


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
	if(!(data2 & (1<<2))) {
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
	if(!(data2 & (1<<3))) {
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

//TODO: add function for Ready to Drive, and implementation after; extern everything to a CAN file and add a CAN file?





