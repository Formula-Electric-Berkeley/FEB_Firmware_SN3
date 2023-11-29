from can_data import MonitorCanData
from bms_table import BmsTable
import tkinter as tk

class GUI:
    WIDTH = 1350
    HEIGHT = 600

    def __init__(self, mcd: MonitorCanData):
        self.root = tk.Tk()
        self.root.geometry(f"{GUI.WIDTH}x{GUI.HEIGHT}")
        self.root.title("BMS Serial Monitor")

        self.table = BmsTable(self.root, mcd)
        self.root.after(0, self.table.update)
