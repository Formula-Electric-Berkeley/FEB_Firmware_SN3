#include "FEB_main.h"
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim2;


volatile uint8_t gu8_MSGGGG[64] = {'\0'};

uint16_t f1 = 0;
uint16_t f2 = 0;
uint16_t f3 = 0;
uint16_t f4 = 0;
uint16_t f5 = 0;

char uart_tx_buffer[1024];

uint32_t tach_val;

void FEB_Error_Handler(){
	while(1){
		sprintf(gu8_MSGGGG, "HAL ERROR\n\r");
		HAL_UART_Transmit(&huart2, gu8_MSGGGG, sizeof(gu8_MSGGGG), 100);
	}
}

void FEB_Init(void){
	sprintf(gu8_MSGGGG, "Init");
	HAL_UART_Transmit(&huart2, gu8_MSGGGG, sizeof(gu8_MSGGGG), 10);
//	FEB_CAN_Init();
	FEB_Fan_Init();
}


void FEB_Main_Loop(void){
	sprintf(gu8_MSGGGG, "loop4");
	  HAL_UART_Transmit(&huart2, gu8_MSGGGG, sizeof(gu8_MSGGGG), 100);

//		f1 = FEB_Get_F1_Speed();
//		f2 = FEB_Get_F2_Speed();
		f3 = FEB_Get_F3_Speed();
		HAL_Delay(3000);
//		f4 = FEB_Get_F4_Speed();
		f5 = FEB_Get_F5_Speed();
		HAL_Delay(3000);

		sprintf(uart_tx_buffer, "%u %u %u %u %u\n", (unsigned int)f1, (unsigned int)f2, (unsigned int)f3, (unsigned int)f4, (unsigned int)f5);
		HAL_UART_Transmit(&huart2, (uint16_t*)uart_tx_buffer, sizeof(uart_tx_buffer), 100);

		HAL_Delay(1000);

//		//start fan 3 reading
//		 sprintf(gu8_MSGG, "Starting 3");
//		 HAL_UART_Transmit(&huart2, gu8_MSGG, sizeof(gu8_MSGG), 100);
//
//		 HAL_Delay(5000);
//
//	  if (HAL_TIM_Base_Start_IT(&htim14) != HAL_OK)
//		  		    {
//		  		      /* Starting Error */
//		  		      Error_Handler();
//		  		    }
//
//	  if (HAL_TIM_IC_Start_IT(&htim14, TIM_CHANNEL_1) != HAL_OK)
//	  {
//	 	      /* Starting Error */
//	 	      Error_Handler();
//	 	    }
//
//	HAL_Delay(100);
//
//	  if (HAL_TIM_IC_Stop_IT(&htim14, TIM_CHANNEL_1) != HAL_OK)
//	  {
//	 	      /* Starting Error */
//	 	      Error_Handler();
//	 	    }
//
//		  if (HAL_TIM_Base_Stop_IT(&htim14) != HAL_OK)
//		    {
//		      /* Starting Error */
//		      Error_Handler();
//		    }


		//end fan 3 reading

//		 sprintf(gu8_MSGG, "Starting 5");
//		 HAL_UART_Transmit(&huart2, gu8_MSGG, sizeof(gu8_MSGG), 100);
//		HAL_Delay(5000);
//
//		//start fan 5 reading
//
//		if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
//				  		    {
//				  		      /* Starting Error */
//				  		      Error_Handler();
//				  		    }
//
//		  if (HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK)
//		  {
//				  /* Starting Error */
//				  Error_Handler();
//				}
//
//		HAL_Delay(100);
//
//		  if (HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1) != HAL_OK)
//		  {
//				  /* Starting Error */
//				  Error_Handler();
//				}
//
//		  if (HAL_TIM_Base_Stop_IT(&htim2) != HAL_OK)
//			{
//			  /* Starting Error */
//			  Error_Handler();
//			}
//
//			//end fan 5 reading

}

//void FEB_Main_Loop(void){
//	//Fan speed set by CAN receive function
//	FEB_Update_Fan_Speed_Measurements();
//	FEB_CAN_Transmit;
//	HAL_Delay(100);
//}
