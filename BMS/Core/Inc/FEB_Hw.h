#ifndef INC_FEB_HW_H_
#define INC_FEB_HW_H_

typedef enum {
	FEB_HW_RELAY_OPEN,
	FEB_HW_RELAY_CLOSE
} FEB_Hw_Relay_t;

void FEB_Hw_Set_BMS_Shutdown_Relay(FEB_Hw_Relay_t state);
void FEB_Hw_Set_AIR_Plus_Relay(FEB_Hw_Relay_t state);
void FEB_Hw_Set_Precharge_Relay(FEB_Hw_Relay_t state);
FEB_Hw_Relay_t FEB_Hw_Get_BMS_Shutdown_Relay(void);
FEB_Hw_Relay_t FEB_Hw_Get_AIR_Plus_Relay(void);
FEB_Hw_Relay_t FEB_Hw_Get_Precharge_Relay(void);

FEB_Hw_Relay_t FEB_Hw_AIR_Minus_Sense(void);
FEB_Hw_Relay_t FEB_Hw_AIR_Plus_Sense(void);
FEB_Hw_Relay_t FEB_Hw_BMS_Shutdown_Sense(void);
FEB_Hw_Relay_t FEB_Hw_IMD_Shutdown_Sense(void);

#endif /* INC_FEB_HW_H_ */
