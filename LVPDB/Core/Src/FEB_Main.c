#include "FEB_Main.h"

extern FEB_CAN_APPS_Message_t FEB_CAN_APPS_Message;

extern I2C_HandleTypeDef hi2c1;
extern CAN_HandleTypeDef hcan1;


extern float current_reading;
extern float ex_current_reading;
extern float cp_current_reading;

float apps_current_reading; //

void FEB_Main_Setup(void) {


	// configuration register value
	uint8_t CONFIG[2] = {0b01000001, 0b00100111}; // default settings


	/*
	 * Values needed to calibrate each of the TPS chips. This value gets passed
	 * into the init function called in later
	 */

	// calibration register values
	uint8_t MAIN_CAL[2] = {0b00000110, 0b10001110}; // Imax = 50A
	uint8_t CP_CAL[2] = {0b00010111, 0b01101000};
	uint8_t AF_CAL[2] = {0b00101000, 0b11110110};
	uint8_t EX_CAL[2] = {0b00110110, 0b10011101};

	// alert types
	uint8_t UNDERV[2] = {0b00010000, 0b00000000};
	uint8_t OVERPWR[2] = {0b00001000, 0b00000000};

	// limits
	uint8_t LV_LIMIT[2] = {0b00000000, 0b00010110}; // = 22
	uint8_t CP_LIMIT[2] = {0b00000001, 0b01010000}; // = 336, 14 * 24
	uint8_t AF_LIMIT[2] = {0b00000000, 0b11000000}; // = 192, 8 * 24
	uint8_t EX_LIMIT[2] = {0b00000000, 0b10010000}; // = 144, 6 * 24

	FEB_CAN_Init();

	//hi2c1p = &hi2c1;

	// uncomment if we need to pull ENs high to start
	//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);// pull PC11 high to enable coolant pump
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);// pull PB5 high to enable accumulator fans
	//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);// pull PC3 high to enable extra

	//TODO: GET ACCURATE UNDERV AND OVERPWR AND DIFFERENT LIMITS. THESE ARE PRIMARILY PLACEHOLDERS.

	FEB_SETUP_TPS2482(&hi2c1, LV_ADDR, CONFIG, MAIN_CAL, UNDERV, LV_LIMIT);
	FEB_SETUP_TPS2482(&hi2c1, CP_ADDR, CONFIG, CP_CAL, OVERPWR, CP_LIMIT);
	FEB_SETUP_TPS2482(&hi2c1, AF_ADDR, CONFIG, AF_CAL, OVERPWR, AF_LIMIT);
	FEB_SETUP_TPS2482(&hi2c1, EX_ADDR, CONFIG, EX_CAL, OVERPWR, EX_LIMIT);
	FEB_SETUP_TPS2482(&hi2c1, SH_ADDR, CONFIG, EX_CAL, OVERPWR, EX_LIMIT);
}


void FEB_Main_Loop(void) {

  FEB_Brake_Light_Control();

  FEB_Peripherals_Control();

  FEB_TPS2482_Poll_Currents();

  FEB_CAN_Transmit(&hcan1, LVPDB_LV_CURRENT, &current_reading);
  FEB_CAN_Transmit(&hcan1, LVPDB_EX_CURRENT, &ex_current_reading);
  FEB_CAN_Transmit(&hcan1, LVPDB_CP_CURRENT, &cp_current_reading);
  apps_current_reading = FEB_CAN_APPS_Message.current;
}
