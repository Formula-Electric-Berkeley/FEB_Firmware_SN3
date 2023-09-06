# Battery Management System
The BMS (Battery Management System) is responsible for regulating the accumulator.

# Table of Contents
- [Battery Management System](#battery-management-system)
- [Table of Contents](#table-of-contents)
- [1. States](#1-states)
  - [1.1 Standard](#11-standard)
  - [1.2 Debug](#12-debug)
  - [1.3 Charge](#13-charge)
  - [1.4 Discharge](#14-discharge)
- [2. UART Debug](#2-uart-debug)
  - [2.1 Message ID](#21-message-id)
  - [2.2 Cell Voltage](#22-cell-voltage)
  - [2.3 Cell Temperature](#23-cell-temperature)

# 1. States
## 1.1 Standard
Standard operation of BMS. Intended state for driving. "Pre-charge" relay opens at 90% threshold.

## 1.2 Debug
BMS transmits relevant data via UART messages. Data is interpreted and displayed by the "BMS_Serial_Monitor" project. Intended state for testing and debugging the car.

## 1.3 Charge
BMS regulates battery charging via an ELCON charger. BMS communicates with ELCON charger via CAN messages. "Pre-charge" relay is closed during operation.

## 1.4 Discharge
Passive cell balancing of battery cells to a target voltage (lowest cell voltage). Cells are discharged through a resistor.

# 2. UART Debug
## 2.1 Message ID
<table>
  <tr>
    <th>Message</th>
    <th>ID</th> 
  </tr>
  <tr>
    <td>Cell Voltage</td>
    <td>0b0000</td> 
  </tr>
  <tr>
    <td>Cell Temperature</td>
    <td>0b0001</td> 
  </tr>
  <tr>
    <td>Disabled Temperature Sensors</td>
    <td>0b0010</td> 
  </tr>
  <tr>
    <td>BMS State</td>
    <td>0b0011</td> 
  </tr>
  <tr>
    <td>Relay State</td>
    <td>0b0100</td> 
  </tr>
  <tr>
    <td>Charge Data</td>
    <td>0b0101</td> 
  </tr>
  <tr>
    <td>Discharge Data</td>
    <td>0b0110</td> 
  </tr>
  <tr>
    <td>IVT Data</td>
    <td>0b0111</td> 
  </tr>
</table>

## 2.2 Cell Voltage
<table>
  <tr>
    <th>Message</th>
    <th>Bits Per Message</th>
  </tr>
  <tr>
    <td>Bank [0, 6]</td>
    <td>3</td> 
  </tr>
  <tr>
    <td>Cells [0, 19]</td>
    <td>5</td> 
  </tr>
</table>

Message Format: [Cell Voltage Message ID] [Bank ID][Cell ID] [V0] ... [V19]

## 2.3 Cell Temperature
<table>
  <tr>
    <th>Message</th>
    <th>Bits Per Message</th>
  </tr>
  <tr>
    <td>Bank [0, 6]</td>
    <td>3</td> 
  </tr>
  <tr>
    <td>Cells [0, 19]</td>
    <td>5</td> 
  </tr>
</table>

Message Format: [Cell Temperature Message ID] [Bank ID][Cell ID] [T0] ... [T19]