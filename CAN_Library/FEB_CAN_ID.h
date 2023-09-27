#ifndef INC_FEB_CAN_ID_H_
#define INC_FEB_CAN_ID_H_

// **************************************** Static CAN IDs ****************************************

// Charger
#define FEB_CAN_ID_CHARGER_BMS 0x1806e5f4
#define FEB_CAN_ID_CHARGER_CCS 0x18ff50e5

// IVT
#define FEB_CAN_ID_IVT_CURRENT 0x521
#define FEB_CAN_ID_IVT_VOLTAGE_1 0x522
#define FEB_CAN_ID_IVT_VOLTAGE_2 0x523
#define FEB_CAN_ID_IVT_VOLTAGE_3 0x524

// **************************************** Dynamic CAN IDs ****************************************

// Global
#define FEB_CAN_ID_SHUTDOWN 0x0

// APPS
#define FEB_CAN_ID_APPS_TORQUE 0x1

// BMS
#define FEB_CAN_ID_BMS_TEMPERATURE 0x2
#define FEB_CAN_ID_BMS_VOLTAGE 0x3

#endif /* INC_FEB_CAN_ID_H_ */
