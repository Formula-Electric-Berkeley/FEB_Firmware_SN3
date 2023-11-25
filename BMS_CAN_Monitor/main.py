from can_connection import CanConnection
from can_data import MonitorCanData
import signal
import sys
import threading

def main():
    global cc, t1

    # Exit handler
    print("Press CTRL+C to exit")
    signal.signal(signal.SIGINT, sigint_handler)

    # CAN
    mcd = MonitorCanData()
    cc = CanConnection(mcd)
    cc.connect()

    # GUI


    # CAN Read
    t1 = threading.Thread(target=cc.read_data)
    t1.start()

def sigint_handler(signum, frame):
    #  Clean up
    cc.stop_read_data()
    cc.disconnect()

    # Exit
    print("\nExiting...")
    sys.exit(0)

if __name__ == "__main__":
    main()
