from can_data import MonitorCanData
import PCANBasic
import select
import sys
import time
import threading

class CanConnection:
    CHANNEL = PCANBasic.PCAN_USBBUS1
    BAUDRATE = PCANBasic.PCAN_BAUD_500K

    def __init__(self, mcd: MonitorCanData):
        self._mcd = mcd
        self._pcan = None
        self._read_thread = None
        self._read_thread_running = False

    # ****************************** Connection ********************************
    
    def connect(self) -> None:
        self._pcan = PCANBasic.PCANBasic()
        status = self._pcan.Initialize(CanConnection.CHANNEL, CanConnection.BAUDRATE)
        if status != PCANBasic.PCAN_ERROR_OK:
            print(f"ERROR: {self._pcan.GetErrorText(status)}")
            sys.exit(1)
    
    def disconnect(self) -> None:
        status = self._pcan.Uninitialize(CanConnection.CHANNEL)
        if status == PCANBasic.PCAN_ERROR_OK:
            print("STATUS: PCAN disconnected")
        else:
            print(f"ERROR: {self._pcan.GetErrorText(status)}")
            sys.exit(1)

    # ****************************** Receive ********************************

    def read_data(self) -> None:
        self._read_thread = threading.Thread(target=self._read_data)
        self._read_thread_running = True
        self._read_thread.start()

    def stop_read_data(self):
        self._read_thread_running = False
        self._read_thread.join()

    def _read_data(self) -> None:
        res = self._pcan.GetValue(CanConnection.CHANNEL, PCANBasic.PCAN_RECEIVE_EVENT)
        if res[0] == PCANBasic.PCAN_ERROR_OK:
            fd = res[1]
            print("NO ERROR")
        else:
            fd = -1
            print("ERROR")
        
        while self._read_thread_running:
            status, message, _ = self._pcan.Read(CanConnection.CHANNEL)
            if status == PCANBasic.PCAN_ERROR_OK:
                self._mcd.store_message(message)
                print("NOS ERROR")
            elif status == PCANBasic.PCAN_ERROR_QRCVEMPTY:
                if fd != -1:
                    select.select([fd], [], [])
                else:
                    time.sleep(0.001)
                print("S ERROR")
    
    # ****************************** Transmit ********************************

    def send_data(self, message: PCANBasic.PCANBasic) -> None:
        status = self._pcan.Write(CanConnection.CHANNEL, message)
        if status == PCANBasic.PCAN_ERROR_OK:
            print("STATUS: Message sent")
        else:
            print(f"ERROR: {self._pcan.GetErrorText(status)}")
