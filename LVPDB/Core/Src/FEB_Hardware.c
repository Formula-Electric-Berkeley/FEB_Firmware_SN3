#include "FEB_Hardware.h"

extern FEB_CAN_APPS_Message_t FEB_CAN_APPS_Message;


void FEB_Peripherals_Control(){


	  if(FEB_Accum_Fan_Control()){
		  Enable_Accumulator_Fans();
	  }else {
		  Disable_Accumulator_Fans();
	  }

	  //Enable Coolant pump, else disable if not received signal
	  if(FEB_Coolant_Pump_Control()){
		  Enable_Coolant_Pump();
	  }else{
		  Disable_Coolant_Pump();
	  }

	  if(FEB_Extra_Control()){
		  Enable_Extra();
	  }else{
		  Disable_Extra();
	  }

//	  // activate peripheral devices if ready to drive
//	  if (FEB_Ready_To_Drive_Status()) {
////		  Enable_Coolant_Pump();
////		  Enable_Accumulator_Fans();
////		  Enable_Extra();
//
//		  //Only enable if we are ready to drive and if we get signal to
//		  //not turn ono, then disable.
//}
//
//	  // de-activate if not ready to drive
//	  } else {
//		  Disable_Coolant_Pump();
//		  Disable_Accumulator_Fans();
//		  Disable_Extra();
//	  }


}


void FEB_Brake_Light_Control(){
	if (FEB_CAN_APPS_Message.brake_pedal >= 4) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);// PA1 high
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);// PA1 low
	}
}

void Enable_Coolant_Pump(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);// pull PC11 high to enable coolant pump
}

void Disable_Coolant_Pump(){
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
}

void Enable_Accumulator_Fans(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);// pull PB5 high to enable accumulator fans
}

void Disable_Accumulator_Fans(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

void Enable_Extra(){
	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);// pull PC3 high to enable extra
}

void Disable_Extra(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
}

void Enable_Shutdown_Source(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);// pull PA0 high to enable shutdown source
}

void Disable_Shutdown_Source(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
}
