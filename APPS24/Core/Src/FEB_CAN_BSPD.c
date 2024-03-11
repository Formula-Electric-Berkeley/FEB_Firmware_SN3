// ********************************** Includes & External **********************************

#include "FEB_CAN_BSPD.h"


// *********************************** Struct ************************************

// ********************************** Functions **********************************

void FEB_CAN_BSPD_checkReset(){
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
		FEB_CAN_BSPD_Transmit();
	}

}

void FEB_CAN_BSPD_Transmit(){
 //do CAN STUFF
}
