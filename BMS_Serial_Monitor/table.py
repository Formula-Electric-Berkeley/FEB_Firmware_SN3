import constants
import tkinter as tk
from VerticalScrolledFrame import VerticalScrolledFrame

class Table:
    CELL_WIDTH = 8
    CELL_HEIGHT = 1

    """
    Atributes:
        __state_cells [dict]: name [str] -> cell [tk.Label]

        __volt_cells [dict]: (bank [int], cell [int]) -> cell [tk.Label]
        __temp_cells [dict]: (bank [int], cell [int]) -> cell [tk.Label]
        __volt_bank_stats_cells [dict]: (bank[int], type [str]) -> cell [tk.Label], processed voltage data for each bank
        __temp_bank_stats_cells [dict]: (bank[int], type [str]) -> cell [tk.Label], processed temperature data for each bank
        __volt_stats_cells [dict]: type [str] -> data [float]
        __temp_stats_cells [dict]: type [str] -> data [float]

        __relay_state_cells [dict]: type [str] -> cell [tk.Label]

        __shutdown_state_cells [dict]: type [str] -> cell [tk.Label]

        __ivt_data_cells [dict]: type [str] -> cell [tk.Label]

        __charger_data_cells [dict]: type [str] -> cell [tk.Label]

        __discharge_data_cells [dict]: type [str] -> cell [tk.Label]
    """
    def __init__(self, master):
        self.__master = master
        self.__frame = self.__create_table_frame()

        # States
        self.__state_cells = dict()        

        # self.__debug_standard = True
        # self.__debug_charge = False
        # self.__debug_discharge = False
        # self.__debug_shutdown = False

        # Volt-temp data
        self.__volt_cells = dict()
        self.__temp_cells = dict()
        self.__volt_bank_stats_cells = dict()
        self.__temp_bank_stats_cells = dict()
        self.__volt_stats_cells = dict()
        self.__temp_stats_cells = dict()

        # Relay states
        self.__relay_state_cells = dict()

        # Shutdown states
        self.__shutdown_state_cells = dict()

        # IVT data
        self.__ivt_data_cells = dict()

        # Charger data
        self.__charger_data_cells = dict()

        # Discharge data
        self.__discharge_data_cells = dict()

        self.__setup()

    def __create_table_frame(self):
        frame = VerticalScrolledFrame(self.__master)
        frame.pack(expand = True, fill = tk.BOTH)
        self.testFrame = frame
        return frame.interior

    def __setup(self):
        row = 0
        self.__create_cell(0, -1, "", border=False, wsf=0.3)
        row = self.__create_blank_row(row)
        row = self.__create_states_table(row)
        row = self.__create_blank_row(row)
        row = self.__create_volt_temp_table(row)
        row = self.__create_blank_row(row)
        row = self.__create_volt_temp_stats_table(row)
        row = self.__create_blank_row(row)
        row = self.__create_shutdown_state_table(row)
        row = self.__create_blank_row(row)
        row = self.__create_relay_state_table(row)
        row = self.__create_blank_row(row)
        row = self.__create_IVT_table(row)
        row = self.__create_blank_row(row)
        row = self.__create_charger_data_table(row)
        row = self.__create_blank_row(row)
        row = self.__create_discharge_data_table(row)
        row = self.__create_blank_row(row)

    #####################
    # Controller States #
    #####################

    def __create_states_table(self, row):
        # Header
        self.__create_cell(row, 0, "State", rs = 2)
        self.__create_cell(row, 1, "Monitor State", cs = 2)
        self.__create_cell(row, 3, "BMS State", cs = 2)
        row += 1

        # Body
        self.__state_cells["monitor"] = self.__create_cell(row, 1, "", cs=2)
        self.__state_cells["bms"] = self.__create_cell(row, 3, "", cs=2)
        row += 1

        return row
    
    #######################
    # Volt-Temp Cell Data #
    #######################

    def __create_volt_temp_table(self, row):
        # Create header
        self.__create_cell(row, 0, "Cell", rs = 2, wsf=2)
        for i in range(constants.NUM_BANKS):
            col = i * 2 + 1
            bank_num = i + 1
            self.__create_cell(row, col, f"Bank {bank_num}", cs=2)
        for i in range(constants.NUM_BANKS * 2):
            col = i + 1
            txt = "V" if i % 2 == 0 else "T"
            self.__create_cell(row + 1, col, txt)
        row += 2

        # Create body
        for i in range(constants.NUM_CELLS_PER_BANK):
            self.__create_cell(row, 0, str(i + 1))
            for j in range(constants.NUM_BANKS):
                self.__volt_cells[(j, i)] = self.__create_cell(row, j * 2 + 1, "")
                self.__temp_cells[(j, i)] = self.__create_cell(row, j * 2 + 2, "")
            row += 1

        # Mean
        self.__create_cell(row, 0, "Mean")
        for i in range(constants.NUM_BANKS):
            self.__volt_bank_stats_cells[(i, "mean")] = self.__create_cell(row, i * 2 + 1, "")
            self.__temp_bank_stats_cells[(i, "mean")] = self.__create_cell(row, i * 2 + 2, "")
        row += 1

        # Total
        self.__create_cell(row, 0, "Total")
        for i in range(constants.NUM_BANKS):
            self.__volt_bank_stats_cells[(i, "sum")] = self.__create_cell(row, i * 2 + 1, "")
            self.__create_cell(row, i * 2 + 2, "-")
        row += 1

        return row
    
    def __create_volt_temp_stats_table(self, row):
        # Create header
        self.__create_cell(row, 0, "All Banks")
        self.__create_cell(row, 1, "Mean", cs=2)
        self.__create_cell(row, 3, "Min", cs=2)
        self.__create_cell(row, 5, "Max", cs=2)
        self.__create_cell(row, 7, "Std. Deviation", cs=2)
        self.__create_cell(row, 9, "Range", cs=2)
        self.__create_cell(row, 11, "Total", cs=2)
        row += 1

        # Voltage stats
        self.__create_cell(row, 0, "V")
        self.__volt_stats_cells["mean"] = self.__create_cell(row, 1, "", cs=2)
        self.__volt_stats_cells["min"] = self.__create_cell(row, 3, "", cs=2)
        self.__volt_stats_cells["max"] = self.__create_cell(row, 5, "", cs=2)
        self.__volt_stats_cells["std_dev"] = self.__create_cell(row, 7, "", cs=2)
        self.__volt_stats_cells["range"] = self.__create_cell(row, 9, "", cs=2)
        self.__volt_stats_cells["sum"] = self.__create_cell(row, 11, "", cs=2)
        row += 1

        # Temperature stats
        self.__create_cell(row, 0, "T")
        self.__temp_stats_cells["mean"] = self.__create_cell(row, 1, "", cs=2)
        self.__temp_stats_cells["min"] = self.__create_cell(row, 3, "", cs=2)
        self.__temp_stats_cells["max"] = self.__create_cell(row, 5, "", cs=2)
        self.__temp_stats_cells["std_dev"] = self.__create_cell(row, 7, "", cs=2)
        self.__temp_stats_cells["range"] = self.__create_cell(row, 9, "", cs=2)
        self.__temp_stats_cells["sum"] = self.__create_cell(row, 11, "", cs=2)
        row += 1

        return row

    ###################
    # Shutdown States #
    ###################

    def __create_shutdown_state_table(self, row):
        # Headers
        self.__create_cell(row, 0, "Shutdown", rs=2)
        self.__create_cell(row, 1, "BMS", cs=2)
        self.__create_cell(row, 3, "IMD", cs=2)
        self.__create_cell(row, 5, "TSMS", cs=2)
        row += 1

        # Body
        self.__shutdown_state_cells["bms"] = self.__create_cell(row, 1, "", cs=2)
        self.__shutdown_state_cells["imd"] = self.__create_cell(row, 3, "", cs=2)
        self.__shutdown_state_cells["tsms"] = self.__create_cell(row, 5, "", cs=2)
        row += 1

        return row
    
    ################
    # Relay States #
    ################

    def __create_relay_state_table(self, row):
        # Headers
        self.__create_cell(row, 0, "Relay", rs=2)
        self.__create_cell(row, 1, "Pre-charge Relay", cs=2)
        self.__create_cell(row, 3, "Air+ Relay", cs=2)
        row += 1

        # Body
        self.__relay_state_cells["pre_charge"] = self.__create_cell(row, 1, "", cs=2)
        self.__relay_state_cells["air_plus"] = self.__create_cell(row, 3, "", cs=2)
        row += 1

        return row
    
    ############
    # IVT Data #
    ############

    def __create_IVT_table(self, row):
        # Headers
        self.__create_cell(row, 0, "IVT Data", rs=2)
        self.__create_cell(row, 1, "U1 [Name]", cs=2)
        self.__create_cell(row, 3, "U2 [Name]", cs=2)
        self.__create_cell(row, 5, "U3 [Name]", cs=2)
        self.__create_cell(row, 7, "I1 [Name]", cs=2)
        row += 1

        # Body
        self.__ivt_data_cells["u1"] = self.__create_cell(row, 1, "", cs=2)
        self.__ivt_data_cells["u2"] = self.__create_cell(row, 3, "", cs=2)
        self.__ivt_data_cells["u3"] = self.__create_cell(row, 5, "", cs=2)
        self.__ivt_data_cells["i1"] = self.__create_cell(row, 7, "", cs=2)

        row += 1

        return row

    ################
    # Charger Data #
    ################

    def __create_charger_data_table(self, row):
        # Header 1
        self.__create_cell(row, 0, "Charger Data", rs=2)
        self.__create_cell(row, 1, "State (On / Off)", cs=2)
        self.__create_cell(row, 3, "Max Current", cs=2)
        self.__create_cell(row, 5, "Max Voltage", cs=2)
        self.__create_cell(row, 7, "Output Current", cs=2)
        self.__create_cell(row, 9, "Output Voltage", cs=2)
        row += 1

        # Body 1
        self.__charger_data_cells["state"] = self.__create_cell(row, 1, "", cs=2)
        self.__charger_data_cells["max_current"] = self.__create_cell(row, 3, "", cs=2)
        self.__charger_data_cells["max_voltage"] = self.__create_cell(row, 5, "", cs=2)
        self.__charger_data_cells["output_current"] = self.__create_cell(row, 7, "", cs=2)
        self.__charger_data_cells["output_voltage"] = self.__create_cell(row, 9, "", cs=2)
        row += 1

        # Header 2
        self.__create_cell(row, 0, "Charger Status Flags", rs=2)
        self.__create_cell(row, 1, "Hardware Failure", cs=2)
        self.__create_cell(row, 3, "Temperature Protection", cs=2)
        self.__create_cell(row, 5, "Input Voltage", cs=2)
        self.__create_cell(row, 7, "Starting State", cs=2)
        self.__create_cell(row, 9, "Communication State", cs=2)
        row += 1

        # Body 2
        self.__charger_data_cells["hardware_failure"] = self.__create_cell(row, 1, "", cs=2)
        self.__charger_data_cells["temperature_protection"] = self.__create_cell(row, 3, "", cs=2)
        self.__charger_data_cells["input_voltage"] = self.__create_cell(row, 5, "", cs=2)
        self.__charger_data_cells["starting_state"] = self.__create_cell(row, 7, "", cs=2)
        self.__charger_data_cells["communication_state"] = self.__create_cell(row, 9, "", cs=2)
        row += 1

        return row
    
    ##################
    # Discharge Data #
    ##################

    def __create_discharge_data_table(self, row):
        # Headers
        self.__create_cell(row, 0, "Discharge Data", rs=2)
        self.__create_cell(row, 1, "Target Voltage", cs=2)
        self.__create_cell(row, 3, "No. Cells Discharging", cs=2)
        row += 1

        # Body
        self.__discharge_data_cells["target_voltage"] = self.__create_cell(row, 1, "", cs=2)
        self.__discharge_data_cells["num_cells_discharging"] = self.__create_cell(row, 3, "", cs=2)
        row += 1

        return row

    ##################
    # Helper Methods #
    ##################

    """
    Create cell at (row, col). Col == -1 reserved as margin.

    Args:
        row [int]: Cell grid row.
        col [int]: Cell grid column.
        txt [str]: Cell text.
        border [bool]: Cell border.
        rs [int]: Cell row span.
        cs [int]: Cell column span.
        wsf [float]: Cell width scale factor.
    Return:
        cell: Tkinter label object.
    """
    def __create_cell(self, row, col, txt, border=True, rs=1, cs=1, wsf=1):
        bd_width = 1 if border else 0
        bd = 1 if border else 0
        cell = tk.Label(self.__frame, text=txt, borderwidth=bd_width, width=int(Table.CELL_WIDTH * wsf * cs), 
                        height=Table.CELL_HEIGHT * rs, bd=bd, relief="solid")
        cell.grid(row=row, column=col + 1, rowspan=rs, columnspan=cs, sticky="nesw")
        return cell
    
    def __create_blank_row(self, row):
        self.__create_cell(row, 0, "", border=False)
        return row + 1
