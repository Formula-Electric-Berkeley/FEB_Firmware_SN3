import customtkinter as ctk
import tkinter as tk
from app_cell_view import CellView
from bms_data import BmsData

class App(ctk.CTk):
    WIDTH=1300
    HEIGHT=700

    def __init__(self):
        super().__init__()

        # GUI Settings
        self.title("BMS GUI")
        self.geometry(f"{App.WIDTH}x{App.HEIGHT}")
        ctk.set_appearance_mode("light")

        # Components
        self.__tab_view = TabView(master=self)
        self.__tab_view.pack(expand=tk.YES, fill=tk.BOTH, padx=10)
        self.__status_bar = StatusBar(master=self)
        self.__status_bar.pack(fill=tk.X, padx=10, pady=10)

    def update_data(self, data: BmsData):
        self.__tab_view.update_data(data)
        self.__status_bar.update_data(data)

class TabView(ctk.CTkTabview):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        # Cell data tab
        self.add("Cell Data")
        data_frame = ctk.CTkFrame(self.tab("Cell Data"))
        data_frame.pack(fill=tk.BOTH, expand=tk.TRUE)
        self.__cell_view = CellView(data_frame)
        self.__cell_view.pack(pady=(15, 0))
    
    def update_data(self, data: BmsData):
        self.__cell_view.update_data(data)

class StatusBar(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        inner_frame = ctk.CTkFrame(self, fg_color="transparent")
        inner_frame.pack(pady=10)

        # BMS State
        bms_state_frame = ctk.CTkFrame(inner_frame)
        bms_state_frame.grid(row=0, column=0)
        ctk.CTkLabel(bms_state_frame, text="BMS State:", width=50).grid(row=0, column=0, padx=(0,15))
        self.__bms_state_text = ctk.CTkLabel(bms_state_frame, width=100, text="", fg_color="#3669A0",
                                             corner_radius=5, text_color="white")
        self.__bms_state_text.grid(row=0, column=1)
    
    def update_data(self, data: BmsData):
        self.__bms_state_text.configure(text="precharge")
