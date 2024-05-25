from app import App
from can_connection import CanConnection
from can_data import MonitorCanData
# import os
# import platform

def main() -> None:
    # CAN
    mcd = MonitorCanData()
    # cc = CanConnection(mcd)
    # cc.connect()
    # cc.read_data()
    
    # TODO: Remove
    # test(mcd)

    # GUI
    app = App(mcd)
    app.mainloop()

    # Clean up
    # cc.stop_read_data()
    # cc.disconnect()
    app.terminate()

    print("Exiting...")

def test(mcd: MonitorCanData):
    import random
    import constants
    import threading
    import time
    import ctypes

    def helper():
        while True:
            # Cell voltage
            for bank in range(1, constants.NUM_BANKS + 1):
                for cell in range(1, constants.NUM_CELLS_PER_BANK + 1):
                    mcd.bms.voltage[(bank, cell)] = round(random.random() * (4.3 - 2.4) + 2.4, 3)

            # Cell temperature
            for bank in range(1, constants.NUM_BANKS + 1):
                for cell in range(1, constants.NUM_CELLS_PER_BANK + 1):
                    mcd.bms.temp[(bank, cell)] = round(random.random() * 65 - 2.5, 3)

            # IVT Current
            mcd.ivt._current = ctypes.c_int32(random.randint(1000, 10000))

            # BMS State
            mcd.bms.state = random.choice(["precharge", "drive", "shutdown", "charge"])

            # Balance
            for bank in range(1, constants.NUM_BANKS + 1):
                mcd.bms.balance[bank] = [random.choice([True, False]) for _ in range(constants.NUM_CELLS_PER_BANK)]
        
            time.sleep(2)

    t1 = threading.Thread(target=helper)
    t1.start()

if __name__ == "__main__":
    # if platform.system() == "Darwin":
    #     # Fix warning in Sonoma
    #     f = open("/dev/null", "w")
    #     os.dup2(f.fileno(), 2)
    #     f.close()
    main()
