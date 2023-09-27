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
  - [2.4 Disabled Temperature Sensors](#24-disabled-temperature-sensors)
  - [2.5 BMS State](#25-bms-state)
  - [2.6 Relay State](#26-relay-state)
  - [2.7 Shutdown State](#27-shutdown-state)
  - [2.8 Charge Data](#28-charge-data)
  - [2.9 Cell Balance](#29-cell-balance)
  - [2.10 IVT Data](#210-ivt-data)
- [3. Shutdown](#3-shutdown)
  - [3.1 Shutdown Triggers](#31-shutdown-triggers)
  - [3.2 Shutdown Control Loop](#32-shutdown-control-loop)

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
    <td>Shutdown State</td>
    <td>0b0101</td>
  </tr>
  <tr>
    <td>Charge Data</td>
    <td>0b0110</td>
  </tr>
  <tr>
    <td>Cell Balance</td>
    <td>0b0111</td> 
  </tr>
  <tr>
    <td>IVT Data</td>
    <td>0b1000</td> 
  </tr>
</table>

## 2.2 Cell Voltage
<table>
  <tr>
    <th>Message</th>
    <th>Value</th>
    <th>Type</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>Bank</td>
    <td>[0, 6]</td>
    <td>uint8_t</td>
    <td>-</td>
  </tr>
  <tr>
    <td>Voltage</td>
    <td>-</td>
    <td>uint16_t</td>
    <td>100uV</td>
  </tr>
</table>
Message Format: [Cell Voltage Message ID] [Bank ID] [v0] ... [v19]

## 2.3 Cell Temperature
<table>
  <tr>
    <th>Message</th>
    <th>Value</th>
    <th>Type</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>Bank</td>
    <td>[0, 6]</td>
    <td>uint8_t</td>
    <td>-</td>
  </tr>
  <tr>
    <td>Temperature</td>
    <td>-</td>
    <td>int16_t</td>
    <td>100mC</td>
  </tr>
</table>
Message Format: [Cell Temperature Message ID] [Bank ID] [t0] ... [t19]

## 2.4 Disabled Temperature Sensors

## 2.5 BMS State
<table>
  <tr>
    <th>Message</th>
    <th>Value</th>
    <th>Type</th>
  </tr>
  <tr>
    <td>Debug Balance</td>
    <td>[0 (Enabled), 1 (Disabled)]</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>Debug Charge</td>
    <td>[0 (Enabled), 1 (Disabled)]</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>Debug Drive</td>
    <td>[0 (Enabled), 1 (Disabled)]</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>Debug Precharge</td>
    <td>[0 (Enabled), 1 (Disabled)]</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>Debug Shutdown</td>
    <td>[0 (Enabled), 1 (Disabled)]</td>
    <td>uint8_t</td>
  </tr>
</table>
Message Format: [BMS State Message ID] [Balance] [Charge] [Drive] [Precharge] [Shutdown]

## 2.6 Relay State
<table>
  <tr>
    <th>Message</th>
    <th>Value</th>
    <th>Type</th>
  </tr>
  <tr>
    <td>Pre-charge Relay</td>
    <td>[0 (closed), 1 (Open)]</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>Air+ Relay</td>
    <td>[0 (Closed), 1 (Open)]</td>
    <td>uint8_t</td>
  </tr>
</table>
Message Format: [Relay State Message ID] [Pre-charge Relay] [Air+ Relay]

## 2.7 Shutdown State
<table>
  <tr>
    <th>Message</th>
    <th>Value</th>
    <th>Type</th>
  </tr>
  <tr>
    <td>BMS</td>
    <td>[0 (closed), 1 (Open)]</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>TSMS</td>
    <td>[0 (Closed), 1 (Open)]</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>IMD</td>
    <td>[0 (Closed), 1 (Open)]</td>
    <td>uint8_t</td>
  </tr>
</table>
Message Format: [Shutdown State Message ID] [BMS] [TSMS] [IMD]

## 2.8 Charge Data
<table>
  <tr>
    <th>Message</th>
    <th>Value</th>
    <th>Type</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>State</td>
    <td>[0 (On), 1 (Off)]</td>
    <td>-</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>Max Charger Current</td>
    <td>-</td>
    <td>100mA</td>
    <td>uint16_t</td>
  </tr>
  <tr>
    <td>Max Charger Voltage</td>
    <td>-</td>
    <td>100mV</td>
    <td>uint16_t</td>
  </tr>
  <tr>
    <td>Charger Current</td>
    <td>-</td>
    <td>100mA</td>
    <td>uint16_t</td>
  </tr>
  <tr>
    <td>Charger Voltage</td>
    <td>-</td>
    <td>100mV</td>
    <td>uint16_t</td>
  </tr>
  <tr>
    <td>Status Flags</td>
    <td>-</td>
    <td>-</td>
    <td>uint8_t</td>
  </tr>
</table>
Message Format: [Charger Data Message ID] [State] [Max Current] [Max Voltage] [Current] [Voltage] [Status Flags]

## 2.9 Cell Balance

## 2.10 IVT Data
<table>
  <tr>
    <th>Message</th>
    <th>Type</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>U1</td>
    <td>int32_t</td>
    <td>mV</td>
  </tr>
  <tr>
    <td>U2</td>
    <td>int32_t</td>
    <td>mV</td>
  </tr>
  <tr>
    <td>U3</td>
    <td>int32_t</td>
    <td>mV</td>
  </tr>
  <tr>
    <td>I1</td>
    <td>int32_t</td>
    <td>mA</td>
  </tr>
</table>
Message Format: [IVT Data Message ID] [U1] [U2] [U3] [I1]

# 3. Shutdown
## 3.1 Shutdown Triggers
* Cell Over/Under Voltage
* Cell Over/Under Temperature
* Charger Invalid Status Flag
* IVT Under/Over Current
* IMD Shutdown
* TSMS Shutdown
* Receive CAN Shutdown Message 

## 3.2 Shutdown Control Loop
* Open BMS Shutdown
* Open Pre-charge Relay
* Open Air+ Relay
* Stop Charge
* Stop Discharge (Cell Balance)
* Transmit CAN Shutdown Message