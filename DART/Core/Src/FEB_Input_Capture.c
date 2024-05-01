//#include "FEB_Input_Capture.h"
//#include <stdint.h>
//
//#define IDLE   0
//#define DONE   1
//#define F_CLK  8000000UL
//
//extern huart2;
//
//volatile uint8_t gu8_State = IDLE;
//volatile uint8_t gu8_MSG[64] = {'\0'};
//volatile uint32_t gu32_T1 = 0;
//volatile uint32_t gu32_T2 = 0;
//volatile uint32_t gu32_Ticks = 0;
//volatile uint16_t gu16_TIM2_OVC = 0;
//volatile uint32_t gu32_Freq = 0;
//
//static uint32_t max_fan_frequency = 248; //248Hz
//
//uint16_t FEB_Current_Fan_Speed;
//
//void FEB_IC_Process(TIM_HandleTypeDef* htim){
//
//	switch(htim->Channel){
//
//		case HAL_TIM_ACTIVE_CHANNEL_1:
//
//			if(gu8_State == IDLE)
//			{
//				gu32_T1 = htim->Instance->CCR1;
//				gu16_TIM2_OVC = 0;
//				gu8_State = DONE;
//			//	sprintf(gu8_MSG, "loop2", gu32_Freq);
////			  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			}
//			else if(gu8_State == DONE)
//			{
//				//gu32_T2 = TIM2->CCR1;
//				gu32_T2 = htim->Instance->CCR1;
//				gu32_Ticks = (gu32_T2 + (gu16_TIM2_OVC * 256)) - gu32_T1;
//				gu32_Freq = (uint32_t)(F_CLK/gu32_Ticks);
//				if(gu32_Freq != 0)
//				{
//				sprintf(gu8_MSG, "%lu Hz\n\r", gu32_Freq);
//				  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//				}
//				if(gu32_Freq == 0)
////				{
////				  sprintf(gu8_MSG, "Failed = %lu Hz\n\r", gu32_Freq);
////				  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
////				}
//				gu8_State = IDLE;
//
////				FEB_Current_Fan_Speed = (gu32_Freq / max_fan_frequency) * 255;
//				FEB_Current_Fan_Speed = gu32_Freq;
//			}
//		break;
//
//		case HAL_TIM_ACTIVE_CHANNEL_2:
//
//			if(gu8_State == IDLE)
//			{
//				gu32_T1 = htim->Instance->CCR2;
//				gu16_TIM2_OVC = 0;
//				gu8_State = DONE;
//				//sprintf(gu8_MSG, "loop2", gu32_Freq);
//		//	  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			}
//			else if(gu8_State == DONE)
//			{
//				gu32_T2 = htim->Instance->CCR2;
//				gu32_Ticks = (gu32_T2 + (gu16_TIM2_OVC * 256)) - gu32_T1;
//				gu32_Freq = (uint32_t)(F_CLK/gu32_Ticks);
//			//				if(gu32_Freq != 0)
//			//				{
//				  sprintf(gu8_MSG, "loop1", gu32_Freq);
//				  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			//				}
//			//				if(gu32_Freq == 0)
//			//				{
//			//				  sprintf(gu8_MSG, "Failed = %lu Hz\n\r", gu32_Freq);
//			//				  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			//				}
//				gu8_State = IDLE;
//
//			//				FEB_Current_Fan_Speed = (gu32_Freq / max_fan_frequency) * 255;
//				FEB_Current_Fan_Speed = gu32_Freq;
//					}
//				break;
//
//		case HAL_TIM_ACTIVE_CHANNEL_3:
//
//			if(gu8_State == IDLE)
//			{
//				gu32_T1 = htim->Instance->CCR3;
//				gu16_TIM2_OVC = 0;
//				gu8_State = DONE;
//				sprintf(gu8_MSG, "loop2", gu32_Freq);
//			  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			}
//			else if(gu8_State == DONE)
//			{
//				gu32_T2 = htim->Instance->CCR3;
//				gu32_Ticks = (gu32_T2 + (gu16_TIM2_OVC * 256)) - gu32_T1;
//				gu32_Freq = (uint32_t)(F_CLK/gu32_Ticks);
//			//				if(gu32_Freq != 0)
//			//				{
////				  sprintf(gu8_MSG, "loop1", gu32_Freq);
//				  //HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			//				}
//			//				if(gu32_Freq == 0)
//			//				{
//			//				  sprintf(gu8_MSG, "Failed = %lu Hz\n\r", gu32_Freq);
//			//				  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			//				}
//				gu8_State = IDLE;
//
//			//				FEB_Current_Fan_Speed = (gu32_Freq / max_fan_frequency) * 255;
//				FEB_Current_Fan_Speed = gu32_Freq;
//					}
//				break;
//
//		case HAL_TIM_ACTIVE_CHANNEL_4:
//
//			if(gu8_State == IDLE)
//			{
//				gu32_T1 = htim->Instance->CCR4;
//				gu16_TIM2_OVC = 0;
//				gu8_State = DONE;
//				sprintf(gu8_MSG, "loop2", gu32_Freq);
//			  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			}
//			else if(gu8_State == DONE)
//			{
//				gu32_T2 = htim->Instance->CCR4;
//				gu32_Ticks = (gu32_T2 + (gu16_TIM2_OVC * 256)) - gu32_T1;
//				gu32_Freq = (uint32_t)(F_CLK/gu32_Ticks);
//			//				if(gu32_Freq != 0)
//			//				{
//				//  sprintf(gu8_MSG, "loop1", gu32_Freq);
//				 // HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			//				}
//			//				if(gu32_Freq == 0)
//			//				{
//			//				  sprintf(gu8_MSG, "Failed = %lu Hz\n\r", gu32_Freq);
//			//				  HAL_UART_Transmit(&huart2, gu8_MSG, sizeof(gu8_MSG), 100);
//			//				}
//				gu8_State = IDLE;
//
//			//				FEB_Current_Fan_Speed = (gu32_Freq / max_fan_frequency) * 255;
//				FEB_Current_Fan_Speed = gu32_Freq;
//					}
//				break;
//	}
//}
//
//
//void FEB_Period_Elapsed()
//{
//	gu16_TIM2_OVC++;
//}
