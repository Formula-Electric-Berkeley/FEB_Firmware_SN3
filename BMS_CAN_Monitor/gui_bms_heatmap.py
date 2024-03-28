from can_data import MonitorCanData
import constants
import customtkinter as ctk
import tkinter as tk

class BMSHeatMap(ctk.CTkFrame):
    COLOR_STANDARD = "#323232"
    COLOR_COLD = "#ffffff"
    COLOR_HOT = "#f54242"

    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self._cell_map = dict() #(bank, cell) -> cell

        self._setup_tables()

    def _setup_tables(self) -> None:
        # Header
        row = 0
        self._create_cell(row=row, col=0, text="Cell", background=self.COLOR_STANDARD, rs=1, wsf=2)
        for i in range(constants.NUM_CELLS_PER_BANK):
            self._create_cell(row=row, col=i+1, text=str(i+1), background=self.COLOR_STANDARD)
        row += 1

        # Body
        for i in range(constants.NUM_BANKS):
            bank_num = i + 1
            self._create_cell(row=row, col=0, text="Bank "+str(bank_num), background=self.COLOR_STANDARD, wsf=2)
            for j in range(constants.NUM_CELLS_PER_BANK):
                cell_num = j + 1
                cell = self._create_cell(row=row, col=j+1, text="", background=self.COLOR_COLD)
                self._cell_map[(bank_num, cell_num)] = cell
            row += 1


    def _create_cell(self, row: int, col: int, text: str, background: str, rs: int = 1, cs: int = 1, wsf: float | int = 1,
                     bd: int = 1) -> tk.Label:
        cell = tk.Label(self, text=text, width=int(4*wsf), height=2, relief="solid", bd=bd, 
                        background=background, fg="white")
        cell.grid(row=row, rowspan=rs, column=col, columnspan=cs, sticky="nesw")
        return cell

    def update_data(self, mcd: MonitorCanData) -> None:
        for bank, cell in mcd.bms.temp:
            self._update_cell(bank, cell, mcd.bms.temp[(bank, cell)])

    def _update_cell(self, bank: int, cell: int, temp: float):
        cell = self._cell_map.get((bank, cell), None)
        if cell is not None:
            red_value = str(hex(255))[2:]
            green_value = str(hex(self.color_hex_map(temp)))[2:]
            if len(green_value) == 1:
                green_value = "0" + green_value
            blue_value = str(hex(self.color_hex_map(temp)))[2:]
            if len(blue_value) == 1:
                blue_value = "0" + blue_value
            bg_hex = "#" + red_value + green_value + blue_value
            cell.configure(background=bg_hex)

    def color_hex_map(self, temp: float):
        if temp < -20 or temp > 60:
            return 0
        normalized_value = (temp + 20) / 80
        mapped_value = int((1 - normalized_value) * 255)
        return mapped_value