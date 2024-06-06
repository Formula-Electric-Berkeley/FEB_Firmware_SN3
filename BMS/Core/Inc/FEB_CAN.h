#ifndef INC_FEB_CAN_H_
#define INC_FEB_CAN_H_

#include <stdbool.h>

bool FEB_CAN_Init(void);
bool get_inverter_status();
void FEB_CAN_Transmit(void);

#endif /* INC_FEB_CAN_H_ */
