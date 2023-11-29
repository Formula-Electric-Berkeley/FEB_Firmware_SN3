import accumulator
from can_data import MonitorCanData
import tkinter as tk
from VerticalScrolledFrame import VerticalScrolledFrame

class BmsTable:
    # Cell size
    CELL_WIDTH = 8
    CELL_HEIGHT = 1

    # Colors
    COLOR_DEFAULT = ""
    COLOR_OUT_OF_RANGE = "#FF0000"

    # Monitor State
    STANDARD_STATE = "standard"
    RANGE_STATE = "range"
    BALANCE_STATE = "balance"

    # Update rate (ms)
    UPDATE_INTERVAL = 1000

    """
    Atributes:
        _state_cells [dict]: name [str] -> cell [tk.Label]

        _volt_cells [dict]: (bank [int], cell [int]) -> cell [tk.Label]
        _temp_cells [dict]: (bank [int], cell [int]) -> cell [tk.Label]
        _volt_bank_stats_cells [dict]: (bank[int], type [str]) -> cell [tk.Label], processed voltage data for each bank
        _temp_bank_stats_cells [dict]: (bank[int], type [str]) -> cell [tk.Label], processed temperature data for each bank
        _volt_stats_cells [dict]: type [str] -> data [float]
        _temp_stats_cells [dict]: type [str] -> data [float]
        _relay_state_cells [dict]: type [str] -> cell [tk.Label]
        _shutdown_state_cells [dict]: type [str] -> cell [tk.Label]
        _ivt_data_cells [dict]: type [str] -> cell [tk.Label]
        _charger_data_cells [dict]: type [str] -> cell [tk.Label]
        _discharge_data_cells [dict]: type [str] -> cell [tk.Label]
    """
    def __init__(self, root: tk.Tk, mcd: MonitorCanData):
        self._root = root
        self._mcd = mcd
        self._frame = self._create_table_frame()

        # Monitor State
        self.monitor_state = BmsTable.STANDARD_STATE

        # Cells
        self._state_cells = dict()
        self._volt_cells = dict()
        self._temp_cells = dict()
        self._volt_bank_stats_cells = dict()
        self._temp_bank_stats_cells = dict()
        self._volt_stats_cells = dict()
        self._temp_stats_cells = dict()
        self._relay_state_cells = dict()
        self._shutdown_state_cells = dict()
        self._ivt_data_cells = dict()
        self._charger_data_cells = dict()
        self._cell_balance_data_cells = dict()

        self._setup()
        self._bind_key_press()

    ###############
    # Key Presses #
    ###############

    def _bind_key_press(self):
        self._root.bind("<KeyPress>", self.__key_press)

    def _key_press(self, e):
        match e.char:
            case 'r' | 'R':
                self._range_state = True
                self._state_cells["monitor"].config(text="Range")
            case 's' | 'S':
                self._range_state = False
                self._state_cells["monitor"].config(text="Standard")

    ################
    # Update Table #
    ################

    def update(self):
        return

        self._update_states_table()
        self._update_volt_temp_table()
        self._update_volt_temp_stats_table()
        self._update_shutdown_state_table()
        self._update_relay_state_table()
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
        row = self._create_blank_row(row)
        row = self._create_shutdown_state_table(row)
        row = self._create_blank_row(row)
        row = self._create_relay_state_table(row)
        row = self._create_blank_row(row)
        row = self._create_IVT_table(row)
        row = self._create_blank_row(row)
        row = self._create_charger_data_table(row)
        row = self._create_blank_row(row)
        row = self._create_cell_balance_data_table(row)
        row = self._create_blank_row(row)

    #####################
    # Controller States #
    #####################

    def _update_states_table(self):
        state = self._mcd.bms.state
        self._state_cells["bms"].config(text=state)

    def _create_states_table(self, row):
        # Header
        self._create_cell(row, 0, "State", rs = 2)
        self._create_cell(row, 1, "Monitor State", cs = 2)
        self._create_cell(row, 3, "BMS State", cs = 2)
        row += 1

        # Body
        self._state_cells["monitor"] = self._create_cell(row, 1, "Standard", cs=2)
        self._state_cells["bms"] = self._create_cell(row, 3, "", cs=2)
        row += 1

        return row
    
    #######################
    # Volt-Temp Cell Data #
    #######################

    def _update_volt_temp_table(self):
        # Voltage
        for bank in range(accumulator.NUM_BANKS):
            for cell in range(accumulator.NUM_CELLS_PER_BANK):
                value = self.__serial_data.voltage.get((bank, cell), '-')
                bg = BmsTable.COLOR_DEFAULT
                if value != '-':
                    value = round(value, 3)
                    if self.__range_state and not accumulator.CELL_VOLTAGE_MIN <= value <= accumulator.CELL_VOLTAGE_MAX:
                        bg = BmsTable.COLOR_OUT_OF_RANGE
                    elif self.__range_state:
                        green = int((value - accumulator.CELL_VOLTAGE_MIN) / (accumulator.CELL_VOLTAGE_MAX - accumulator.CELL_VOLTAGE_MIN) * 135 + 120) % 256
                        rgb = (0, green, 0)
                        bg = "#%02x%02x%02x" % rgb 
                self.__volt_cells[(bank, cell)].config(text=value, background=bg)

        # Temperature
        for bank in range(accumulator.NUM_BANKS):
            for cell in range(accumulator.NUM_CELLS_PER_BANK):
                value = self.__serial_data.temperature.get((bank, cell), '-')
                bg = BmsTable.COLOR_DEFAULT
                if value != '-':
                    value = round(value, 3)
                    if self.__range_state and not accumulator.CELL_TEMPERATURE_MIN <= value <= accumulator.CELL_TEMPERATURE_MAX:
                        bg = BmsTable.COLOR_OUT_OF_RANGE
                self.__temp_cells[(bank, cell)].config(text=value, background=bg)

    def _update_volt_temp_stats_table(self):
        # Voltage
        volt_stats = self.__serial_data.voltage.get_stats()
        self._volt_stats_cells["mean"].config(text=volt_stats["mean"])
        self._volt_stats_cells["min"].config(text=volt_stats["min"])
        self._volt_stats_cells["max"].config(text=volt_stats["max"])
        self._volt_stats_cells["stdev"].config(text=volt_stats["stdev"])
        self._volt_stats_cells["range"].config(text=volt_stats["range"])
        self._volt_stats_cells["sum"].config(text=volt_stats["sum"])

        # Bank voltage
        for bank in range(accumulator.NUM_BANKS):
            bank_volt_stats = self.__serial_data.voltage.get_stats(bank=bank)
            self._volt_bank_stats_cells[(bank, "mean")].config(text=bank_volt_stats["mean"])
            self._volt_bank_stats_cells[(bank, "sum")].config(text=bank_volt_stats["sum"])

        # Temperature
        temp_stats = self.__serial_data.temperature.get_stats()
        self._temp_stats_cells["mean"].config(text=temp_stats["mean"])
        self._temp_stats_cells["min"].config(text=temp_stats["min"])
        self._temp_stats_cells["max"].config(text=temp_stats["max"])
        self._temp_stats_cells["stdev"].config(text=temp_stats["stdev"])
        self._temp_stats_cells["range"].config(text=temp_stats["range"])
    
        # Bank temperature
        for bank in range(accumulator.NUM_BANKS):
            bank_temp_stats = self._serial_data.temperature.get_stats(bank=bank)
            self._temp_bank_stats_cells[(bank, "mean")].config(text=bank_temp_stats["mean"])

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
                self._volt_cells[(j, i)] = self._create_cell(row, j * 2 + 1, "")
                self._temp_cells[(j, i)] = self._create_cell(row, j * 2 + 2, "")
            row += 1

        # Mean
        self._create_cell(row, 0, "Mean")
        for i in range(accumulator.NUM_BANKS):
            self._volt_bank_stats_cells[(i, "mean")] = self._create_cell(row, i * 2 + 1, "")
            self._temp_bank_stats_cells[(i, "mean")] = self._create_cell(row, i * 2 + 2, "")
        row += 1

        # Total
        self._create_cell(row, 0, "Total")
        for i in range(accumulator.NUM_BANKS):
            self._volt_bank_stats_cells[(i, "sum")] = self._create_cell(row, i * 2 + 1, "")
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

    ###################
    # Shutdown States #
    ###################

    def _update_shutdown_state_table(self):
        states = ["bms", "imd", "tsms"]
        for s in states:
            value = self.__serial_data.shutdown_state.get(s, "-")
            bg = BmsTable.COLOR_DEFAULT if value == 0 else BmsTable.COLOR_OUT_OF_RANGE
            self.__shutdown_state_cells[s].config(text=value, background=bg)

    def _create_shutdown_state_table(self, row):
        # Headers
        self._create_cell(row, 0, "Shutdown", rs=2)
        self._create_cell(row, 1, "BMS", cs=2)
        self._create_cell(row, 3, "IMD", cs=2)
        self._create_cell(row, 5, "TSMS", cs=2)
        row += 1

        # Body
        self._shutdown_state_cells["bms"] = self._create_cell(row, 1, "", cs=2)
        self._shutdown_state_cells["imd"] = self._create_cell(row, 3, "", cs=2)
        self._shutdown_state_cells["tsms"] = self._create_cell(row, 5, "", cs=2)
        row += 1

        return row
    
    ################
    # Relay States #
    ################

    def _update_relay_state_table(self):
        states = ["pre_charge", "air_plus"]
        for s in states:
            value = self.__serial_data.relay_state.get(s, "-")
            bg = BmsTable.COLOR_DEFAULT if value == 0 else BmsTable.COLOR_OUT_OF_RANGE
            self.__relay_state_cells[s].config(text=value, background=bg)

    def _create_relay_state_table(self, row):
        # Headers
        self._create_cell(row, 0, "Relay", rs=2)
        self._create_cell(row, 1, "Pre-charge Relay", cs=2)
        self._create_cell(row, 3, "Air+ Relay", cs=2)
        row += 1

        # Body
        self._relay_state_cells["pre_charge"] = self._create_cell(row, 1, "", cs=2)
        self._relay_state_cells["air_plus"] = self._create_cell(row, 3, "", cs=2)
        row += 1

        return row
    
    ############
    # IVT Data #
    ############

    def _update_IVT_table(self):
        states = ["u1", "u2", "u3", "i1"]
        for s in states:
            value = self.__serial_data.ivt_data.get(s, 0)
            self.__ivt_data_cells[s].config(text=value)

    def _create_IVT_table(self, row):
        # Headers
        self._create_cell(row, 0, "IVT Data", rs=2)
        self._create_cell(row, 1, "U1 [Name]", cs=2)
        self._create_cell(row, 3, "U2 [Name]", cs=2)
        self._create_cell(row, 5, "U3 [Name]", cs=2)
        self._create_cell(row, 7, "I1 [Name]", cs=2)
        row += 1

        # Body
        self._ivt_data_cells["u1"] = self._create_cell(row, 1, "", cs=2)
        self._ivt_data_cells["u2"] = self._create_cell(row, 3, "", cs=2)
        self._ivt_data_cells["u3"] = self._create_cell(row, 5, "", cs=2)
        self._ivt_data_cells["i1"] = self._create_cell(row, 7, "", cs=2)

        row += 1

        return row

    ################
    # Charger Data #
    ################

    def _update_charger_data_table(self):
        states = ["state", "max_current", "max_voltage", "output_current", "output_voltage",
                  "hardware_failure", "temperature_protection", "input_voltage", "starting_state",
                  "communication_state"]
        for s in states:
            value = self.__serial_data.charger_data.get(s, "-")
            self.__charger_data_cells[s].config(text=value)

    def _create_charger_data_table(self, row):
        # Header 1
        self._create_cell(row, 0, "Charger Data", rs=2)
        self._create_cell(row, 1, "State (On / Off)", cs=2)
        self._create_cell(row, 3, "Max Current", cs=2)
        self._create_cell(row, 5, "Max Voltage", cs=2)
        self._create_cell(row, 7, "Output Current", cs=2)
        self._create_cell(row, 9, "Output Voltage", cs=2)
        row += 1

        # Body 1
        self._charger_data_cells["state"] = self.__create_cell(row, 1, "", cs=2)
        self._charger_data_cells["max_current"] = self.__create_cell(row, 3, "", cs=2)
        self._charger_data_cells["max_voltage"] = self.__create_cell(row, 5, "", cs=2)
        self._charger_data_cells["output_current"] = self.__create_cell(row, 7, "", cs=2)
        self._charger_data_cells["output_voltage"] = self.__create_cell(row, 9, "", cs=2)
        row += 1

        # Header 2
        self._create_cell(row, 0, "Charger Status Flags", rs=2)
        self._create_cell(row, 1, "Hardware Failure", cs=2)
        self._create_cell(row, 3, "Temperature Protection", cs=2)
        self._create_cell(row, 5, "Input Voltage", cs=2)
        self._create_cell(row, 7, "Starting State", cs=2)
        self._create_cell(row, 9, "Communication State", cs=2)
        row += 1

        # Body 2
        self._charger_data_cells["hardware_failure"] = self._create_cell(row, 1, "", cs=2)
        self._charger_data_cells["temperature_protection"] = self._create_cell(row, 3, "", cs=2)
        self._charger_data_cells["input_voltage"] = self._create_cell(row, 5, "", cs=2)
        self._charger_data_cells["starting_state"] = self._create_cell(row, 7, "", cs=2)
        self._charger_data_cells["communication_state"] = self._create_cell(row, 9, "", cs=2)
        row += 1

        return row
    
    #####################
    # Cell Balance Data #
    #####################

    def _update_cell_balance_data_table(self):
        states = ["target_voltage", "num_cells_discharging"]
        for s in states:
            value = self._serial_data.cell_balance_data.get(s, "-")
            self._cell_balance_data_cells[s].config(text=value)

    def _create_cell_balance_data_table(self, row):
        # Headers
        self._create_cell(row, 0, "Discharge Data", rs=2)
        self._create_cell(row, 1, "Target Voltage", cs=2)
        self._create_cell(row, 3, "No. Cells Discharging", cs=2)
        row += 1

        # Body
        self._cell_balance_data_cells["target_voltage"] = self._create_cell(row, 1, "", cs=2)
        self._cell_balance_data_cells["num_cells_discharging"] = self._create_cell(row, 3, "", cs=2)
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
        self.__create_cell(row, 0, "", border=False)
        return row + 1
