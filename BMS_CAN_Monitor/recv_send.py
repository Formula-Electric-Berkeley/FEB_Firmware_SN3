import ctypes

import PCANBasic
import select
import sys
import threading
import time

# Constants
channel = PCANBasic.PCAN_USBBUS1
baudrate = PCANBasic.PCAN_BAUD_250K

def main():
    # Setup
    pcan = PCANBasic.PCANBasic()
    
    # Connect
    status = pcan.Initialize(channel, baudrate)
    if status != PCANBasic.PCAN_ERROR_OK:
        print(f"ERROR: {pcan.GetErrorText(status)}")
        sys.exit(1)

    # Monitor bus
    threading.Thread(target=recv, args=[pcan]).start()
    threading.Thread(target=send, args=[pcan]).start()

def recv(pcan: PCANBasic.PCANBasic):
    result = pcan.GetValue(channel, PCANBasic.PCAN_RECEIVE_EVENT)
    if result[0] == PCANBasic.PCAN_ERROR_OK:
        fd = result[1]
    else:
        fd = -1

    while True:
        status, message, ts = pcan.Read(channel)
        if status == PCANBasic.PCAN_ERROR_OK:
            data = [x for x in message.DATA]
            print(f"ID: {message.ID}, DLC: {message.LEN}, Data: {data}")
        elif status == PCANBasic.PCAN_ERROR_QRCVEMPTY:
            if fd != -1:
                select.select([fd],[],[])
            else:
                time.sleep(0.001)

def send(pcan: PCANBasic.PCANBasic):
    while True:
        message = PCANBasic.TPCANMsg()
        message.ID = ctypes.c_uint(5)
        message.MSGTYPE = PCANBasic.PCAN_MESSAGE_STANDARD
        message.LEN = ctypes.c_ubyte(0)
        message.DATA = (ctypes.c_ubyte * 8)()

        status = pcan.Write(channel, message)
        if status == PCANBasic.PCAN_ERROR_OK:
            pass
        time.sleep(0.3)

if __name__ == "__main__":
    main()