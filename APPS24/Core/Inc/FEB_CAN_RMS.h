
#ifndef INC_FEB_CAN_RMS_H_
#define INC_FEB_CAN_RMS_H_

// ********************************** Includes **********************************

#include "FEB_CAN_ID.h"
#include "FEB_Const.h"
#include <stdint.h>


// ********************************** Functions **********************************
void FEB_CAN_RMS_Setup(void);
void FEB_CAN_RMS_Process(void);
void FEB_CAN_RMS_Disable(void);
uint16_t FEB_CAN_RMS_getMaxTorque(void);
void FEB_CAN_RMS_Torque(void);
void FEB_CAN_RMS_updateTorque(void);
void FEB_CAN_RMS_torqueTransmit(void);
void FEB_CAN_RMS_Init(void);
uint8_t FEB_CAN_RMS_Filter_Config(CAN_HandleTypeDef* hcan, uint8_t FIFO_assignment, uint8_t filter_bank);
void FEB_CAN_RMS_Store_Msg(AddressIdType RxId, uint8_t *RxData);




#endif /* INC_FEB_CAN_RMS_H_ */
