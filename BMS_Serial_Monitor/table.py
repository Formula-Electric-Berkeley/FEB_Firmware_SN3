import constants
import tkinter as tk
from tkinter import ttk
import random
from test import VerticalScrolledFrame

# TODO: Setup new table format
# TODO: Setup new modes
# TODO: Restructure class

# Format
# Row 0 - 2 cells wide
# Row 1+ - 1 cell wide

class Table:
    CELL_WIDTH = 8
    CELL_HEIGHT = 1

    """
    Atributes:
        __volt_cells [dict]: (bank [int], cell [int]) -> cell [tk.Label]
        __temp_cells [dict]: (bank [int], cell [int]) -> cell [tk.Label]
        __volt_bank_stats_cells [dict]: (bank[int], type [str]) -> cell [tk.Label], processed voltage data for each bank
        __temp_bank_stats_cells [dict]: (bank[int], type [str]) -> cell [tk.Label], processed temperature data for each bank
        __volt_stats_cells [dict]: type [str] -> data [float]
        __temp_stats_cells [dict]: type [str] -> data [float]
    """
    def __init__(self, gui):
        self.__gui = gui
        self.__frame = self.__create_table_frame()

        # BMS States
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

        self.__setup()

        # test
        row = 25
        for i in range(row, row + 10):
            self.__create_cell(i, 0, str(i))

    def __create_table_frame(self):
        # TODO: Remove frame bg
        frame = VerticalScrolledFrame(self.__gui.root)
        frame.pack(expand = True, fill = tk.BOTH)
        return frame.interior

        # frame = tk.Frame(self.__gui.root, bg="#ffffff")
        # frame.pack()
        # return frame

    def __setup(self):
        row = 0
        row = self.__create_blank_row(row)
        row = self.__create_volt_temp_table(row)
        row = self.__create_blank_row(row)
    
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
                self.__volt_cells[(j, i)] = self.__create_cell(row, j * 2 + 1, "V" + str(j) + str(i))
                self.__temp_cells[(j, i)] = self.__create_cell(row, j * 2 + 2, "T" + str(j) + str(i))
            row += 1

        # Mean
        self.__create_cell(row, 0, "Mean")
        for i in range(constants.NUM_BANKS):
            self.__volt_bank_stats_cells[(i, "mean")] = self.__create_cell(row, i * 2 + 1, "VM")
            self.__temp_bank_stats_cells[(i, "mean")] = self.__create_cell(row, i * 2 + 2, "TM")
        row += 1

        # Total
        self.__create_cell(row, 0, "Total")
        for i in range(constants.NUM_BANKS):
            self.__volt_bank_stats_cells[(i, "sum")] = self.__create_cell(row, i * 2 + 1, "VS")
            self.__create_cell(row, i * 2 + 2, "-")
        row += 1

        return row
    
    def __create_volt_temp_stats_table(self, row):
        pass


    ##################
    # Helper Methods #
    ##################

    """
    Args:
        row [int]: Cell grid row.
        col [int]: Cell grid column.
        txt [str]: Cell text.
        border [bool]: Cell border.
        rs [int]: Cell row span.
        cs [int]: Cell column span.
        wsf [int]: Cell width scale factor.
    Return:
        cell: Tkinter label object.
    """
    def __create_cell(self, row, col, txt, border=True, rs=1, cs=1, wsf=1):
        bd_width = 1 if border else 0
        bd = 1 if border else 0
        cell = tk.Label(self.__frame, text=txt, borderwidth=bd_width, width=Table.CELL_WIDTH * wsf * cs, 
                        height=Table.CELL_HEIGHT * rs, bd=bd, relief="solid")
        cell.grid(row=row, column=col, rowspan=rs, columnspan=cs, sticky="nesw")
        return cell
    
    def __create_blank_row(self, row):
        self.__create_cell(row, 0, "", border=False)
        return row + 1
