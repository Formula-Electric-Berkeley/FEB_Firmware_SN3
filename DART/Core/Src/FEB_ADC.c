//// ********************************** Includes & External **********************************
//#include "FEB_ADC.h"
//
////extern ADC_ChannelConfTypeDef sConfig;
//extern ADC_HandleTypeDef hadc;
//ADC_ChannelConfTypeDef sConfig = {0};
//// ********************************** Initialize **********************************
////void FEB_ADC_Init(void){
////	HAL_ADC_Start(&hadc);
////}
//
//// ********************************** ADC Channel Configuration **********************
//
//void ADC_Select_CH(uint32_t channel){
//
//	sConfig.Channel = channel;
//	sConfig.Rank = 1;
//	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
//	{
//		Error_Handler();
//	}
//}
//
//// ********************************** Read Tachometer **********************************
//
//uint32_t FEB_Read_Tachometer(uint32_t channel){
//	//ADC_Select_CH(channel);
//	sConfig.Channel = channel;
//		sConfig.Rank = 1;
//		sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//		if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
//		{
//			Error_Handler();
//		}
//
//	HAL_ADC_Start(&hadc);
//	HAL_ADC_PollForConversion(&hadc, 1000);
//	uint32_t val = HAL_ADC_GetValue(&hadc);
//	HAL_ADC_Stop(&hadc);
//	return val - 3300;
//}
//
//uint32_t FEB_Read_Tachometer_Fan_1() {
//	return FEB_Read_Tachometer(ADC_CHANNEL_6);
//}
//
//uint32_t FEB_Read_Tachometer_Fan_2() {
//	return FEB_Read_Tachometer(ADC_CHANNEL_5);
//}
//
//uint32_t FEB_Read_Tachometer_Fan_3() {
//	return FEB_Read_Tachometer(ADC_CHANNEL_4);
//}
//
//uint32_t FEB_Read_Tachometer_Fan_4() {
//	return FEB_Read_Tachometer(ADC_CHANNEL_1);
//}
//
//uint32_t FEB_Read_Tachometer_Fan_5() {
//	return FEB_Read_Tachometer(ADC_CHANNEL_0);
//}
