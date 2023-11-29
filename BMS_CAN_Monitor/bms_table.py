import accumulator
from can_data import MonitorCanData
import tkinter as tk
from VerticalScrolledFrame import VerticalScrolledFrame

# TODO: Remove
import random

class BmsTable:
    # Cell size
    CELL_WIDTH = 8
    CELL_HEIGHT = 1

    # Colors
    COLOR_DEFAULT = ""
    COLOR_OUT_OF_RANGE = "#DD0000"
    COLOR_WARNING = "#F26D21"
    COLOR_DEVIATION_VOLT = "#00DD00"
    COLOR_DEVIATION_TEMP = "#DD0000"
    COLOR_RELAY_OPEN = "#F26D21"

    # Monitor state
    STANDARD_STATE = "standard"
    RANGE_STATE = "range"
    BALANCE_STATE = "balance"
    DEVIATION_STATE = "deviation"

    # Update rate (ms)
    UPDATE_INTERVAL = 1000

    def __init__(self, root: tk.Tk, mcd: MonitorCanData):
        self._root = root
        self._mcd = mcd
        self._frame = self._create_table_frame()

        # Monitor
        self._monitor_state = BmsTable.STANDARD_STATE

        # Cells
        self._state_cells = dict()              # str -> tk.Label
        self._volt_cells = dict()               # (bank, cell) -> tk.Label
        self._temp_cells = dict()               # (bank, cell) -> tk.Label
        self._volt_bank_stats_cells = dict()    # bank -> dict[str -> tk.Label]
        self._temp_bank_stats_cells = dict()    # bank -> dict[str -> tk.Label]
        self._volt_stats_cells = dict()         # str -> tk.Label
        self._temp_stats_cells = dict()         # str -> tk.Label
        self._relay_state_cells = dict()        # str -> tk.Label

        self._setup()

        # TODO: REMOVE
        self.test_update()
        self._bind_key_press()

    ###############
    # Key Presses #
    ###############

    def _bind_key_press(self):
        self._root.bind("<KeyPress>", self._key_press)

    def _key_press(self, e):
        match e.char:
            case 'r' | 'R':
                self._monitor_state = BmsTable.RANGE_STATE
                self._state_cells["monitor"].config(text="Range")
            case 's' | 'S':
                self._monitor_state = BmsTable.STANDARD_STATE
                self._state_cells["monitor"].config(text="Standard")
            case 'd' | 'D':
                self._monitor_state = BmsTable.DEVIATION_STATE
                self._state_cells["monitor"].config(text="Deviation")

    ################
    # Update Table #
    ################

    # TODO: Remove test
    def test_update(self) -> None:
        self._mcd.bms.state = "Standard"

        for bank in range(1, accumulator.NUM_BANKS + 1):
            for cell in range(1, accumulator.NUM_CELLS_PER_BANK + 1):
                self._mcd.bms.temp[(bank, cell)] = random.random() * 60
        
        for bank in range(1, accumulator.NUM_BANKS + 1):
            for cell in range(1, accumulator.NUM_CELLS_PER_BANK + 1):
                self._mcd.bms.voltage[(bank, cell)] = random.random() * (4.2 - 2.5) + 2.5
        
        relay_open = "open"
        relay_closed = "close"
        self._mcd.bms.relay_state = {
            "shutdown": relay_open,
            "air+": relay_open,
            "pre-charge": relay_closed
        }
    
    def update(self) -> None:
        self._update_states_table()
        self._update_volt_temp_table()
        self._update_volt_temp_stats_table()
        self._update_relay_state_table()

        self._root.after(BmsTable.UPDATE_INTERVAL, self.update)

        return
        self._update_shutdown_state_table()
        
        self._update_IVT_table()
        self._update_charger_data_table()
        self._update_cell_balance_data_table()

        self._root.after(self.__update_interval, self.update)

    ################
    # Create Table #
    ################

    def _create_table_frame(self):
        frame = VerticalScrolledFrame(self._root)
        frame.pack(expand=True, fill=tk.BOTH)
        return frame.interior

    def _setup(self):
        row = 0
        self._create_cell(0, -1, "", border=False, wsf=0.3)
        row = self._create_blank_row(row)
        row = self._create_states_table(row)
        row = self._create_blank_row(row)
        row = self._create_volt_temp_table(row)
        row = self._create_blank_row(row)
        row = self._create_volt_temp_stats_table(row)
        # row = self._create_blank_row(row)
        # row = self._create_shutdown_state_table(row)
        row = self._create_blank_row(row)
        row = self._create_relay_state_table(row)
        # row = self._create_blank_row(row)
        # row = self._create_IVT_table(row)
        # row = self._create_blank_row(row)
        # row = self._create_charger_data_table(row)
        # row = self._create_blank_row(row)
        # row = self._create_cell_balance_data_table(row)
        # row = self._create_blank_row(row)

    ##############################
    # BMS / Monitor States Table #
    ##############################

    def _update_states_table(self):
        self._state_cells["monitor"].config(text=self._monitor_state.title())
        self._state_cells["bms"].config(text=self._mcd.bms.state)

    def _create_states_table(self, row):
        # Header
        self._create_cell(row, 0, "State", rs = 2)
        self._create_cell(row, 1, "Monitor State", cs = 2)
        self._create_cell(row, 3, "BMS State", cs = 2)
        row += 1

        # Body
        self._state_cells["monitor"] = self._create_cell(row, 1, "", cs=2)
        self._state_cells["bms"] = self._create_cell(row, 3, "", cs=2)
        row += 1

        return row

    #######################
    # Volt-Temp Cell Data #
    #######################

    def _update_volt_temp_table(self):
        resolution = 4

        # Voltage
        for bank in range(1, accumulator.NUM_BANKS + 1):
            for cell in range(1, accumulator.NUM_CELLS_PER_BANK + 1):
                value = self._mcd.bms.voltage.get((bank, cell), '-')
                bg = BmsTable.COLOR_DEFAULT
                if value != '-':
                    value = round(value, resolution)
                    if self._monitor_state == BmsTable.RANGE_STATE:
                        if value < accumulator.CELL_VOLT_MIN or value > accumulator.CELL_VOLT_MAX:
                            bg = BmsTable.COLOR_OUT_OF_RANGE
                    if self._monitor_state == BmsTable.DEVIATION_STATE:
                        if accumulator.CELL_VOLT_MIN <= value <= accumulator.CELL_VOLT_MAX:
                            alpha = (value - accumulator.CELL_VOLT_MIN) / accumulator.CELL_VOLT_RANGE
                            alpha = min(alpha + 0.1, 1)
                            bg = self._rgb_mix(BmsTable.COLOR_DEVIATION_VOLT, BmsTable.COLOR_DEFAULT, alpha)
                self._volt_cells[(bank, cell)].config(text=value, background=bg)

        # Temperature
        for bank in range(1, accumulator.NUM_BANKS + 1):
            for cell in range(1, accumulator.NUM_CELLS_PER_BANK + 1):
                value = self._mcd.bms.temp.get((bank, cell), '-')
                bg = BmsTable.COLOR_DEFAULT
                if value != '-':
                    value = round(value, resolution)
                    if self._monitor_state == BmsTable.RANGE_STATE:
                        if value < accumulator.CELL_TEMP_CHARGE_MIN or value > accumulator.CELL_TEMP_DISCHARGE_MAX:
                            bg = BmsTable.COLOR_OUT_OF_RANGE
                        elif value > accumulator.CELL_TEMP_CHARGE_MAX:
                            bg = BmsTable.COLOR_WARNING
                    if self._monitor_state == BmsTable.DEVIATION_STATE:
                        if accumulator.CELL_TEMP_CHARGE_MIN <= value <= accumulator.CELL_TEMP_DISCHARGE_MAX:
                            cell_temp_range = accumulator.CELL_TEMP_DISCHARGE_MAX - accumulator.CELL_TEMP_CHARGE_MIN
                            alpha = (value - accumulator.CELL_TEMP_CHARGE_MIN) / cell_temp_range
                            alpha = min(alpha + 0.1, 1)
                            bg = self._rgb_mix(BmsTable.COLOR_DEVIATION_TEMP, BmsTable.COLOR_DEFAULT, alpha)
                self._temp_cells[(bank, cell)].config(text=value, background=bg)

    def _update_volt_temp_stats_table(self):
        resolution = 4

        # Voltage
        volt_stats = self._mcd.bms.voltage.get_stats()
        self._volt_stats_cells["mean"].config(text=round(volt_stats["mean"], resolution))
        self._volt_stats_cells["min"].config(text=round(volt_stats["min"], resolution))
        self._volt_stats_cells["max"].config(text=round(volt_stats["max"], resolution))
        self._volt_stats_cells["stdev"].config(text=round(volt_stats["stdev"], resolution))
        self._volt_stats_cells["range"].config(text=round(volt_stats["range"], resolution))
        self._volt_stats_cells["sum"].config(text=round(volt_stats["sum"], resolution))

        # Bank voltage
        for bank in range(1, accumulator.NUM_BANKS + 1):
            bank_volt_stats = self._mcd.bms.voltage.get_stats(bank)
            self._volt_bank_stats_cells[(bank, "mean")].config(text=round(bank_volt_stats["mean"], resolution))
            self._volt_bank_stats_cells[(bank, "sum")].config(text=round(bank_volt_stats["sum"], resolution))

        # # Temperature
        temp_stats = self._mcd.bms.temp.get_stats()
        self._temp_stats_cells["mean"].config(text=round(temp_stats["mean"], resolution))
        self._temp_stats_cells["min"].config(text=round(temp_stats["min"], resolution))
        self._temp_stats_cells["max"].config(text=round(temp_stats["max"], resolution))
        self._temp_stats_cells["stdev"].config(text=round(temp_stats["stdev"], resolution))
        self._temp_stats_cells["range"].config(text=round(temp_stats["range"], resolution))
    
        # # Bank temperature
        for bank in range(1, accumulator.NUM_BANKS + 1):
            bank_temp_stats = self._mcd.bms.voltage.get_stats(bank)
            self._temp_bank_stats_cells[(bank, "mean")].config(text=round(bank_temp_stats["mean"], resolution))

    def _create_volt_temp_table(self, row):
        # Create header
        self._create_cell(row, 0, "Cell", rs = 2, wsf=2)
        for i in range(accumulator.NUM_BANKS):
            col = i * 2 + 1
            bank_num = i + 1
            self._create_cell(row, col, f"Bank {bank_num}", cs=2)
        for i in range(accumulator.NUM_BANKS * 2):
            col = i + 1
            txt = "V" if i % 2 == 0 else "T"
            self._create_cell(row + 1, col, txt)
        row += 2

        # Create body
        for i in range(accumulator.NUM_CELLS_PER_BANK):
            self._create_cell(row, 0, str(i + 1))
            for j in range(accumulator.NUM_BANKS):
                self._volt_cells[(j + 1, i + 1)] = self._create_cell(row, j * 2 + 1, "")
                self._temp_cells[(j + 1, i + 1)] = self._create_cell(row, j * 2 + 2, "")
            row += 1

        # Mean
        self._create_cell(row, 0, "Mean")
        for i in range(accumulator.NUM_BANKS):
            self._volt_bank_stats_cells[(i + 1, "mean")] = self._create_cell(row, i * 2 + 1, "")
            self._temp_bank_stats_cells[(i + 1, "mean")] = self._create_cell(row, i * 2 + 2, "")
        row += 1

        # Total
        self._create_cell(row, 0, "Total")
        for i in range(accumulator.NUM_BANKS):
            self._volt_bank_stats_cells[(i + 1, "sum")] = self._create_cell(row, i * 2 + 1, "")
            self._create_cell(row, i * 2 + 2, "-")
        row += 1

        return row
    
    def _create_volt_temp_stats_table(self, row):
        # Create header
        self._create_cell(row, 0, "All Banks")
        self._create_cell(row, 1, "Mean", cs=2)
        self._create_cell(row, 3, "Min", cs=2)
        self._create_cell(row, 5, "Max", cs=2)
        self._create_cell(row, 7, "Std. Deviation", cs=2)
        self._create_cell(row, 9, "Range", cs=2)
        self._create_cell(row, 11, "Total", cs=2)
        row += 1

        # Voltage stats
        self._create_cell(row, 0, "V")
        self._volt_stats_cells["mean"] = self._create_cell(row, 1, "", cs=2)
        self._volt_stats_cells["min"] = self._create_cell(row, 3, "", cs=2)
        self._volt_stats_cells["max"] = self._create_cell(row, 5, "", cs=2)
        self._volt_stats_cells["stdev"] = self._create_cell(row, 7, "", cs=2)
        self._volt_stats_cells["range"] = self._create_cell(row, 9, "", cs=2)
        self._volt_stats_cells["sum"] = self._create_cell(row, 11, "", cs=2)
        row += 1

        # Temperature stats
        self._create_cell(row, 0, "T")
        self._temp_stats_cells["mean"] = self._create_cell(row, 1, "", cs=2)
        self._temp_stats_cells["min"] = self._create_cell(row, 3, "", cs=2)
        self._temp_stats_cells["max"] = self._create_cell(row, 5, "", cs=2)
        self._temp_stats_cells["stdev"] = self._create_cell(row, 7, "", cs=2)
        self._temp_stats_cells["range"] = self._create_cell(row, 9, "", cs=2)
        self._temp_stats_cells["sum"] = self._create_cell(row, 11, "-", cs=2)
        row += 1

        return row

    ################
    # Relay States #
    ################

    def _update_relay_state_table(self):
        states = ["pre-charge", "air+", "shutdown"]
        for s in states:
            value = self._mcd.bms.relay_state.get(s, '-').title()
            bg = BmsTable.COLOR_DEFAULT
            if value != '-' and value == "Open":
                bg = BmsTable.COLOR_RELAY_OPEN
            self._relay_state_cells[s].config(text=value, background=bg)

    def _create_relay_state_table(self, row):
        # Headers
        self._create_cell(row, 0, "Relay", rs=2)
        self._create_cell(row, 1, "Pre-charge", cs=2)
        self._create_cell(row, 3, "Air+", cs=2)
        self._create_cell(row, 5, "Shutdown", cs=2)
        row += 1

        # Body
        self._relay_state_cells["pre-charge"] = self._create_cell(row, 1, "", cs=2)
        self._relay_state_cells["air+"] = self._create_cell(row, 3, "", cs=2)
        self._relay_state_cells["shutdown"] = self._create_cell(row, 5, "", cs=2)
        row += 1

        return row

    ##################
    # Helper Methods #
    ##################

    """
    Create cell at (row, col). Col == -1 reserved as margin.

    Args:
        row: Cell grid row.
        col: Cell grid column.
        txt: Cell text.
        border: Cell border.
        rs: Cell row span.
        cs: Cell column span.
        wsf: Cell width scale factor.
    Return:
        cell: Tkinter label object.
    """
    def _create_cell(self, row: int, col: int, txt: str, border: bool = True, 
                     rs : int = 1, cs : int = 1, wsf : float = 1):
        bd_width = 1 if border else 0
        bd = 1 if border else 0
        cell = tk.Label(self._frame, text=txt, borderwidth=bd_width, width=int(BmsTable.CELL_WIDTH * wsf * cs), 
                        height=BmsTable.CELL_HEIGHT * rs, bd=bd, relief="solid")
        cell.grid(row=row, column=col + 1, rowspan=rs, columnspan=cs, sticky="nesw")
        if not BmsTable.COLOR_DEFAULT:
            BmsTable.COLOR_DEFAULT = cell.cget("background")
        return cell
    
    def _create_blank_row(self, row):
        self._create_cell(row, 0, "", border=False)
        return row + 1
    
    def _rgb_mix(self, rgb_1: str, rgb_2: str, alpha: float):
        rgb_1 = (int(x) / 256 for x in self._root.winfo_rgb(rgb_1))
        rgb_2 = (int(x) / 256 for x in self._root.winfo_rgb(rgb_2))
        new_rgb = [int(alpha * c1 + (1 - alpha) * c2) for (c1, c2) in zip(rgb_1, rgb_2)]
        return self._rgb_dec_to_hex(new_rgb)
    
    def _rgb_dec_to_hex(self, dec: tuple[int, int, int]) -> str:
        return f"#{format(dec[0], '02x')}{format(dec[1], '02x')}{format(dec[2], '02x')}"
    
    def _rgb_hex_to_dec(self, hex: str) -> tuple[int, int, int]:
        return ((int(hex[1:3], 16), int(hex[3:5], 16), int(hex[5:7], 16)))