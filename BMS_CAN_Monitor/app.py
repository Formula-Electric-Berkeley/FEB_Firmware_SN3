from can_data import MonitorCanData
import customtkinter as ctk
from gui_bms_state import BmsState
from gui_charge import Charge
# from gui_ivt import IVT
from gui_bms_heatmap import BMSHeatMap
from gui_volt_temp_table import VoltTempTable
import threading
import time
import tkinter as tk

class App(ctk.CTk):
    WIDTH = 1300
    HEIGHT = 700

    def __init__(self, mcd: MonitorCanData):
        super().__init__()

        # GUI settings
        ctk.set_appearance_mode("Dark") 
        self.title("BMS GUI")
        # self.resizable(False, False)
        self.geometry(f"{App.WIDTH}x{App.HEIGHT}")

        # Components
        self._tab_view = TabView(master=self, anchor="nw")
        self._tab_view.pack(expand=tk.YES, fill=tk.BOTH, padx=10)
        self._bms_state = BmsState(self)
        self._bms_state.pack(fill=tk.BOTH, padx=10, pady=10)

        # Interaction
        self._monitor_state = "standard"

        # Setup
        self._bind_key_press()

        # Start update thread
        self._mcd = mcd
        self._update_thread = threading.Thread(target=self._update_data)
        self._update_thread_running = True
        self._update_thread.start()

    def terminate(self) -> None:
        self._update_thread_running = False
        self._update_thread.join()
    
    def _update_data(self) -> None:
        while self._update_thread_running:
            self._tab_view._update_data(self._mcd, self._monitor_state)
            self._bms_state.update_data(self._mcd, self._monitor_state)
            time.sleep(0.5)

    def _bind_key_press(self) -> None:
        self.bind("<KeyPress>", self._key_press)

    def _key_press(self, e: tk.Event):
        match e.char:
            case 'b' | 'B':
                self._monitor_state = "balance"
            case 'r' | 'R':
                self._monitor_state = "range"
            case 's' | 'S':
                self._monitor_state = "standard"

class TabView(ctk.CTkTabview):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        # Cell data tab
        self.add("Cell Data")
        data_frame = ctk.CTkFrame(self.tab("Cell Data"))
        data_frame.pack(fill=tk.BOTH, expand=tk.TRUE)
        self._volt_temp_table = VoltTempTable(data_frame)
        self._volt_temp_table.pack(pady=(15, 0))

        # Charge tab
        self.add("Charge")
        self._charge_data = Charge(self.tab("Charge"))
        self._charge_data.pack(pady=15)

        # IVT tab
        # self.add("IVT")
        # self._ivt = IVT(self.tab("IVT"))
        # self._ivt.pack(pady=15)

        # Heatmap data tab
        self.add("HeatMap")
        self._bms_heatmap = BMSHeatMap(self.tab("HeatMap"))
        self._bms_heatmap.pack(pady=15)
    
    def _update_data(self, mcd: MonitorCanData, monitor_state: str) -> None:
        self._volt_temp_table.update_data(mcd, monitor_state)
        self._charge_data.update_data(mcd)
        self._ivt.update_data(mcd)
        self._bms_heatmap.update_data(mcd)