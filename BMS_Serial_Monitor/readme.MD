# BMS Serial Monitor
The BMS Serial Monitor is a GUI for data from the BMS (Battery Management System).

# Table of Contents
- [BMS Serial Monitor](#bms-serial-monitor)
- [Table of Contents](#table-of-contents)
- [1. Data Logged](#1-data-logged)
  - [1.1 Raw Data](#11-raw-data)
  - [1.2 Processed Data](#12-processed-data)
- [2. Usage Instructions](#2-usage-instructions)
- [3. Python Libraries](#3-python-libraries)

# 1. Data Logged
## 1.1 Raw Data
* Battery Voltage (17 x 8)
* Battery Temperature (17 x 8)
* Disabled Temperature Sensors
* BMS State
  * Debug Standard
  * Debug Charge
  * Debug Discharge
  * Debug Shutdown
* Relays
  * Shutdown Circuit - AIRs Relay (0 = Closed, 1 = Open)
  * Precharge Relay (0 = Closed, 1 = Open)
* Charge Data
  * Charger State (0 = On, 1 = Off)
  * Charger Current
  * Charger Voltage
  * Charger Status Flags (0 = No Error, 1 = Error)
    * Hardware Failure
    * Temperature Protection
    * Input Voltage
    * Starting State
    * Communication State
* Discharge Data
  * Target Voltage
  * Cells Discharging
* IVT Data
  * Voltage U1
  * Voltage U2
  * Voltage U3
  * Current I1

## 1.2 Processed Data
* Voltage: Mean, Min, Max, Std, Total
* Temperature: Mean, Min, Max, Std
* Discharge Data: No. Cells Discharging

# 2. Usage Instructions
1. Run "serial_monitor.py". Active COM ports are printed. Terminate execution.
2. Set serial COM port in "constants.py".
3. Run "serial_monitor.py"

# 3. Python Libraries
1. serial 
2. serial-tool
3. tkinter