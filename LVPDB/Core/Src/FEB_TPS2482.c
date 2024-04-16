#include "FEB_TPS2482.h"

/*
 * The LVPDB has multiple TPS chips on the bus. These are the addresses of
 * each of the TPS chips. The naming conventions is as follows:
 * 		LV - Low voltage bus (the main one)
 * 		CP - Coolant pump
 * 		AF - Accumulator fans
 * 		EX - extra radiator fans
 */



float current_reading;
float ex_current_reading;
float cp_current_reading;

extern I2C_HandleTypeDef hi2c1;

void FEB_SETUP_TPS2482(I2C_HandleTypeDef *hi2cp, uint8_t DEV_ADDR, uint8_t CONFIG[], uint8_t CAL_REG[], uint8_t ALERT[], uint8_t LIMIT[]) {
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Write(hi2cp, DEV_ADDR, 0x00, 1, CONFIG, 2, HAL_MAX_DELAY); // configure
//	if (ret != HAL_OK) {
//			// error handler
//		__disable_irq();
//		while(1);
//	}
	ret = HAL_I2C_Mem_Write(hi2cp, DEV_ADDR, 0x05, 1, CAL_REG, 2, HAL_MAX_DELAY); // calibrate
//	if (ret != HAL_OK) {
//			// error handler
//		__disable_irq();
//		while(1);
//	}
	ret = HAL_I2C_Mem_Write(hi2cp, DEV_ADDR, 0x06, 1, ALERT, 2, HAL_MAX_DELAY); // set alert
//	if (ret != HAL_OK) {
//			// error handler
//		__disable_irq();
//		while(1);
//	}
	ret = HAL_I2C_Mem_Write(hi2cp, DEV_ADDR, 0x07, 1, LIMIT, 2, HAL_MAX_DELAY); // set limit
//	if (ret != HAL_OK) {
//		// error handler
//		__disable_irq();
//		while(1);
//	}

	if (ret != HAL_OK) {

	}
}

void FEB_TPS2482_shutdownIfError(I2C_HandleTypeDef * hi2c, uint8_t DEV_ADDR, GPIO_TypeDef * EN, uint16_t EN_NUM, GPIO_TypeDef * AL, uint16_t AL_NUM,
		GPIO_TypeDef * PG, uint16_t PG_NUM, float VMAX, float VMIN, float IMAX, float IMIN, float PMAX, float PMIN) {

	// pull EN low if
		// 1. alert pin pulled low
		// 2. PG goes low while EN is high
		// 3. shunt voltage, bus voltage, power, or current out of bounds

	// 1.
	if (HAL_GPIO_ReadPin(AL, AL_NUM) == GPIO_PIN_RESET) {
		HAL_GPIO_WritePin(EN, EN_NUM, GPIO_PIN_RESET);
	}
	// 2.
	if ((HAL_GPIO_ReadPin(EN, EN_NUM) == GPIO_PIN_SET) && (HAL_GPIO_ReadPin(PG, PG_NUM) == GPIO_PIN_RESET)) {
		HAL_GPIO_WritePin(EN, EN_NUM, GPIO_PIN_RESET);
	}
	// 3.
	FEB_TPS2482_pullLowIfOutOfBounds(hi2c, DEV_ADDR, EN, EN_NUM, VMAX, VMIN, 0x01); // shunt voltage
	FEB_TPS2482_pullLowIfOutOfBounds(hi2c, DEV_ADDR, EN, EN_NUM, VMAX, VMIN, 0x02); // bus voltage
	FEB_TPS2482_pullLowIfOutOfBounds(hi2c, DEV_ADDR, EN, EN_NUM, PMAX, PMIN, 0x03); // power
	FEB_TPS2482_pullLowIfOutOfBounds(hi2c, DEV_ADDR, EN, EN_NUM, IMAX, IMIN, 0x04); // current

}

//TODO: Modify to accomodate other sensing types, right now its only good for shunt voltage
void FEB_TPS2482_pullLowIfOutOfBounds(I2C_HandleTypeDef * hi2c, uint8_t DEV_ADDR, GPIO_TypeDef * EN, uint16_t EN_NUM, float MAX, float MIN,
		uint8_t REG) {
	uint8_t buf[12];
	buf[0] = REG;
	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(hi2c, DEV_ADDR, buf, 1, 100);
	if (ret != HAL_OK) {
		HAL_GPIO_WritePin(EN, EN_NUM, GPIO_PIN_RESET); // pull EN low
	} else {
		ret = HAL_I2C_Master_Receive(hi2c, DEV_ADDR, buf, 2, 100);
		if (ret != HAL_OK) {
			HAL_GPIO_WritePin(EN, EN_NUM, GPIO_PIN_RESET); // pull EN low
		} else {
			int val = ((int16_t)buf[0] << 4) | (buf[1] >> 4); // combine the 2 bytes
			val = val - 1;
			val |= 0xF000; // subtract 1 and take complement
			float parsed = val * 0.0000025;// convert to decimal and multiply by 2.5uV
			if (parsed > MAX || parsed < MIN) {
				HAL_GPIO_WritePin(EN, EN_NUM, GPIO_PIN_RESET);
			}
		}
	}
}

//Brief: Uses I2C to pull current value from a device on the bus
//Param: hi2c Pointer to a I2C_HandleTypeDef structure that contains
//                the configuration information for the specified I2C.
//Param: DEV_ADDR which points to which device on the bus you want to poll
float FEB_TPS2482_PollBusCurrent(I2C_HandleTypeDef * hi2c, uint8_t DEV_ADDR){
	//Buffer to store data;
	uint8_t buf[12];
	buf[0] = 4; //4 is the register that stores the current value
	float returnVal = -1; //Set return val default to -1 as an "error". Not that great since we can actually have negative current
	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(hi2c, DEV_ADDR, buf, 1, 100);
	if(ret == HAL_OK){
		ret = HAL_I2C_Master_Receive(hi2c, DEV_ADDR, buf, 2,100);
		if(ret == HAL_OK){
			int16_t val = (buf[0]<<8) | buf[1]; //Not sure if little endian or not, needs testing!
			returnVal = val * 0.002; // LSB-weight = 2mA/bit
		}
	}
//	HAL_I2C_IsDeviceReady(hi2c, DEV_ADDR, 1, 100);
	if(ret == HAL_ERROR) return -2.0;
	if(ret == HAL_BUSY) return -3.0;
	if(ret == HAL_TIMEOUT) return -4.0;

	return returnVal;
}

void FEB_TPS2482_Poll_Currents(){
	current_reading = FEB_TPS2482_PollBusCurrent(&hi2c1,LV_ADDR+1);
	ex_current_reading = FEB_TPS2482_PollBusCurrent(&hi2c1,EX_ADDR+1);
	cp_current_reading = FEB_TPS2482_PollBusCurrent(&hi2c1,CP_ADDR+1);
}









