from can_data import MonitorCanData
import constants
import customtkinter as ctk
import tkinter as tk

class VoltTempTable(ctk.CTkFrame):
    COLOR_STANDARD = "#323232"
    COLOR_OUT_OF_RANGE = "#ff0000"
    COLOR_BALANCE = "#3262a8"

    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        # Cell data
        self._bank_header = dict()          # bank -> tk.Label(text=bank)
        self._cell_header = dict()          # cell -> tk.Label(text=cell)
        self._id_volt_map = dict()          # (bank, cell) -> volt cell
        self._id_temp_map = dict()          # (bank, cell) -> temp cell
        self._volt_temp_id_map = dict()     # volt cell | temp cell -> (bank, cell, "V" | "T")

        # Bank stats
        self._bank_volt_mean = dict()       # bank -> volt cell
        self._bank_volt_total = dict()      # bank -> volt cell
        self._bank_temp_mean = dict()       # bank -> temp cell

        # Battery stats
        self._volt_stats = dict()
        self._temp_stats = dict()

        self._setup_tables()
    
    def update_data(self, mcd: MonitorCanData, monitor_state: str) -> None:
        # Cell voltage
        for bank, cell in mcd.bms.voltage:
            self._update_cell(bank, cell, 'V', mcd.bms.voltage[(bank, cell)], monitor_state, mcd.bms.state, 
                              mcd.bms.balance)

        # Cell temperature
        for bank, cell in mcd.bms.temp:
            self._update_cell(bank, cell, 'T', mcd.bms.temp[(bank, cell)], monitor_state, mcd.bms.state, 
                              mcd.bms.balance)

        # Stats
        volt_stats = mcd.bms.voltage.get_stats()
        temp_stats = mcd.bms.temp.get_stats()
        for bank in range(1, constants.NUM_BANKS + 1):
            bank_volt_stats = mcd.bms.voltage.get_stats(bank)
            bank_temp_stats = mcd.bms.temp.get_stats(bank)
            
            # Bank stats
            self._update_bank_mean(bank, 'V', bank_volt_stats["mean"])
            self._update_bank_total(bank, 'V', bank_volt_stats["sum"])
            self._update_bank_mean(bank, 'T', bank_temp_stats["mean"])

            # Battery stats
            stat_types = ["mean", "min", "max", "stdev", "range", "sum"]
            for s in stat_types:
                self._update_battery_stats('V', s, volt_stats[s])
            for s in stat_types[:-1]:
                self._update_battery_stats('T', s, temp_stats[s])

    def _update_cell(self, bank: int, cell: int, value_type: str, value: int | float, monitor_state: str,
                     bms_state: str, balance: dict[str, list[bool]]) -> None:
        if value_type.upper() == 'V':
            volt_cell = self._id_volt_map.get((bank, cell), None)
            if volt_cell is not None:
                # Update text
                text = str(value)

                # Update color
                monitor_state = monitor_state.lower()
                bg = self.COLOR_STANDARD
                if monitor_state == "range" and (value < constants.CELL_VOLT_MIN or value > constants.CELL_VOLT_MAX):
                    bg = self.COLOR_OUT_OF_RANGE
                elif monitor_state == "balance" and bank in balance and balance[bank][cell - 1]:
                    bg = self.COLOR_BALANCE

                # Set cell
                volt_cell.configure(text=text, bg=bg)
        elif value_type.upper() == 'T':
            temp_cell = self._id_temp_map.get((bank, cell), None)
            if temp_cell is not None:
                # Update text
                text = str(value)

                # Update color
                monitor_state = monitor_state.lower()
                bms_state = bms_state.lower()
                bg = self.COLOR_STANDARD
                if monitor_state == "range":
                    if bms_state == "charge" and (value < constants.CELL_TEMP_CHARGE_MIN or
                                                  value > constants.CELL_TEMP_CHARGE_MAX):
                        bg = self.COLOR_OUT_OF_RANGE    
                    elif value < constants.CELL_TEMP_DISCHARGE_MIN or value > constants.CELL_TEMP_DISCHARGE_MAX:
                        bg = self.COLOR_OUT_OF_RANGE

                # Set cell
                temp_cell.configure(text=text, bg=bg)
    
    def _update_bank_mean(self, bank: int, value_type: str, value: int | float) -> None:
        value = round(value, 3)
        if value_type.upper() == 'V':
            cell = self._bank_volt_mean.get(bank, None)
            if cell is not None:
                cell.configure(text=str(value))
        if value_type.upper() == 'T':
            cell = self._bank_temp_mean.get(bank, None)
            if cell is not None:
                cell.configure(text=str(value))
    
    def _update_bank_total(self, bank: int, value_type: str, value: int | float) -> None:
        if value_type.upper() == 'V':
            cell = self._bank_volt_total.get(bank, None)
            if cell is not None:
                value = round(value, 3)
                cell.configure(text=str(value))

    def _update_battery_stats(self, value_type: str, stat_type: str, value: int | float) -> None:
        value = round(value, 3)
        if value_type.upper() == 'V':
            cell = self._volt_stats.get(stat_type, None)
            if cell is not None:
                cell.configure(text=str(value))
        if value_type.upper() == 'T':
            cell = self._temp_stats.get(stat_type, None)
            if cell is not None:
                cell.configure(text=str(value))

    def _cell_enter(self, event: tk.Event):
        cell_id = self._volt_temp_id_map.get(event.widget, None)
        if cell_id is None:
            return
        cell = cell_id[1]
        for cell_header in self._cell_header.values():
            pass
            cell_header.configure(fg="white")
        self._cell_header[cell].configure(fg="red")
    
    def _cell_leave(self, event: tk.Event):
        cell_id = self._volt_temp_id_map.get(event.widget, None)
        if cell_id is None:
            return
        cell = cell_id[1]
        self._cell_header[cell].configure(fg="white")

    def _setup_tables(self) -> None:
        row = 0
        row = self._setup_data_table(row)
        row = self._setup_statistics_table(row)

    def _setup_data_table(self, row: int) -> int:
        # Header
        self._create_cell(row=0, col=0, text="Cell", rs=2, wsf=1.5)
        for i in range(constants.NUM_BANKS):
            col = i*2 + 1
            bank_num = i + 1
            bank_header = self._create_cell(row=row, col=col, cs=2, text=f"Bank {bank_num}")
            self._bank_header[bank_num] = bank_header
        for i in range(constants.NUM_BANKS * 2):
            col = i + 1
            text = "V" if i % 2 == 0 else "T"
            self._create_cell(row=row+1, col=col, text=text)
        row += 2

        # Body
        for i in range(constants.NUM_CELLS_PER_BANK):
            cell = i + 1
            cell_header = self._create_cell(row=row, col=0, text=str(cell))
            self._cell_header[cell] = cell_header
            for j in range(constants.NUM_BANKS):
                bank = j + 1
                volt_cell = self._create_cell(row=row, col=2*j+1, text="")
                temp_cell = self._create_cell(row=row, col=2*j+2, text="")
                self._volt_temp_id_map[volt_cell] = (bank, cell, "V")
                self._volt_temp_id_map[temp_cell] = (bank, cell, "T")
                self._id_volt_map[(bank, cell)] = volt_cell
                self._id_temp_map[(bank, cell)] = temp_cell
                volt_cell.bind("<Enter>", self._cell_enter)
                temp_cell.bind("<Enter>", self._cell_enter)
                volt_cell.bind("<Leave>", self._cell_leave)
                temp_cell.bind("<Leave>", self._cell_leave)
            row += 1

        # Bank statistics
        self._create_cell(row=row, col=0, text="Mean")
        self._create_cell(row=row+1, col=0, text="Total")
        for i in range(constants.NUM_BANKS):
            bank = i + 1
            self._bank_volt_mean[bank] = self._create_cell(row=row, col=2*i+1, text=f"")
            self._bank_temp_mean[bank] = self._create_cell(row=row, col=2*i+2, text=f"")

            self._bank_volt_total[bank] = self._create_cell(row=row+1, col=2*i+1, text=f"")
            self._create_cell(row=row+1, col=2*i+2, text=f"-")
        row += 2

        return row

    def _setup_statistics_table(self, row: int) -> int:
        row = self._create_empty_row(row)
    
        # Header
        self._create_cell(row=row, col=0, text="All Banks")
        self._create_cell(row=row, col=1, text="Mean", cs=2)
        self._create_cell(row=row, col=3, text="Min", cs=2)
        self._create_cell(row=row, col=5, text="Max", cs=2)
        self._create_cell(row=row, col=7, text="Std. Deviation", cs=2)
        self._create_cell(row=row, col=9, text="Range", cs=2)
        self._create_cell(row=row, col=11, text="Total", cs=2)
        row += 1

        # Voltage stats
        self._create_cell(row, 0, "V")
        self._volt_stats["mean"] = self._create_cell(row, 1, "", cs=2)
        self._volt_stats["min"] = self._create_cell(row, 3, "", cs=2)
        self._volt_stats["max"] = self._create_cell(row, 5, "", cs=2)
        self._volt_stats["stdev"] = self._create_cell(row, 7, "", cs=2)
        self._volt_stats["range"] = self._create_cell(row, 9, "", cs=2)
        self._volt_stats["sum"] = self._create_cell(row, 11, "", cs=2)
        row += 1

        # Temperature stats
        self._create_cell(row, 0, "T")
        self._temp_stats["mean"] = self._create_cell(row, 1, "", cs=2)
        self._temp_stats["min"] = self._create_cell(row, 3, "", cs=2)
        self._temp_stats["max"] = self._create_cell(row, 5, "", cs=2)
        self._temp_stats["stdev"] = self._create_cell(row, 7, "", cs=2)
        self._temp_stats["range"] = self._create_cell(row, 9, "", cs=2)
        self._create_cell(row, 11, "-", cs=2)
        row += 1

    def _create_cell(self, row: int, col: int, text: str, rs: int = 1, cs: int = 1, wsf: float | int = 1,
                     bd: int = 1) -> tk.Label:
        cell = tk.Label(self, text=text, width=int(8*wsf), height=1, relief="solid", bd=bd, 
                        background=self.COLOR_STANDARD, fg="white")
        cell.grid(row=row, rowspan=rs, column=col, columnspan=cs, sticky="nesw")
        return cell

    def _create_empty_row(self, row: int) -> int:
        cell = ctk.CTkLabel(self, text="", width=8, height=1)
        cell.grid(row=row, column=0)
        return row + 1
