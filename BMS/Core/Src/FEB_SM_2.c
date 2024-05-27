//// ******************************** Includes & External ********************************
//
//#include <FEB_SM_2.h>
//
//extern CAN_HandleTypeDef hcan1;
//extern CAN_TxHeaderTypeDef FEB_CAN_Tx_Header;
//extern uint8_t FEB_CAN_Tx_Data[8];
//extern uint32_t FEB_CAN_Tx_Mailbox;
//
//// ******************************** Static Function Declaration ********************************
//
//static void balance(void);
//static void balance_to_precharge(void);
//static void balance_to_shutdown(void);
//
//static void charge(void);
//static void charge_to_precharge(void);
//static void charge_to_shutdown(void);
//
//static void precharge(void);
//static void precharge_to_drive(void);
//
//static void shutdown(void);
//
//// ******************************** Static Variables ********************************
//
//static FEB_SM_ST_t current_state;
//
//// List of transitions pairs (s, a, s')
//#define NUM_TRANSITIONS 9
//static FEB_SM_Transition_t transitions[NUM_TRANSITIONS] = {
//	{FEB_SM_ST_BALANCE,	balance_to_precharge, FEB_SM_ST_PRECHARGE},
//	{FEB_SM_ST_BALANCE, balance_to_shutdown, FEB_SM_ST_SHUTDOWN},
//	{FEB_SM_ST_CHARGE, charge_to_precharge, FEB_SM_ST_PRECHARGE},
//	{FEB_SM_ST_CHARGE, charge_to_shutdown, FEB_SM_ST_SHUTDOWN},
//	{FEB_SM_ST_DRIVE, shutdown, FEB_SM_ST_SHUTDOWN},
//	{FEB_SM_ST_PRECHARGE, balance, FEB_SM_ST_BALANCE},
//	{FEB_SM_ST_PRECHARGE, charge, FEB_SM_ST_CHARGE},
//	{FEB_SM_ST_PRECHARGE, precharge_to_drive, FEB_SM_ST_DRIVE},
//	{FEB_SM_ST_PRECHARGE, shutdown, FEB_SM_ST_SHUTDOWN},
//};
//
//// ******************************** Public Interface ********************************
//
//void FEB_SM_Init(void) {
//	current_state = FEB_SM_ST_PRECHARGE;
//	FEB_Relay_Shutdown(FEB_RELAY_ST_CLOSE);
//	FEB_Relay_AIR_Plus(FEB_RELAY_ST_OPEN);
//	FEB_Relay_Precharge(FEB_RELAY_ST_CLOSE);
//}
//
//FEB_SM_ST_t FEB_SM_Get_Current_State(void) {
//	return current_state;
//}
//
//// Return 0 if state switch succeeds, 1 otherwise.
//uint8_t FEB_SM_Set_Current_State(FEB_SM_ST_t next_state) {
//	for (uint8_t i = 0; i < NUM_TRANSITIONS; i++) {
//		if (transitions[i].curr_state == current_state &&
//			transitions[i].next_state == next_state) {
//			transitions[i].action();
//			return 0;
//		}
//	}
//	return 1;
//}
//
//void FEB_SM_CAN_Transmit(void) {
//	// Initialize transmission header
//	FEB_CAN_Tx_Header.DLC = 2;
//	FEB_CAN_Tx_Header.StdId = FEB_CAN_ID_BMS_STATE;
//	FEB_CAN_Tx_Header.IDE = CAN_ID_STD;
//	FEB_CAN_Tx_Header.RTR = CAN_RTR_DATA;
//	FEB_CAN_Tx_Header.TransmitGlobalTime = DISABLE;
//
//	// BMS state
//	uint8_t bms_state;
//	switch (FEB_SM_Get_Current_State()) {
//		case FEB_SM_ST_PRECHARGE:
//			bms_state = 0;
//			break;
//		case FEB_SM_ST_CHARGE:
//			bms_state = 1;
//			break;
//		case FEB_SM_ST_BALANCE:
//			bms_state = 2;
//			break;
//		case FEB_SM_ST_DRIVE:
//			bms_state = 3;
//			break;
//		case FEB_SM_ST_SHUTDOWN:
//			bms_state = 4;
//			break;
//	}
//
//	// Relay state
//	uint8_t shutdown_relay = FEB_Relay_ST_Shutdown() == FEB_RELAY_ST_CLOSE ? 0 : 1;
//	uint8_t AIR_plus_relay = FEB_Relay_ST_AIR_Plus() == FEB_RELAY_ST_CLOSE ? 0 : 1;
//	uint8_t precharge_relay = FEB_Relay_ST_Precharge() == FEB_RELAY_ST_CLOSE ? 0 : 1;
//	uint8_t relay_state = (shutdown_relay << 2) + (AIR_plus_relay << 1) + precharge_relay;
//
//	// Configure CAN data
//	FEB_CAN_Tx_Data[0] = bms_state;
//	FEB_CAN_Tx_Data[1] = relay_state;
//
//	// Delay until mailbox available
//	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {}
//
//	// Add Tx data to mailbox
//	if (HAL_CAN_AddTxMessage(&hcan1, &FEB_CAN_Tx_Header, FEB_CAN_Tx_Data, &FEB_CAN_Tx_Mailbox) != HAL_OK) {
//		FEB_SM_Set_Current_State(FEB_SM_ST_SHUTDOWN);
//	}
//}
//
//// ******************************** Transition Functions ********************************
//
//static void balance(void) {
//	current_state = FEB_SM_ST_BALANCE;
//	FEB_Relay_AIR_Plus(FEB_RELAY_ST_OPEN);
//	FEB_Relay_Precharge(FEB_RELAY_ST_OPEN);
//}
//
//static void balance_to_precharge(void) {
//	FEB_LTC6811_Stop_Balance();
//	precharge();
//}
//
//static void balance_to_shutdown(void) {
//	FEB_LTC6811_Stop_Balance();
//	shutdown();
//}
//
//static void charge(void) {
//	current_state = FEB_SM_ST_CHARGE;
//	FEB_Relay_AIR_Plus(FEB_RELAY_ST_CLOSE);
//	FEB_Relay_Precharge(FEB_RELAY_ST_OPEN);
//}
//
//static void charge_to_precharge(void) {
//	// TODO: Stop charge
//	precharge();
//}
//
//static void charge_to_shutdown(void) {
//	// TODO: Stop charge
//	shutdown();
//}
//
//static void precharge(void) {
//	current_state = FEB_SM_ST_PRECHARGE;
//	FEB_Relay_Precharge(FEB_RELAY_ST_CLOSE);
//	FEB_Relay_AIR_Plus(FEB_RELAY_ST_OPEN);
//}
//
//static void precharge_to_drive(void) {
//	current_state = FEB_SM_ST_DRIVE;
//	FEB_Relay_AIR_Plus(FEB_RELAY_ST_CLOSE);
//	osDelay(500);
//	FEB_Relay_Precharge(FEB_RELAY_ST_OPEN);
//}
//
//static void shutdown(void) {
//	current_state = FEB_SM_ST_SHUTDOWN;
//	FEB_Relay_Shutdown(FEB_RELAY_ST_OPEN);
//	FEB_Relay_AIR_Plus(FEB_RELAY_ST_OPEN);
//	FEB_Relay_Precharge(FEB_RELAY_ST_OPEN);
//}
