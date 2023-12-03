from can_data import MonitorCanData
from bms_table import BmsTable
from status_bar import StatusBar
import tkinter as tk

class GUI:
    WIDTH = 1350
    HEIGHT = 600

    def __init__(self, mcd: MonitorCanData):
        self.root = tk.Tk()
        self.root.geometry(f"{GUI.WIDTH}x{GUI.HEIGHT}")
        self.root.title("BMS Serial Monitor")

        # Components
        self.bms_table = BmsTable(self.root, mcd)
        self.status = StatusBar(self.root)

        # Update
        self.root.after(0, self.bms_table.update)
