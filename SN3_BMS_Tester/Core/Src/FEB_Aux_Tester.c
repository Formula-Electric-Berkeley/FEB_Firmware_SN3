// ******************************** Includes and External ********************************

#include "FEB_Aux_Tester.h"

extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_adc1;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

// ******************************** Global Variables ********************************

int CURR_CELL = 1;
uint16_t READINGS[20];

volatile uint16_t adcResultsDMA[3]; //Array to store values read by ADC
const int adcChannelCount = sizeof (adcResultsDMA) / sizeof (adcResultsDMA[0]); //Size of array
volatile int adcConversionComplete = 0;

const uint8_t AD5243_ADDRESS = 0x2F << 1; //0101 1110. Last bit: W bit.

// ******************************** Functions ********************************

void FEB_Aux_Tester_Test_Aux(void) {
	FEB_Aux_Tester_Init();
	FEB_Aux_Tester_Test_Cell_Voltages(); //Test cell voltages
	FEB_Aux_Tester_Test_Cell_Temps(); //Test cell temperatures
}

void FEB_Aux_Tester_Init(void) {
	//All CS High
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); //CS1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); //CS2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //CS3
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); //CS4
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); //CS5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); //CS6
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); //CS7
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //CS8
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET); //CS9
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); //CS10
}

// ******************************** Test Voltages ********************************

void FEB_Aux_Tester_Test_Cell_Voltages(void) {
	CURR_CELL = 0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); //CS1 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); //CS1 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //CS2 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); //CS2 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); //CS3 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //CS3 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); //CS4 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); //CS4 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); //CS5 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); //CS5 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); //CS6 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); //CS6 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); //CS7 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); //CS7 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //CS8 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); //CS8 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET); //CS9 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET); //CS9 High
	CURR_CELL++;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //CS10 Low
	FEB_Aux_Tester_Test_Cell_Voltage(); //Test Cell Voltage
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); //CS10 High
}

void FEB_Aux_Tester_Test_Cell_Voltage(void) {
	for (int i = 0; i < 256; i++) {
		HAL_SPI_Transmit(&hspi2, (uint8_t*)i, 1, 100);
		FEB_Aux_Tester_Read_Outputs(); //Read outputs: voltages 1-20, temps 1-20
		FEB_Aux_Tester_Validate_Voltage_Reading(); //Validate readings
	}
}

void FEB_Aux_Tester_Read_Outputs(void) {
	//Cycle through MUX Channels 0-5
	for (int i = 0; i <= 5; i++) {
		FEB_Aux_Tester_selectMuxChannel(i); //Select MUX Channel based on i
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcResultsDMA, adcChannelCount); // Start the ADC conversion
		while (adcConversionComplete == 0) {
			// Wait for the conversion to complete
		}
		adcConversionComplete = 0; //Reset variable
		HAL_ADC_Stop_DMA(&hadc1); //Stop Conversion
	    READINGS[i] = adcResultsDMA[0]; //Out 1
	    READINGS[i + 7] = adcResultsDMA[1]; //Out 2
	    READINGS[i + 14] = adcResultsDMA[2]; //Out 3
	}
	FEB_Aux_Tester_selectMuxChannel(6); //Select MUX Channel 6
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcResultsDMA, adcChannelCount); // Start the ADC conversion
	while (adcConversionComplete == 0) {
		// Wait for the conversion to complete
	}
	adcConversionComplete = 0; //Reset adcConversionComplete variable
	HAL_ADC_Stop_DMA(&hadc1); //Stop Conversion
    READINGS[6] = adcResultsDMA[0]; //Out 1
    READINGS[13] = adcResultsDMA[1]; //Out 2
}

void FEB_Aux_Tester_Validate_Voltage_Reading(void) {
	for (int i = 0; i < 19; i++) {
		//Somehow validate voltages
		//If wrong, output:
	}
}

// ******************************** Test Temperatures ********************************

void FEB_Aux_Tester_Test_Cell_Temps(void) {
	//Reset readings
	for (int i = 0; i < 19; i++) {
		READINGS[i] = 0;
	}
	for (int i = 0; i < 256; i++) {
		FEB_Aux_Tester_Set_Potentiometer_Resistance(i);
		FEB_Aux_Tester_Validate_Temp_Reading();
	}
}

void FEB_Aux_Tester_Set_Potentiometer_Resistance(uint8_t resistance) {
	uint8_t buf[2];
	buf[0] = 0x00; //MSB Low selects channel 1
	buf[1] = resistance;
	HAL_I2C_Master_Transmit(&hi2c1, AD5243_ADDRESS, buf, 2, 100); //Write value to potentiometer
}

void FEB_Aux_Tester_Validate_Temp_Reading(void) {
	for (int i = 0; i < 19; i++) {
		//Somehow validate temps
	}
}

// ******************************** Auxilary Functions ********************************

void FEB_Aux_Tester_selectMuxChannel(uint8_t channel) {
    // Ensure that the channel value is within the valid range (0-7)
    if (channel < 8) {
        // Set the select pins based on the binary encoding
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, (channel & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); //SEL1
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, (channel & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET); //SEL2
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (channel & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET); //SEL3
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	adcConversionComplete = 1;
}

