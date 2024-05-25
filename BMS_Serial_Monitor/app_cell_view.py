from app_helper import create_cell
import constants
import customtkinter as ctk
import tkinter as tk
import bms_data

class CellView(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self.__volt_cells = dict()  # (bank, cell) : tk.Label
        self.__temp_cells = dict()  # (bank, cell) : tk.Label

        self.__setup_view()

    def update_data(self, data: bms_data.BmsData):
        bms_data.lock.acquire()
        for bank in range(1, constants.NUM_BANKS + 1):
            for cell in range(1, constants.NUM_CELLS_PER_BANK + 1):
                cell_data = data.cell_data.cell.get((bank, cell), None)
                if not cell_data:
                    continue
                self.__volt_cells[(bank, cell)].configure(text=str(cell_data["voltage"]))
                self.__temp_cells[(bank, cell)].configure(text=str(cell_data["temperature"]))
        bms_data.lock.release()

    def __setup_view(self):
        row = 0
        row = self.__setup_raw_view(row)

    def __setup_raw_view(self, row):
        # Header
        create_cell(self, row=0, col=0, text="Cell", rs=2, wsf=1.5)
        for i in range(constants.NUM_BANKS):
            col = i*2 + 1
            bank_num = i + 1
            create_cell(self, row=row, col=col, cs=2, text=f"Bank {bank_num}")
        for i in range(constants.NUM_BANKS * 2):
            col = i + 1
            text = "V" if i % 2 == 0 else "T"
            create_cell(self, row=row+1, col=col, text=text)
        row += 2

        # Body
        for i in range(constants.NUM_CELLS_PER_BANK):
            cell = i + 1
            create_cell(self, row=row, col=0, text=str(cell))
            for j in range(constants.NUM_BANKS):
                bank = j + 1
                volt_cell = create_cell(self, row=row, col=2*j+1, text="")
                temp_cell = create_cell(self, row=row, col=2*j+2, text="")
                self.__volt_cells[(bank, cell)] = volt_cell
                self.__temp_cells[(bank, cell)] = temp_cell
            row += 1
        
        return row
