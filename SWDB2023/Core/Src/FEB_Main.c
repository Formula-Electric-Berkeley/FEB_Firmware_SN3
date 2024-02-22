// ********************************** Includes & External **********************************

#include "FEB_Main.h"


// ********************************** Variables **********************************


static const uint8_t VOLUME = 2; // volume of the buzzer (10%)
static bool Button_Checking = false;
static bool Button_Timer_Flag = false;

uint8_t data2;

// ********************************** Functions **********************************

FEB_Main_While(){
	HAL_Delay(10);
	FEB_IOExpander_Read();
	FEB_Buttons_coolantPump(); //TODO: create standard transmit function that can switch buttons/processes
	FEB_Buttons_accumulatorFans();
}

//TODO: look at bottom of FEB_BUttons.c
