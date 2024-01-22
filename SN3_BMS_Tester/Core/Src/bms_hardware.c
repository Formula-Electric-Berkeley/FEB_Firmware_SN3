/*!
  ltc681x hardware library
@verbatim
  This library contains all of the hardware dependant functions used by the bms
  code
@endverbatim

Copyright 2018(c) Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.
 - Neither the name of Analog Devices, Inc. nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.
 - The use of this software may or may not infringe the patent rights
   of one or more patent holders.  This license does not release you
   from the requirement that you obtain separate licenses from these
   patent holders to use this software.
 - Use of the software either in source or binary form, must be run
   on or directly connected to an Analog Devices Inc. component.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright 2017 Linear Technology Corp. (LTC)
*/

// ********************************** Includes & External **********************************

#include "bms_hardware.h"

extern SPI_HandleTypeDef hspi2;

// ********************************** Functions **********************************

// ******************** Delay ********************

void delay_u(uint16_t micro) {
	HAL_Delay(1);
}

void delay_m(uint16_t milli) {
	HAL_Delay(milli);
}

// ******************** SPI ********************

void cs_low(uint8_t pin) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
}

void cs_high(uint8_t pin) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
}

void spi_write_array(uint8_t len, uint8_t data[]) {
	HAL_SPI_Transmit(&hspi2, data, len, 100);
}

void spi_write_read(uint8_t tx_Data[], uint8_t tx_len, uint8_t *rx_data, uint8_t rx_len) {
	for (uint8_t i = 0; i < tx_len; i++) {
		HAL_SPI_Transmit(&hspi2, &tx_Data[i], 1, 100);
	}

	for (uint8_t i = 0; i < rx_len; i++) {
		HAL_SPI_Receive(&hspi2, &rx_data[i], 1, 100);
	}
}

uint8_t spi_read_byte(uint8_t tx_dat) {
  uint8_t data;
  HAL_SPI_Receive(&hspi2, &data, 1, 100);
  return data;
}
