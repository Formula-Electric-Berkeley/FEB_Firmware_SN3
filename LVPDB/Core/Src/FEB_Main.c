#include "FEB_Main.h"
#include "stdio.h"
#include "stdbool.h"


extern FEB_CAN_APPS_Message_t FEB_CAN_APPS_Message;
extern FEB_CAN_SW_Message_t FEB_CAN_SW_Message;
extern I2C_HandleTypeDef hi2c1;
extern CAN_HandleTypeDef hcan1;

// I2C_HandleTypeDef* hi2c1p;

/*
 * The LVPDB has multiple TPS chips on the bus. These are the addresses of
 * each of the TPS chips. The naming conventions is as follows:
 * 		LV - Low voltage bus (the main one)
 * 		CP - Coolant pump
 * 		AF - Accumulator fans
 * 		EX - extra radiator fans
 */
#define LV_ADDR (0b1000000 << 1)
#define CP_ADDR (0b1000100 << 1)
#define AF_ADDR (0b1000101 << 1)
#define EX_ADDR (0b1000001 << 1)

static bool isDriving = false;


char buf[128];
int buf_len;
float current_reading;
float ex_current_reading;
float cp_current_reading;
float apps_current_reading;


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

	FEB_TPS2482_SETUP(&hi2c1, LV_ADDR, CONFIG, MAIN_CAL, UNDERV, LV_LIMIT);
	FEB_TPS2482_SETUP(&hi2c1, CP_ADDR, CONFIG, CP_CAL, OVERPWR, CP_LIMIT);
	FEB_TPS2482_SETUP(&hi2c1, AF_ADDR, CONFIG, AF_CAL, OVERPWR, AF_LIMIT);
	FEB_TPS2482_SETUP(&hi2c1, EX_ADDR, CONFIG, EX_CAL, OVERPWR, EX_LIMIT);


}


void FEB_Main_Loop(void) {


  // Brake Light
  if (FEB_CAN_APPS_Message.brake_pedal > BRAKE_THRE) {
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);// PA1 high
  } else {
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);// PA1 low
  }

  // activate peripheral devices if ready to drive
  if (FEB_CAN_SW_Message.ready_to_drive == 1 && !isDriving) {
	  isDriving = true;
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);// pull PC11 high to enable coolant pump
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);// pull PB5 high to enable accumulator fans
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);// pull PC3 high to enable extra

  // de-activate if not ready to drive
  } else if (FEB_CAN_SW_Message.ready_to_drive == 0 && isDriving) {
	  isDriving = false;
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
  }



  current_reading = FEB_TPS2482_PollBusCurrent(&hi2c1,LV_ADDR+1);
  ex_current_reading = FEB_TPS2482_PollBusCurrent(&hi2c1,EX_ADDR+1);
  cp_current_reading = FEB_TPS2482_PollBusCurrent(&hi2c1,CP_ADDR+1);

  FEB_CAN_Transmit(&hcan1, LVPDB_LV_CURRENT, &current_reading);
  FEB_CAN_Transmit(&hcan1, LVPDB_EX_CURRENT, &ex_current_reading);
  FEB_CAN_Transmit(&hcan1, LVPDB_CP_CURRENT, &cp_current_reading);
  apps_current_reading = FEB_CAN_APPS_Message.current;

  //make CAN_current file with these functions, one fnction per unqiue transmit
  //file itself should delare hcan1
  //configure tx header (check struct to see what to configure)
  //then send message w/



  buf_len = sprintf((char*)buf, "ready: %d, brake: %.3f\r\n", SW_MESSAGE.ready_to_drive, FEB_CAN_APPS_Message.brake_pedal);
  //HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);

  buf_len = sprintf((char*) buf, "Current Draw (LV, EX, CP, APPS): %.3f, %.3f, %.3f, %.3f\r\n", current_reading, ex_current_reading, cp_current_reading, apps_current_reading);
  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);


}
