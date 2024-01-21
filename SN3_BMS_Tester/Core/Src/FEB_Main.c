// ******************************** Includes ********************************

#include "FEB_Main.h"
#include "main.h"

// ******************************** Functions ********************************

void FEB_Main_Setup(void) {

}

void FEB_Main_Loop(void) {
	//If Start Pressed:
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET) {
		//If Aux:
		FEB_Aux_Tester_Test_Aux();

		//If Daughter:
		FEB_Daughter_Tester_Test_Daughter();
	}
}
