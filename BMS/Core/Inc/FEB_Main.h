#ifndef INC_FEB_MAIN_H_
#define INC_FEB_MAIN_H_

void FEB_Main_Setup(void);
void FEB_SM_UART_Transmit(void);

// Tasks
void FEB_Main_Task1_VT(void);
void FEB_Main_Task2_State(void);
void FEB_Main_Task3_Charge(void);
void FEB_Main_Task4_Balance(void);
void FEB_Main_Task5_IVT(void);
void FEB_Main_Task6_CAN(void);
#endif /* INC_FEB_MAIN_H_ */
