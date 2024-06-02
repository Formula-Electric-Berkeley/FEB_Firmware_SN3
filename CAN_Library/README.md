# CAN Library

- [CAN Library](#can-library)
- [1 Overview](#1-overview)
- [2 Update Instructions](#2-update-instructions)
- [3 CAN Message Documentation](#3-can-message-documentation)
  - [3.1 BMS](#31-bms)
    - [3.1.1 State](#311-state)
  - [3.2 APPS](#32-apps)
    - [3.2.1 Normalized Brake](#321-normalized-brake)
    - [3.2.2 RMS Param Msg](#322-rms-param-msg)
    - [3.2.3 RMS Command Msg](#323-rms-command-msg)
    - [3.2.4 BSPD](#324-bspd)
    - [3.2.5 Current](#325-current)
  - [3.3 LVPDB](#33-lvpdb)
  - [3.4 DCU](#34-dcu)
  - [3.5 ICS](#35-ics)
    - [3.5.1 Buttons \& Swtiches](#351-buttons--swtiches)
  - [3.6 DART](#36-dart)
    - [3.6.1 Measured Fan Speeds (Both DART1 and DART2)](#361-measured-fan-speeds-both-dart1-and-dart2)

# 1 Overview
STM32 Files:
* ```FEB_CAN_ID.h```: A header file that stores all CAN Message IDs.
* ```FEB_CAN.c```: A file that handles CAN library initialization and received message handling. Additional CAN files will build of this one. File contains template code for writing additional CAN files.
* ```FEB_CAN.h```: Header file for ```FEB_CAN.c```.

Other files:
* ```FEB_CAN_ID.csv```: A CSV file that stores names of CAN messages without assigned CAN IDs. This file is used to generate dynamic CAN IDs.
* ```FEB_CAN_Static_ID.csv```: A CSV file that sores names of CAN messages and their assigned CAN ID.
* ```generate.py```: This file uses data from ```FEB_CAN_ID.csv``` and ```FEB_CAN_Static_ID.csv``` to generate ```FEB_CAN_ID.h```.
* ```FEB_CAN_ID.py```: This file storse all CAN Message IDs. Used for the CAN Monitor.

# 2 Update Instructions
1. <b>Add CAN Message</b>: To add a CAN message with a static CAN ID, update ```FEB_CAN_Static_ID.csv``` file. To add a CAN message without a static CAN ID, update the ```FEB_CAN_ID.csv``` file.
2. <b>Generate</b>: Run ```generate.py``` script. This will update the ```FEB_CAN_ID.h``` header file.
3. <b>Documentation</b>: Document the new CAN message in this readme file.
4. <b>GitHub</b>: Push all changes to GitHub to ensure the CAN Library is up to date for everyone.

# 3 CAN Message Documentation

Note: Bytes and bits are 0-indexed.

## 3.1 BMS

### 3.1.1 State
<table>
  <tr>
    <th>Byte</th>
    <th>Value</th>
    <th>Datatype</th>
  </tr>
  <tr>
    <td>0</td>
    <td>BMS State</td>
    <td>uint8_t</td>
  </tr>
</table>

<table>
  <tr>
    <th>BMS State Value</th>
    <th>BMS State</th>
  </tr>
  <tr>
    <td>0</td>
    <td>Startup</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Standby</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Balance</td>
  </tr>
  <tr>
    <td>3</td>
    <td>Charge</td>
  </tr>
  <tr>
    <td>4</td>
    <td>Precharge</td>
  </tr>
  <tr>
    <td>5</td>
    <td>Drive-Standby</td>
  </tr>
  <tr>
    <td>6</td>
    <td>Drive</td>
  </tr>
  <tr>
    <td>7</td>
    <td>Drive-Regen</td>
  </tr>
  <tr>
    <td>8</td>
    <td>Fault</td>
  </tr>
</table>

## 3.2 APPS
### 3.2.1 Normalized Brake
<table>
    <tr>
        <th>Byte</th>
        <th>Value</th>
        <th>Datatype</th>
    </tr>
    <tr>
        <td>0-4</td>
        <td>Normalized Brake</td>
        <td>float</td>
    </tr>
</table>

### 3.2.2 RMS Param Msg
<table>
    <tr>
        <th>Byte</th>
        <th>Value</th>
        <th>Datatype</th>
    </tr>
    <tr>
        <td>0</td>
        <td>addr</td>
        <td>uint8_t</td>
    </tr>
    <tr>
        <td>2</td>
        <td>r/w command</td>
        <td>boolean</td>
    </tr>
    <tr>
        <td>3</td>
        <td>NA</td>
        <td>-</td>
    </tr>
    <tr>
        <td>4-5</td>
        <td>data</td>
        <td>uint8_t</td>
    </tr>
    <tr>
        <td>6-7</td>
        <td>NA</td>
        <td>-</td>
    </tr>
</table>

<table>
    <tr>
        <th>R/W Command Value</th>
        <th>State</th>
    </tr>
    <tr>
        <td>0</td>
        <td>Read</td>
    </tr>
    <tr>
        <td>1</td>
        <td>Write</td>
    </tr>
</table>

<table>
    <tr>
        <th>Addr Value </th>
        <th>State</th>
    </tr>
    <tr>
        <td>20</td>
        <td>Fault Clear</td>
    </tr>
    <tr>
        <td>148</td>
        <td>CAN Active Messages Lo Wor</td>
    </tr>
</table>

### 3.2.3 RMS Command Msg
<table>
    <tr>
        <th>Byte</th>
        <th>Value</th>
        <th>Datatype</th>
    </tr>
    <tr>
        <td>0-1</td>
        <td>Torque Command</td>
        <td>Torque</td>
    </tr>
    <tr>
        <td>2-3</td>
        <td>Speed Command</td>
        <td>Angular Velocity</td>
    </tr>
    <tr>
        <td>4</td>
        <td>Direction</td>
        <td>boolean</td>
    </tr>
    <tr>
        <td>5.0</td>
        <td>Inverter Enable</td>
        <td>boolean</td>
    </tr>
    <tr>
        <td>5.1</td>
        <td>Inverter Discharge</td>
        <td>boolean</td>
    </tr>
    <tr>
        <td>5.2</td>
        <td>Speedmode Enabled</td>
        <td>boolean</td>
    </tr>
    <tr>
        <td>6-7</td>
        <td>Command Torque Limited</td>
        <td>Torque</td>
    </tr>
</table>

### 3.2.4 BSPD
<table>
    <tr>
        <th>Byte</th>
        <th>Value</th>
        <th>Datatype</th>
    </tr>
    <tr>
        <td>0</td>
        <td>BSPD State</td>
        <td>uint8_t</td>
    </tr>
</table>

<table>
    <tr>
        <th>BSPD State Value</th>
        <th>BSPD State</th>
    </tr>
    <tr>
        <td>0</td>
        <td>False / Not Triggered</td>
    </tr>
    <tr>
        <td>1</td>
        <td>True / Triggered</td>
    </tr>
</table>

### 3.2.5 Current
<table>
    <tr>
        <th>Byte</th>
        <th>Value</th>
        <th>Datatype</th>
    </tr>
    <tr>
        <td>0-4</td>
        <td>TPS Current</td>
        <td>float</td>
    </tr>
</table>

</body>

## 3.3 LVPDB

## 3.4 DCU

## 3.5 ICS
### 3.5.1 Buttons & Swtiches
The DASH_Breakout Board transmits a 1 byte message corresponding to status of the I/O on the I/O expander, the bits in the message are broken down in the table below:
<table>
  <tr>
    <th>Bit</th>
    <th>Value</th>
    <th>Datatype</th>
  </tr>
  <tr>
    <td>0</td>
    <td>Buzzer State</td>
    <td>bool</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Button 1 (Ready-To-Drive)</td>
    <td>bool</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Button 2</td>
    <td>bool</td>
  </tr>
  <tr>
    <td>3</td>
    <td>Button 3</td>
    <td>bool</td>
  </tr>
  <tr>
    <td>4</td>
    <td>Button 4</td>
    <td>bool</td>
  </tr>
  <tr>
    <td>5</td>
    <td>Switch 1 (Coolant Pump)</td>
    <td>bool</td>
  </tr>
  <tr>
    <td>6</td>
    <td>Switch 2 (Radiator Fans)</td>
    <td>bool</td>
  </tr>
  <tr>
    <td>7</td>
    <td>Switch 3 (Accumulator Fans)</td>
    <td>bool</td>
  </tr>
</table>

## 3.6 DART
### 3.6.1 Measured Fan Speeds (Both DART1 and DART2)
<table>
  <tr>
    <th>Byte</th>
    <th>Value</th>
    <th>Datatype</th>
  </tr>
  <tr>
    <td>0</td>
    <td>Measured fan speed for fan 1, with 0 being fan off and 255 being fan at full speed</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Equivalent for fan 2</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Equivalent for fan 3</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>3</td>
    <td>Equivalent for fan 4</td>
    <td>uint8_t</td>
  </tr>
  <tr>
    <td>4</td>
    <td>Equivalent for fan 5</td>
    <td>uint8_t</td>
  </tr>
</table>
