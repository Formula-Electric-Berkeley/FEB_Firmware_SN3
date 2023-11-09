# CAN Library

- [CAN Library](#can-library)
- [1 Overview](#1-overview)
- [2 Update Instructions](#2-update-instructions)
- [3 CAN Message Documentation](#3-can-message-documentation)
  - [3.1 BMS](#31-bms)
    - [3.1.1 Voltage](#311-voltage)
    - [3.1.2 Temperature](#312-temperature)
    - [3.1.3 State](#313-state)
  - [3.2 APPS](#32-apps)
  - [3.3 LVPDB](#33-lvpdb)
  - [3.4 DCU](#34-dcu)

# 1 Overview
STM32 Files:
* ```FEB_CAN_ID.h```: A header file that stores all CAN Message IDs.
* ```FEB_CAN.c```: A file that handles CAN library initialization and received message handling. Additional CAN files will build of this one. File contains template code for writing additional CAN files.
* ```FEB_CAN.h```: Header file for ```FEB_CAN.c```.

Other files:
* ```FEB_CAN_ID.csv```: A CSV file that stores names of CAN messages without assigned CAN IDs. This file is used to generate dynamic CAN IDs.
* ```FEB_CAN_STATIC_ID.csv```: A CSV file that sores names of CAN messages and their assigned CAN ID.
* ```generate.py```: This file uses data from ```FEB_CAN_ID.csv``` and ```FEB_CAN_STATIC_ID.csv``` to generate ```FEB_CAN_ID.h```.

# 2 Update Instructions
1. <b>Add CAN Message</b>: To add a CAN message with a static CAN ID, update ```FEB_CAN_STATIC_ID.csv``` file. To add a CAN message without a static CAN ID, update the ```FEB_CAN_ID.csv``` file.
2. <b>Generate</b>: Run ```generate.py``` script. This will update the ```FEB_CAN_ID.h``` header file. Add this file to STM32 project, if necessary.
3. <b>Documentation</b>: Document the new CAN message in this readme file.
4. <b>GitHub</b>: Push all changes to GitHub to ensure the CAN Library is up to date for everyone.

# 3 CAN Message Documentation

## 3.1 BMS
### 3.1.1 Voltage
<table>
  <tr>
    <th>Byte</th>
    <th>Value</th>
    <th>Factor</th>
    <th>Offset</th>
    <th>Datatype</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>0</td>
    <td>Bank</td>
    <td>1</td>
    <td>0</td>
    <td>uint8_t</td>
    <td>-</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Cell</td>
    <td>1</td>
    <td>0</td>
    <td>uint8_t</td>
    <td>-</td>
  </tr>
  <tr>
    <td>2-3</td>
    <td>Voltage</td>
    <td>10<sup>-4</sup></td>
    <td>0</td>
    <td>uint16_t</td>
    <td>V</td>
  </tr>
</table>

### 3.1.2 Temperature
<table>
  <tr>
    <th>Byte</th>
    <th>Value</th>
    <th>Factor</th>
    <th>Offset</th>
    <th>Datatype</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>0</td>
    <td>Bank</td>
    <td>1</td>
    <td>0</td>
    <td>uint8_t</td>
    <td>-</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Cell</td>
    <td>1</td>
    <td>0</td>
    <td>uint8_t</td>
    <td>-</td>
  </tr>
  <tr>
    <td>2-3</td>
    <td>Temperature</td>
    <td>10<sup>-2</sup></td>
    <td>0</td>
    <td>int16_t</td>
    <td><sup>o</sup>C</td>
  </tr>
</table>

### 3.1.3 State

## 3.2 APPS

## 3.3 LVPDB

## 3.4 DCU