// ******************************** Includes ********************************

#include "FEB_Daughter_Tester.h"

// ******************************** Functions ********************************

void FEB_Daughter_Tester_Test_Daughter(void) {

}

void FEB_Daughter_Tester_Test_Daughter_Voltages(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET); //SRCLR High
	for (int i = 0; i < 10; i++) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Shift out 1 bit
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //Clock High
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //Clock Low
		FEB_Daughter_Tester_Test_Daughter_Voltage(); //Test Cell Voltage
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET); //SRCLR Low
}

void FEB_Daughter_Tester_Test_Daughter_Voltage(void) {

}
