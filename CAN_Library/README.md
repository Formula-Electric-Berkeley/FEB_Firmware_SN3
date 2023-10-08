# CAN Library

- [CAN Library](#can-library)
- [1 Overview](#1-overview)
- [2 Update Instructions](#2-update-instructions)
  - [2.1 Add CAN Message](#21-add-can-message)
  - [2.2 Generate](#22-generate)
  - [2.3 Documentation](#23-documentation)
  - [2.4 GitHub](#24-github)
- [3 CAN Message Documentation](#3-can-message-documentation)
  - [3.1 BMS](#31-bms)
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
## 2.1 Add CAN Message
To add a CAN message with a static CAN ID, update ```FEB_CAN_STATIC_ID.csv``` file. To add a CAN message without a static CAN ID, update the ```FEB_CAN_ID.csv``` file.

## 2.2 Generate
Run ```generate.py``` script. This will update the ```FEB_CAN_ID.h``` header file. Add this file to STM32 project, if necessary.

## 2.3 Documentation
Document the new CAN message in this readme file.

## 2.4 GitHub
Push all changes to GitHub to ensure the CAN Library is up to date for everyone.

# 3 CAN Message Documentation

## 3.1 BMS

## 3.2 APPS

## 3.3 LVPDB

## 3.4 DCU