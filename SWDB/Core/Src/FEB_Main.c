/*
 * FEB_Main.c
 *
 *  Created on: Jan 21, 2024
 *      Author: viploverahate
 */

//******************* INCLUDES & EXTERNS ****************
#include "FEB_Main.h"


// *********************** VARIABLES *********************
static const uint8_t IOEXP2_ADDR = 0b0100001 << 1; // ask aslex what this is
static const uint8_t VOLUME = 2; // volume of the buzzer (10%)
static bool Button_Checking = false;
static bool Button_Timer_Flag = false;

//below is from user2, but needs to be accessed by other functions
bool ready_to_drive = false;
bool last_button_state = false;
bool lock = false;

bool lastButton_4 = 0; // coolant pump
bool lastButton_5 = 0; // acumulator fans
bool lastButton_6 = 0; // extra
bool lock_4 = 0;
bool lock_5 = 0;
bool lock_6 = 0;
bool coolant_pump = 0;
bool accumulator_fans = 0;
bool extra = 0;

char buf[128];
int buf_len;
//uint8_t data1;
uint8_t data2;
HAL_StatusTypeDef ret;


//******************  FUNCTIONS ****************************

void FEB_Main_User2(void){


	  HAL_TIM_Base_Start_IT(&htim13);
	  HAL_TIM_Base_Stop_IT(&htim13);
	  HAL_TIM_Base_Start_IT(&htim14);
	  HAL_TIM_Base_Stop_IT(&htim14);
	  Button_Checking = false;
	  Button_Timer_Flag = false;
	  // 0% PWM cycle to turnoff buzzer
	  htim2.Instance->CCR2 = 0;
	  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);


	  FEB_CAN_Init();
}

void FEB_Main_While(void){

	  HAL_Delay(10);
	  // read both IOexpanders
//	  ret = HAL_I2C_Master_Receive(&hi2c1, IOEXP1_ADDR, &data1, 1, HAL_MAX_DELAY);
//	  if ( ret != HAL_OK ) {
//		  buf_len = sprintf((char*)buf, "IO_1 Error\r\n");
//	  }
	  ret = HAL_I2C_Master_Receive(&hi2c1, IOEXP2_ADDR, &data2, 1, HAL_MAX_DELAY);
	  if ( ret != HAL_OK ) {
		  buf_len = sprintf((char*)buf, "IO_2 Error\r\n");
	  }

	  // check the read values
//	  if (data1 == 0xFF && data2 == 0xFF) {
//		  buf_len = sprintf((char*)buf, "NO BUT\r\n");
//	  }
//	  if (!(data1 & (1<<1))) { // BUT_0 -> IO1 P1
//		  buf_len = sprintf((char*)buf, "BUT_0\r\n");
//	  }
//	  if (!(data1 & (1<<0))) { // BUT_1 -> IO1 P0
//		  buf_len = sprintf((char*)buf, "BUT_1\r\n");
//	  }
//	  if (!(data2 & (1<<7))) { // BUT_2 -> IO2 P7
//		  buf_len = sprintf((char*)buf, "BUT_2\r\n");
//	  }
//	  if (!(data2 & (1<<0))) { // BUT_3 -> IO2 P0
//		  buf_len = sprintf((char*)buf, "BUT_3\r\n");
//		  // Start timer
//		  HAL_TIM_Base_Start_IT(&htim14);
//		  // turn on buzzer with 30% PWM cycle
//		  htim2.Instance->CCR2 = VOLUME;
//	  }

	  if (!(data2 & (1<<1))) { // BUT_4 -> IO2 P1, coolant pump
		  if (!lastButton_4) {
			  lock_4 = true;
			  coolant_pump = !coolant_pump;
			  FEB_CAN_Transmit(&hcan1);
		  }
		  lastButton_4 = true;
	  } else {
		  if (lastButton_4) {
			  lock_4 = false;
		  }
		  lastButton_4 = false;
	  }

	  if (!(data2 & (1<<2))) { // BUT_5 -> IO2 P2, accumulator fans
		  if (!lastButton_5) {
			  lock_5 = true;
			  accumulator_fans = !accumulator_fans;
			  FEB_CAN_Transmit(&hcan1);
		  }
		  lastButton_5 = true;
	  } else {
		  if (lastButton_5) {
			  lock_5 = false;
		  }
		  lastButton_5 = false;
	  }

	  if (!(data2 & (1<<3))) { // BUT_6 -> IO2 P3, extra
		  if (!lastButton_6) {
			  lock_6 = true;
			  extra = !extra;
			  FEB_CAN_Transmit(&hcan1);
		  }
		  lastButton_6 = true;
	  } else {
		  if (lastButton_6) {
			  lock_6 = false;
		  }
		  lastButton_6 = false;
	  }

	  if (!(data2 & (1<<0))) { // BUT_3 -> IO2 P0, ready to drive
//		  buf_len = sprintf((char*)buf, "BUT_3\r\n");
//		  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);

		  // if the system is not checking a button, start the timer to check the timer
		  if (!Button_Checking && (last_button_state == false) && !lock) {
			  // Start timer to count 1 sec hold time
			  HAL_TIM_Base_Start_IT(&htim13);
			  Button_Checking = true;

//			  buf_len = sprintf((char*)buf, "starting timer 13\r\n");
//			  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
		  }
		  // if 1 sec has elapsed, and this button is still pressed, it is a valid action
		  // turn on buzzer and send CAN msg, reset states
		  if (Button_Timer_Flag) {
			  ready_to_drive = !ready_to_drive;

			  // when the ready_to_drive state is just changed, lock from changing again
			  lock = true;

			  // start timer for buzzer
			  HAL_TIM_Base_Start_IT(&htim14);
			  // turn on buzzer at VOLUME
			  htim2.Instance->CCR2 = VOLUME;

			  FEB_CAN_Transmit(&hcan1);

			  Button_Timer_Flag = false;

//			  buf_len = sprintf((char*)buf, "staring timer 14\r\n");
//			  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
		  }

		  //record button state
		  last_button_state = true;

	  } else {
		  Button_Timer_Flag = false;

		  // allow ready_to_drive to change when button is released.
		  if (last_button_state) {
			  last_button_state = false;
			  lock = false;
		  }
	  }
//	  buf_len = sprintf((char*)buf, "check:%d flag:%d\r\n", Button_Checking, Button_Timer_Flag);
//	  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
	  buf_len = sprintf((char*)buf, "ready: %d coolant: %d accumulator: %d extra: %d\r\n", ready_to_drive, coolant_pump, accumulator_fans, extra);
	  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);

}
//this goes under user 4
	//READ: Below Function is declared but not used in the main file anywhere according to SN2 main.c
	/*
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
	//	char buf[10];
	//	int buf_len;

		// timer for button
		// if 1 sec has passed since the first detection of button pressed, go back to not checking state
		if (htim == &htim13) {
		  Button_Checking = false;
		  Button_Timer_Flag = true;
		  HAL_TIM_Base_Stop_IT(&htim13);

	//	  buf_len = sprintf((char*)buf, "Timer13\r\n");
	//	  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
		}

		// timer for buzzer
		if (htim == &htim14)
		{
		  // turn off buzzer
		  htim2.Instance->CCR2 = 0;
		  // Stop timer
		  HAL_TIM_Base_Stop_IT(&htim14);

	//	  buf_len = sprintf((char*)buf, "Timer14\r\n");
	//	  HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, HAL_MAX_DELAY);
		}
	}
	*/
