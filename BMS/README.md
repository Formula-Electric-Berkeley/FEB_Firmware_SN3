# Battery Management System
The BMS (battery management system) is responsible for regulating the high voltage battery.

# Table of Contents
- [Battery Management System](#battery-management-system)
- [Table of Contents](#table-of-contents)
- [1. States](#1-states)
- [2. Shutdown](#2-shutdown)
  - [2.1 Shutdown Triggers](#21-shutdown-triggers)
  - [2.2 Shutdown Control Loop](#22-shutdown-control-loop)

# 1. States
![FEB - BMS State Machine](https://github.com/Formula-Electric-Berkeley/FEB_Firmware_SN3/assets/54029493/d737d390-af1d-4601-88f9-d46e4aa008c2)

# 2. Shutdown
## 2.1 Shutdown Triggers
* Cell Over/Under Voltage
* Cell Over/Under Temperature
* Charger Invalid Status Flag
* IVT Under/Over Current
* IMD Shutdown
* TSMS Shutdown
* Receive CAN Shutdown Message 

## 2.2 Shutdown Control Loop
* Open BMS Shutdown
* Open Pre-charge Relay
* Open Air+ Relay
* Stop Charge
* Stop Discharge (Cell Balance)
* Transmit CAN Shutdown Message
