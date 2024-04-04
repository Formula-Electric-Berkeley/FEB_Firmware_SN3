#ifndef INC_FEB_STRAIN_GAUGE_H_
#define INC_FEB_STRAIN_GAUGE_H_

#include "stm32f4xx_hal.h"

uint8_t ReadStrainGaugeData(void);
void FEB_SPI_Read_Thermocouples(SPI_HandleTypeDef *hspi);

#endif /* INC_FEB_STRAIN_GAUGE_H_ */
