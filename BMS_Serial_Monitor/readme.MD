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
- [4. Monitor States](#4-monitor-states)

# 1. Data Logged
## 1.1 Raw Data
* Battery Voltage (20 x 7)
* Battery Temperature (20 x 7)
* Disabled Temperature Sensors
* BMS State
  * Debug Standard
  * Debug Charge
  * Debug Discharge
  * Debug Shutdown
* Relays
  * Pre-charge Relay (0 = Closed, 1 = Open)
  * Air+ Relay (0 = Closed, 1 = Open)
* Shutdown
  * BMS (0 = Closed, 1 = Open)
  * TSMS (0 = Closed, 1 = Open)
  * IMD (0 = Closed, 1 = Open)
* Charge Data
  * Charger State (0 = On, 1 = Off)
  * Max Charger Current
  * Max Charger Voltage
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
* Voltage: Mean, Min, Max, Std, Range, Total
* Temperature: Mean, Min, Max, Std, Range
* Discharge Data: No. Cells Discharging

# 2. Usage Instructions
1. Run "serial_monitor.py". Active COM ports are printed. Terminate execution.
2. Set serial COM port in "constants.py".
3. Run "serial_monitor.py"

# 3. Python Libraries
1. serial 
2. serial-tool
3. tkinter

# 4. Monitor States
<table>
  <tr>
    <th>State</th>
    <th>Command</th>
    <th>About</th>
  </tr>
  <tr>
    <td>Standard</td>
    <td>S</td> 
    <td>-</td>
  </tr>
  <tr>
    <td>Range</td>
    <td>R</td> 
    <td>Highlight out of range values (e.g., voltage, temperature)</td>
  </tr>
  <tr>
    <td>Discharge</td>
    <td>D</td> 
    <td>Highlight discharging cells</td>
  </tr>
</table>