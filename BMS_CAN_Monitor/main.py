from can_connection import CanConnection
from can_data import MonitorCanData
from gui import GUI
import signal
import sys

def main() -> None:
    global cc, gui

    # Exit handler
    print("Press CTRL+C to exit")
    signal.signal(signal.SIGINT, sigint_handler)

    # CAN
    mcd = MonitorCanData()
    cc = CanConnection(mcd)
    # cc.connect()
    # cc.read_data()

    # GUI
    gui = GUI(mcd)
    gui.root.mainloop()

    sigint_handler()

def sigint_handler(signum=None, frame=None) -> None:
    #  Clean up
    cc.stop_read_data()
    cc.disconnect()
    gui.root.destroy()

    # Exit
    print("\nExiting...")
    sys.exit(0)

if __name__ == "__main__":
    main()
