# SN3 APPS code 

## Description
Please use this doc as a debug log and documentation for all code related to the APPS. 
The APPS or "Acceleration Pedal Position Sensor" is a board on our vehicle that controls the
torque to the inverter. 

### 3/8/24 
Changes from SN2 vehicle that need to be implemented in this repository include the HECS sensing 
and the current sensing. There might also need to be a reading from the BSPD reset. 

## 3/24/24


FEB_Normalized: file that handles normalizing brake and accelerator position values

FEB_HECS: APPS Board will read whether this current from the HECS and turn on the HECS indicator by writing to the GPIO port
to turn on the LED

FEB_CAN_BSPD: APPS is checking whether BSPD is triggered. If its triggered it will transmit that it is triggered on CAN
otherwise transmits not triggered.

Note: For clarification purposes, the HECS is not the same thing as the BSPD, and the HECS indicator is seperate from the BSPD indicator
All things related to the BSPD on the SN3 APPS board are done via analog except sending CAN messages. 


FEB_CAN_RMS_getRegenTorque: returns the regen torque we want to command
Considers the battery temperature to make sure we're not going over the temp limit
Commands 0 torque for certain unsafe conditions specified by MIT paper (implementation incomplete)

FEB_CAN_RMS_getDesiredRegenTorque: maxRegenTorqueMech computes the max regen torque we can exert from the mechanical perspective (awaiting that function)
A function that outputs our desired regen torque, only considering mechanical limitations (implementation incomplete)

## 3/28/24

Addeding FEB_CAN_BMS.c and .h to read BMS voltage and SOC values for Regen equations