#ifndef INC_FEB_HW_H_
#define INC_FEB_HW_H_

typedef enum {
	FEB_HW_RELAY_CLOSE,
	FEB_HW_RELAY_OPEN
} FEB_Hw_Relay_t;

void FEB_Hw_Shutdown_Relay(FEB_Hw_Relay_t state);
void FEB_Hw_AIR_Plus_Relay(FEB_Hw_Relay_t state);
void FEB_Hw_Precharge_Relay(FEB_Hw_Relay_t state);

FEB_Hw_Relay_t FEB_Hw_Read_Shutdown_Circuit(void);

#endif /* INC_FEB_HW_H_ */
