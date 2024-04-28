#include "FEB_main.h"
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim2;

volatile uint8_t gu8_MSGG[64] = {'\0'};


char uart_tx_buffer[5];
uint32_t tach_val;


void FEB_Init(void){
	//FEB_CAN_Init();
	//FEB_ADC_Init();
	FEB_Fan_Init();
	//HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	//HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);

}

void FEB_Main_Loop(void){


		FEB_Fan_3_Speed_Set(255 * 0.2);
		FEB_Fan_5_Speed_Set(255 * 0.3);


		//start fan 3 reading
		 sprintf(gu8_MSGG, "Starting 3");
		 HAL_UART_Transmit(&huart2, gu8_MSGG, sizeof(gu8_MSGG), 100);

		 HAL_Delay(1000);

	  if (HAL_TIM_Base_Start_IT(&htim14) != HAL_OK)
		  		    {
		  		      /* Starting Error */
		  		      Error_Handler();
		  		    }

	  if (HAL_TIM_IC_Start_IT(&htim14, TIM_CHANNEL_1) != HAL_OK)
	  {
	 	      /* Starting Error */
	 	      Error_Handler();
	 	    }

	HAL_Delay(10000);

	  if (HAL_TIM_IC_Stop_IT(&htim14, TIM_CHANNEL_1) != HAL_OK)
	  {
	 	      /* Starting Error */
	 	      Error_Handler();
	 	    }

		  if (HAL_TIM_Base_Stop_IT(&htim14) != HAL_OK)
		    {
		      /* Starting Error */
		      Error_Handler();
		    }


		//end fan 3 reading

		 sprintf(gu8_MSGG, "Starting 5");
		 HAL_UART_Transmit(&huart2, gu8_MSGG, sizeof(gu8_MSGG), 100);
		HAL_Delay(1000);

		//start fan 5 reading

		if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
				  		    {
				  		      /* Starting Error */
				  		      Error_Handler();
				  		    }

		  if (HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK)
		  {
				  /* Starting Error */
				  Error_Handler();
				}

		HAL_Delay(10000);

		  if (HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1) != HAL_OK)
		  {
				  /* Starting Error */
				  Error_Handler();
				}

		  if (HAL_TIM_Base_Stop_IT(&htim2) != HAL_OK)
			{
			  /* Starting Error */
			  Error_Handler();
			}

			//end fan 5 reading

}
