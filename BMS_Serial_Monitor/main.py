from app import App
from bms_data import BmsData
from serial_connection import SerialConnection
import threading
import sys
import time

def main() -> None:
    data = BmsData()

    sc = SerialConnection(data)
    threading.Thread(target=sc.read_data).start()

    app = App()
    app.protocol("WM_DELETE_WINDOW", lambda: sys.exit(0))
    threading.Thread(target=update_data, args=[app, data]).start()

    app.mainloop()

def update_data(app: App, data: BmsData):
    while True:
        app.update_data(data)
        time.sleep(0.1)

if __name__ == "__main__":
    main()
