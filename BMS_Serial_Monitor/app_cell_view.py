import app_helper
import constants
import customtkinter as ctk
import tkinter as tk
import bms_data

class CellView(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self.__volt_cells = dict()              # (bank, cell) : tk.Label
        self.__temp_cells = dict()              # (bank, cell) : tk.Label
        self.__bank_volt_mean_cells = dict()         # bank : tk.Label
        self.__bank_temp_mean_cells = dict()         # bank: tk.Label
        self.__bank_volt_total_cells = dict()   # bank: tk.Label

        self.__setup_view()

    def update_data(self, data: bms_data.BmsData):
        bms_data.lock.acquire()

        for bank in range(1, constants.NUM_BANKS + 1):
            bank_volt_total = 0
            bank_temp_total = 0
            cell_volt_count = 0
            cell_temp_count = 0
            for cell in range(1, constants.NUM_CELLS_PER_BANK + 1):
                cell_data = data.cell_data.cell.get((bank, cell), None)
                if not cell_data:
                    continue
                
                # Read values
                self.__volt_cells[(bank, cell)].configure(text=str(cell_data["voltage"]))
                self.__temp_cells[(bank, cell)].configure(text=str(cell_data["temperature"]))

                # Update cell count
                if not cell_data["voltage-fault"]:
                    bank_volt_total += cell_data["voltage"]
                    cell_volt_count += 1
                if not cell_data["temperature-fault"]:
                    bank_temp_total += cell_data["temperature"]
                    cell_temp_count += 1

                # Update cell color
                if (cell_data["voltage-fault"]):
                    app_helper.update_cell_color(self.__volt_cells[(bank, cell)], app_helper.COLOR_OUT_OF_RANGE)
                elif cell_data["balance"]:
                    app_helper.update_cell_color(self.__volt_cells[(bank, cell)], app_helper.COLOR_BALANCE)
                else:
                    app_helper.update_cell_color(self.__volt_cells[(bank, cell)], app_helper.COLOR_STANDARD)
                if (cell_data["temperature-fault"]):
                    app_helper.update_cell_color(self.__temp_cells[(bank, cell)], app_helper.COLOR_OUT_OF_RANGE)
                else:
                    app_helper.update_cell_color(self.__temp_cells[(bank, cell)], app_helper.COLOR_STANDARD)

            
            # Store bank processed data
            bank_volt_total = round(bank_volt_total, 3)
            bank_volt_mean = round(bank_volt_total / cell_volt_count, 3) if cell_volt_count > 0 else 0
            bank_temp_mean = round(bank_temp_total / cell_temp_count, 3) if cell_temp_count > 0 else 0
            self.__bank_volt_total_cells[bank].configure(text=str(bank_volt_total))
            self.__bank_volt_mean_cells[bank].configure(text=str(bank_volt_mean))
            self.__bank_temp_mean_cells[bank].configure(text=str(bank_temp_mean))

        bms_data.lock.release()

    def __setup_view(self):
        row = 0
        row = self.__setup_raw_view(row)

    def __setup_raw_view(self, row):
        # Header
        app_helper.create_cell(self, row=0, col=0, text="Cell", rs=2, wsf=1.5)
        for i in range(constants.NUM_BANKS):
            col = i*2 + 1
            bank_num = i + 1
            app_helper.create_cell(self, row=row, col=col, cs=2, text=f"Bank {bank_num}")
        for i in range(constants.NUM_BANKS * 2):
            col = i + 1
            text = "V" if i % 2 == 0 else "T"
            app_helper.create_cell(self, row=row+1, col=col, text=text)
        row += 2

        # Body
        for i in range(constants.NUM_CELLS_PER_BANK):
            cell = i + 1
            app_helper.create_cell(self, row=row, col=0, text=str(cell))
            for j in range(constants.NUM_BANKS):
                bank = j + 1
                volt_cell = app_helper.create_cell(self, row=row, col=2*j+1, text="")
                temp_cell = app_helper.create_cell(self, row=row, col=2*j+2, text="")
                self.__volt_cells[(bank, cell)] = volt_cell
                self.__temp_cells[(bank, cell)] = temp_cell
            row += 1

        # Bank statistics
        app_helper.create_cell(self, row=row, col=0, text="Mean")
        app_helper.create_cell(self, row=row+1, col=0, text="Total")
        for i in range(constants.NUM_BANKS):
            bank = i + 1
            self.__bank_volt_mean_cells[bank] = app_helper.create_cell(self, row=row, col=2*i+1, text="")
            self.__bank_temp_mean_cells[bank] = app_helper.create_cell(self, row=row, col=2*i+2, text="")
            self.__bank_volt_total_cells[bank] = app_helper.create_cell(self, row=row+1, col=2*i+1, text="")
            app_helper.create_cell(self, row=row+1, col=2*i+2, text="")
        row += 2

        return row
