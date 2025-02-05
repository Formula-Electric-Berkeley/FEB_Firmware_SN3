#ifndef INC_FEB_MAIN_H_
#define INC_FEB_MAIN_H_

void FEB_Main_Setup(void);
void FEB_SM_UART_Transmit(void);

// Tasks
void FEB_Task_ADBMS(void);
void FEB_Main_Task2_State(void);
void FEB_Task_Charge(void);
void FEB_Task_Balance(void);
void FEB_Task_IVT(void);
void FEB_Task_CAN(void);

#endif /* INC_FEB_MAIN_H_ */
