// ********************************** Includes & External **********************************
#include "FEB_Fan.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

// ********************************** Fan Configuration **********************************

static uint8_t FEB_Fan_1_Speed = 0;		// [0, 255]
static uint8_t FEB_Fan_2_Speed = 0;		// [0, 255]
static uint8_t FEB_Fan_3_Speed = 0;		// [0, 255]
static uint8_t FEB_Fan_4_Speed = 0;		// [0, 255]
static uint8_t FEB_Fan_5_Speed = 0;		// [0, 255]

static PIDController FEB_Fan_1_PID;
static PIDController FEB_Fan_2_PID;
static PIDController FEB_Fan_3_PID;
static PIDController FEB_Fan_4_PID;
static PIDController FEB_Fan_5_PID;

static uint8_t Accumulator_temp = 30;
static uint8_t Fan_1_section_temp = 30;
static uint8_t Fan_2_section_temp = 30;
static uint8_t Fan_3_section_temp = 30;
static uint8_t Fan_4_section_temp = 30;
static uint8_t Fan_5_section_temp = 30;

// ********************************** Initialize **********************************

void FEB_Fan_Init(){
	FEB_Fan_PWM_Init();
	FEB_Fan_All_Speed_Set(255 * 0.5);
	FEB_PID_Init_All();
}

// ********************************** Setpoint Generation **********************************
// Using average temperature
uint32_t FEB_Get_Fan_Setpoint(){
	if (Accumulator_temp <= 30){
			return 0.5 * 255;
	} else if (Accumulator_temp <= 35){
		return 0.75 * 255;
	} else {
		return 255;
	}
}

// Using temperature for individual sections of the accumulator
uint32_t FEB_Get_Fan_1_Setpoint(){
	if (Fan_1_section_temp <= 30){
		return 0.5 * 255;
	} else if (Fan_1_section_temp <= 35){
		return 0.75 * 255;
	} else {
		return 255;
	}
}

uint32_t FEB_Get_Fan_2_Setpoint(){
	if (Fan_2_section_temp <= 30){
		return 0.5 * 255;
	} else if (Fan_2_section_temp <= 35){
		return 0.75 * 255;
	} else {
		return 255;
	}
}

uint32_t FEB_Get_Fan_3_Setpoint(){
	if (Fan_3_section_temp <= 30){
		return 0.5 * 255;
	} else if (Fan_3_section_temp <= 35){
		return 0.75 * 255;
	} else {
		return 255;
	}
}

uint32_t FEB_Get_Fan_4_Setpoint(){
	if (Fan_4_section_temp <= 30){
		return 0.5 * 255;
	} else if (Fan_4_section_temp <= 35){
		return 0.75 * 255;
	} else {
		return 255;
	}
}

uint32_t FEB_Get_Fan_5_Setpoint(){
	if (Fan_5_section_temp <= 30){
		return 0.5 * 255;
	} else if (Fan_5_section_temp <= 35){
		return 0.75 * 255;
	} else {
		return 255;
	}
}


// ********************************** PID **********************************
void FEB_PID_Init_All(){
	FEB_PID_Init(&FEB_Fan_1_PID);
	FEB_PID_Init(&FEB_Fan_2_PID);
	FEB_PID_Init(&FEB_Fan_3_PID);
	FEB_PID_Init(&FEB_Fan_4_PID);
	FEB_PID_Init(&FEB_Fan_5_PID);
}

void FEB_Update_PWM_All_Fans(){
	FEB_Update_PWM_Fan_1();
}

void FEB_Update_PWM_Fan_1(){
	uint32_t setpoint = FEB_Get_Fan_1_Setpoint();
	uint32_t tachometer_reading = FEB_Fan_Read_Tachometer_Fan_1();
	float new_PWM = FEB_PID_Update(&FEB_Fan_1_PID, setpoint, tachometer_reading);
	FEB_Fan_1_Speed_Set(new_PWM);
}

// ********************************** PWM **********************************
void FEB_Fan_PWM_Init(void) {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

void FEB_Fan_All_Speed_Set(uint8_t speed) {
	FEB_Fan_1_Speed_Set(speed);
	FEB_Fan_2_Speed_Set(speed);
	FEB_Fan_3_Speed_Set(speed);
	FEB_Fan_4_Speed_Set(speed);
}

void FEB_Fan_1_Speed_Set(uint8_t speed) {
	FEB_Fan_1_Speed = speed;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, FEB_Fan_1_Speed);
}

void FEB_Fan_2_Speed_Set(uint8_t speed) {
	FEB_Fan_2_Speed = speed;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, FEB_Fan_2_Speed);
}

void FEB_Fan_3_Speed_Set(uint8_t speed) {
	FEB_Fan_3_Speed = speed;
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, FEB_Fan_3_Speed);
}

void FEB_Fan_4_Speed_Set(uint8_t speed) {
	FEB_Fan_4_Speed = speed;
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, FEB_Fan_4_Speed);
}

void FEB_Fan_5_Speed_Set(uint8_t speed) {
	FEB_Fan_5_Speed = speed;
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, FEB_Fan_5_Speed);
}


// ********************************** Tachometer **********************************


uint32_t FEB_Fan_Read_Tachometer_Fan_1() {
	return 0;
}