from app_helper import create_cell, create_empty_row
import customtkinter as ctk
import bms_data

class GeneralView(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        row = 0

        self.__inner_frame = ctk.CTkFrame(self)
        self.__inner_frame.pack()

        self.__charge_data_cells = dict()
        self.__charge_status_cells = dict()
        self.__relay_data_cells = dict()
        self.__gpio_sense_cells = dict()
        self.__ivt_data_cells = dict()

        row = self.__setup_charge_view(row)
        row = create_empty_row(self.__inner_frame, row)
        row = self.__setup_gpio_view(row)
        row = create_empty_row(self.__inner_frame, row)
        row = self.__setup_ivt_view(row)

    def __setup_charge_view(self, row: int) -> int:
        # Charge data headers
        create_cell(self.__inner_frame, row=row, col=0, text="Charge Data", rs=2, wsf=2)
        create_cell(self.__inner_frame, row=row, col=1, text="Control", wsf=2)
        create_cell(self.__inner_frame, row=row, col=2, text="Max Current", wsf=2)
        create_cell(self.__inner_frame, row=row, col=3, text="Max Voltage", wsf=2)
        create_cell(self.__inner_frame, row=row, col=4, text="Output Current", wsf=2)
        create_cell(self.__inner_frame, row=row, col=5, text="Output Voltage", wsf=2)
        row += 1

        # Charge data
        self.__charge_data_cells["control"] = create_cell(self.__inner_frame, row=row, col=1, text="", wsf=2)
        self.__charge_data_cells["max_current"] = create_cell(self.__inner_frame, row=row, col=2, text="", wsf=2)
        self.__charge_data_cells["max_voltage"] = create_cell(self.__inner_frame, row=row, col=3, text="", wsf=2)
        self.__charge_data_cells["output-current"] = create_cell(self.__inner_frame, row=row, col=4, text="", wsf=2)
        self.__charge_data_cells["output-voltage"] = create_cell(self.__inner_frame, row=row, col=5, text="", wsf=2)
        row += 1

        # Charge status flag headers
        create_cell(self.__inner_frame, row=row, col=0, text="Charger Status Flags", rs=2, wsf=2)
        create_cell(self.__inner_frame, row=row, col=1, text="Hardware Failure", wsf=2)
        create_cell(self.__inner_frame, row=row, col=2, text="Temperature Protection", wsf=2)
        create_cell(self.__inner_frame, row=row, col=3, text="Input Voltage", wsf=2)
        create_cell(self.__inner_frame, row=row, col=4, text="Starting State", wsf=2)
        create_cell(self.__inner_frame, row=row, col=5, text="Communication State", wsf=2)
        row += 1

        # Charge status flags
        self.__charge_status_cells["hardware-failure"] = create_cell(self.__inner_frame, row=row, col=1, text="", wsf=2)
        self.__charge_status_cells["temperature-protection"] = create_cell(self.__inner_frame, row=row, col=2, text="", wsf=2)
        self.__charge_status_cells["input-voltage"] = create_cell(self.__inner_frame, row=row, col=3, text="", wsf=2)
        self.__charge_status_cells["starting-state"] = create_cell(self.__inner_frame, row=row, col=4, text="", wsf=2)
        self.__charge_status_cells["communication-state"] = create_cell(self.__inner_frame, row=row, col=5, text="", wsf=2)
        row += 1

        return row
    
    def __setup_gpio_view(self, row: int) -> int:
        # Relay data headers
        create_cell(self.__inner_frame, row=row, col=0, text="Relays", rs=2, wsf=2)
        create_cell(self.__inner_frame, row=row, col=1, text="Shutdown Relay", wsf=2)
        create_cell(self.__inner_frame, row=row, col=2, text="AIR+ Relay", wsf=2)
        create_cell(self.__inner_frame, row=row, col=3, text="Precharge Relay", wsf=2)
        row += 1

        # Relay data
        self.__relay_data_cells["bms-shutdown"] = create_cell(self.__inner_frame, row=row, col=1, text="", wsf=2)
        self.__relay_data_cells["air-plus"] = create_cell(self.__inner_frame, row=row, col=2, text="", wsf=2)
        self.__relay_data_cells["precharge"] = create_cell(self.__inner_frame, row=row, col=3, text="", wsf=2)
        row += 1

        # Sense data headers
        create_cell(self.__inner_frame, row=row, col=0, text="GPIO Sense", rs=2, wsf=2)
        create_cell(self.__inner_frame, row=row, col=1, text="AIR- Sense", wsf=2)
        create_cell(self.__inner_frame, row=row, col=2, text="AIR+ Sense", wsf=2)
        create_cell(self.__inner_frame, row=row, col=3, text="BMS Shutdown", wsf=2)
        create_cell(self.__inner_frame, row=row, col=4, text="IMD Shutdown", wsf=2)
        row += 1

        # Sense data
        self.__gpio_sense_cells["air-minus-sense"] = create_cell(self.__inner_frame, row=row, col=1, text="", wsf=2)
        self.__gpio_sense_cells["air-plus-sense"] = create_cell(self.__inner_frame, row=row, col=2, text="", wsf=2)
        self.__gpio_sense_cells["bms-shutdown"] = create_cell(self.__inner_frame, row=row, col=3, text="", wsf=2)
        self.__gpio_sense_cells["imd-shutdown"] = create_cell(self.__inner_frame, row=row, col=4, text="", wsf=2)
        row += 1

        return row

    def __setup_ivt_view(self, row: int) -> None:
        # IVT data headers
        create_cell(self.__inner_frame, row=row, col=0, text="IVT", rs=2, wsf=2)
        create_cell(self.__inner_frame, row=row, col=1, text="U1", wsf=2)
        create_cell(self.__inner_frame, row=row, col=2, text="U2", wsf=2)
        create_cell(self.__inner_frame, row=row, col=3, text="U3", wsf=2)
        create_cell(self.__inner_frame, row=row, col=4, text="I", wsf=2)
        row += 1

        # IVT data
        self.__ivt_data_cells["u1"] = create_cell(self.__inner_frame, row=row, col=1, text="", wsf=2)
        self.__ivt_data_cells["u2"] = create_cell(self.__inner_frame, row=row, col=2, text="", wsf=2)
        self.__ivt_data_cells["u3"] = create_cell(self.__inner_frame, row=row, col=3, text="", wsf=2)
        self.__ivt_data_cells["i"] = create_cell(self.__inner_frame, row=row, col=4, text="", wsf=2)
        row += 1

    def update_data(self, data: bms_data.BmsData):
        self.__gpio_sense_cells["air-minus-sense"].configure(text=data.state.sense.get("air-minus", ""))
        self.__gpio_sense_cells["air-plus-sense"].configure(text=data.state.sense.get("air-plus", ""))
        self.__gpio_sense_cells["bms-shutdown"].configure(text=data.state.sense.get("bms-shutdown", ""))
        self.__gpio_sense_cells["imd-shutdown"].configure(text=data.state.sense.get("imd-shutdown", ""))


        self.__relay_data_cells["bms-shutdown"].configure(text=data.state.relay.get("bms-shutdown", ""))
        self.__relay_data_cells["air-plus"].configure(text=data.state.relay.get("air-plus", ""))
        self.__relay_data_cells["precharge"].configure(text=data.state.relay.get("precharge", ""))

        self.__ivt_data_cells["u1"].configure(text=str(data.ivt.voltage_1))
        self.__ivt_data_cells["u2"].configure(text=str(data.ivt.voltage_2))
        self.__ivt_data_cells["u3"].configure(text=str(data.ivt.voltage_3))
        self.__ivt_data_cells["i"].configure(text=str(data.ivt.current))
