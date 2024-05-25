from app import App
from bms_data import BmsData
from serial_connection import SerialConnection
import threading
import sys

def main() -> None:
    data = BmsData()
    
    # TODO: Remove test code
    for i in range(1, 8):
        for j in range(1, 21):
            import random
            data.cell_data.cell[(i, j)] = {"voltage": round(random.random() * 10, 3), "temperature": 3.14}

    # sc = SerialConnection(bms_data)
    # threading.Thread(target=sc.read_data())

    app = App()
    app.protocol("WM_DELETE_WINDOW", lambda: sys.exit(0))
    threading.Thread(target=app.update_data, args=[data]).start()

    app.mainloop()

if __name__ == "__main__":
    main()
