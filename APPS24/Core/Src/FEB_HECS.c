// **************************************** Includes & External ****************************************

#include "FEB_HECS.h"

extern ADC_HandleTypeDef hadc1;



// **************************************** Configuration ****************************************

bool currHigh = false;


// **************************************** Functions ****************************************

void FEB_HECS_update(){
	uint16_t current_value = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_4);
//	char buf[128];
//	uint8_t buf_len;
//	buf_len = sprintf(buf, "HECS Current:%d\n", current_value);

	if (current_value > HECS_CURRENT){ //checks if current is above value (to be determined)
		currHigh = true;
	}
	FEB_HECS_indicate();
}

void FEB_HECS_indicate(){ //sends GPIO output to HECS Indicator
	if (currHigh == true){
		if (FEB_Normalized_getBrake() > 0.0){ //should check whether brake is pressed -- get double checked
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // sets GPIO PA5 to high
		}
	}else{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //sets GPIPO PA5 to low
	}
	HAL_Delay(1000);
}
