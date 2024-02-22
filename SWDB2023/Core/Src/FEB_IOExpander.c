// ********************************** Includes & External **********************************

#include "FEB_IOExpander.h"

extern uint8_t data2;

// ********************************** Variables **********************************

//static const uint8_t IOEXP1_ADDR = 0b0100000 << 1; // Use 8-bit address
static const uint8_t IOEXP2_ADDR = 0b0100001 << 1;

char buf[128];
int buf_len;
HAL_StatusTypeDef ret;



// ********************************** Functions **********************************

FEB_IOExpander_Read(){
	  ret = HAL_I2C_Master_Receive(&hi2c1, IOEXP2_ADDR, &data2, 1, HAL_MAX_DELAY);
	  if ( ret != HAL_OK ) {
		  buf_len = sprintf((char*)buf, "IO_2 Error\r\n");
	  }

	  //check SN2 code for old read functions
}
