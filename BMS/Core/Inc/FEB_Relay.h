#ifndef INC_FEB_RELAY_H_
#define INC_FEB_RELAY_H_

// ******************************** Includes ********************************

#include <stdint.h>

// ******************************** Relay State  ********************************

typedef enum {
	FEB_RELAY_ST_CLOSE,
	FEB_RELAY_ST_OPEN
} FEB_Relay_ST_t;

// ******************************** Functions  ********************************

// Relay control
void FEB_Relay_Shutdown(FEB_Relay_ST_t relay_state);
void FEB_Relay_AIR_Plus(FEB_Relay_ST_t relay_state);
void FEB_Relay_Precharge(FEB_Relay_ST_t relay_state);

// Relay state
FEB_Relay_ST_t FEB_Relay_ST_Shutdown(void);
FEB_Relay_ST_t FEB_Relay_ST_AIR_Plus(void);
FEB_Relay_ST_t FEB_Relay_ST_Precharge(void);

#endif /* INC_FEB_RELAY_H_ */
