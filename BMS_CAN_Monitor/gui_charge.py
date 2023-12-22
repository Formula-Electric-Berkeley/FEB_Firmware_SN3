from can_data import MonitorCanData
import constants
import customtkinter as ctk
import tkinter as tk

class Charge(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self._data_frame = ChargeBalanceData(self)
        self._data_frame.pack()

        self._control_frame = ChargeControl(self)
        self._control_frame.pack(pady=(25, 0))
    
    def update_data(self, mcd: MonitorCanData) -> None:
        self._data_frame.update_data(mcd)
        
class ChargeControl(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self._current_slider = None
        self._current_label = None
        self._setup_controls()

    def _setup_controls(self) -> None:
        # Current selector
        num_steps = int((constants.CHARGE_MAX - constants.CHARGE_MIN) / constants.CHARGE_RESOLUTION)
        self._current_slider = ctk.CTkSlider(self, from_=constants.CHARGE_MIN, to=constants.CHARGE_MAX,
                                             height=25, width=800, number_of_steps=num_steps,
                                             command=self._update_current_slider, progress_color="#1f6aa5",
                                             button_color="#dddddd", button_hover_color="#999999")
        self._current_slider.grid(row=0, column=0, padx=(15, 0), pady=15)

        # Current label
        self._current_label = ctk.CTkLabel(self, width=75)
        self._current_label.grid(row=0, column=1, padx=(5, 15), pady=15)
        self._update_current_slider(self._current_slider.get())
    
    def _update_current_slider(self, value):
        text = "{:04.1f} A".format(value)
        self._current_label.configure(text=text)
        
class ChargeBalanceData(ctk.CTkFrame):
    COLOR_STANDARD = "#323232"

    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        self._charge_data_cell = dict()     # str -> tk.Label
        self._status_flag_cell = dict()     # str -> tk.Label
        self._balance_data_cell = dict()    # str -> tk.Label
        self._setup_table()
    
    def update_data(self, mcd: MonitorCanData) -> None:
        # Charge data
        self._charge_data_cell["control"].configure(text=str(mcd.charger.bms_control))
        self._charge_data_cell["max_current"].configure(text=str(mcd.charger.bms_max_current))
        self._charge_data_cell["max_voltage"].configure(text=str(mcd.charger.bms_max_voltage))
        self._charge_data_cell["output_current"].configure(text=str(mcd.charger.ccs_current))
        self._charge_data_cell["output_voltage"].configure(text=str(mcd.charger.ccs_voltage))

        # Charge status flag
        status = mcd.charger.ccs_status
        self._status_flag_cell["hardware_failure"].configure(text=str(status["hardware_failure"]))
        self._status_flag_cell["temperature_protection"].configure(text=str(status["temperature_protection"]))
        self._status_flag_cell["input_voltage"].configure(text=str(status["input_voltage"]))
        self._status_flag_cell["start_state"].configure(text=str(status["start_state"]))
        self._status_flag_cell["comm_state"].configure(text=str(status["comm_state"]))

        # Balance data
        num_balance_cells = sum(sum(state) for state in mcd.bms.balance.values())
        self._balance_data_cell["target_volt"].configure(text=str(mcd.bms.balance_volt))
        self._balance_data_cell["num_balance_cells"].configure(text=str(num_balance_cells))
    
    def _setup_table(self) -> None:
        row = 0

        # Charge data headers
        self._create_cell(row=row, col=0, text="Charge Data", rs=2, wsf=2, pady=(15, 0), padx=(15, 0))
        self._create_cell(row=row, col=1, text="Control", wsf=2, pady=(15, 0))
        self._create_cell(row=row, col=2, text="Max Current", wsf=2, pady=(15, 0))
        self._create_cell(row=row, col=3, text="Max Voltage", wsf=2, pady=(15, 0))
        self._create_cell(row=row, col=4, text="Output Current", wsf=2, pady=(15, 0))
        self._create_cell(row=row, col=5, text="Output Voltage", wsf=2, pady=(15, 0), padx=(0, 15))
        row += 1

        # Charge data
        self._charge_data_cell["control"] = self._create_cell(row=row, col=1, text="", wsf=2)
        self._charge_data_cell["max_current"] = self._create_cell(row=row, col=2, text="", wsf=2)
        self._charge_data_cell["max_voltage"] = self._create_cell(row=row, col=3, text="", wsf=2)
        self._charge_data_cell["output_current"] = self._create_cell(row=row, col=4, text="", wsf=2)
        self._charge_data_cell["output_voltage"] = self._create_cell(row=row, col=5, text="", wsf=2, padx=(0, 15))
        row += 1

        # Status flag headers
        self._create_cell(row=row, col=0, text="Charger Status Flags", rs=2, wsf=2, padx=(15, 0))
        self._create_cell(row=row, col=1, text="Hardware Failure", wsf=2)
        self._create_cell(row=row, col=2, text="Temperature Protection", wsf=2)
        self._create_cell(row=row, col=3, text="Input Voltage", wsf=2)
        self._create_cell(row=row, col=4, text="Starting State", wsf=2)
        self._create_cell(row=row, col=5, text="Communication State", wsf=2, padx=(0, 15))
        row += 1

        # Status flags
        self._status_flag_cell["hardware_failure"] = self._create_cell(row=row, col=1, text="", wsf=2)
        self._status_flag_cell["temperature_protection"] = self._create_cell(row=row, col=2, text="", wsf=2)
        self._status_flag_cell["input_voltage"] = self._create_cell(row=row, col=3, text="", wsf=2)
        self._status_flag_cell["start_state"] = self._create_cell(row=row, col=4, text="", wsf=2)
        self._status_flag_cell["comm_state"] = self._create_cell(row=row, col=5, text="", wsf=2, padx=(0, 15))
        row += 1

        # Balance data headers
        row = self._create_empty_row(row)
        self._create_cell(row=row, col=0, text="Balance Data", rs=2, wsf=2, padx=(15, 0), pady=(0, 15))
        self._create_cell(row=row, col=1, text="Target Voltage", wsf=2)
        self._create_cell(row=row, col=2, text="No. Cells Balancing", wsf=2)
        row += 1

        # Balance data
        self._balance_data_cell["target_volt"] = self._create_cell(row=row, col=1, text="", wsf=2, pady=(0, 15))
        self._balance_data_cell["num_balance_cells"] = self._create_cell(row=row, col=2, text="", wsf=2, pady=(0, 15))

    def _create_cell(self, row: int, col: int, text: str, rs: int = 1, cs: int = 1, wsf: float | int = 1,
                        bd: int = 1, padx: int | tuple[int, int] = 0, pady: int | tuple[int, int] = 0) -> tk.Label:
        cell = tk.Label(self, text=text, width=int(8*wsf), height=1, relief="solid", bd=bd, 
                        background=self.COLOR_STANDARD, fg="white")
        cell.grid(row=row, rowspan=rs, column=col, columnspan=cs, sticky="nesw", padx=padx, pady=pady)
        return cell
    
    def _create_empty_row(self, row: int) -> int:
        cell = ctk.CTkLabel(self, text="", width=8, height=1)
        cell.grid(row=row, column=0)
        return row + 1
