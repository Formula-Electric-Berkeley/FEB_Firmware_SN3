# Battery Management System
The BMS (battery management system) is responsible for regulating the high voltage battery.

# Table of Contents
- [Battery Management System](#battery-management-system)
- [Table of Contents](#table-of-contents)
- [1. States](#1-states)
  - [1.1 Standard](#11-standard)
  - [1.2 Debug](#12-debug)
  - [1.3 Charge](#13-charge)
  - [1.4 Discharge](#14-discharge)
- [2. Shutdown](#2-shutdown)
  - [2.1 Shutdown Triggers](#21-shutdown-triggers)
  - [2.2 Shutdown Control Loop](#22-shutdown-control-loop)

# 1. States
## 1.1 Standard
Standard operation of BMS. Intended state for driving. "Pre-charge" relay opens at 90% threshold.

## 1.2 Debug
BMS transmits relevant data via UART messages. Data is interpreted and displayed by the "BMS_Serial_Monitor" project. Intended state for testing and debugging the car.

## 1.3 Charge
BMS regulates battery charging via an ELCON charger. BMS communicates with ELCON charger via CAN messages. "Pre-charge" relay is closed during operation.

## 1.4 Discharge
Passive cell balancing of battery cells to a target voltage (lowest cell voltage). Cells are discharged through a resistor.

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