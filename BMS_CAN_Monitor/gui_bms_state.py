from can_data import MonitorCanData
import constants
import customtkinter as ctk

class BmsState(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        inner_frame = ctk.CTkFrame(self, fg_color="transparent")
        inner_frame.pack(pady=10)

        # BMS State
        bms_state_frame = ctk.CTkFrame(inner_frame, fg_color="transparent")
        bms_state_frame.grid(row=0, column=0)
        ctk.CTkLabel(bms_state_frame, text="BMS:", width=50).grid(row=0, column=0)
        self._bms_state_text = ctk.CTkLabel(bms_state_frame, width=100, text="Precharge", fg_color="#3669A0",
                                            corner_radius=5)
        self._bms_state_text.grid(row=0, column=1)

        # Monitor State
        monitor_state_frame = ctk.CTkFrame(inner_frame, fg_color="transparent")
        monitor_state_frame.grid(row=0, column=1)
        ctk.CTkLabel(monitor_state_frame, text="Monitor:", width=75).grid(row=0, column=0)
        self._monitor_state_text = ctk.CTkLabel(monitor_state_frame, width=100, text="", fg_color="#3669A0",
                                            corner_radius=5)
        self._monitor_state_text.grid(row=0, column=1)

        # Progress Bar
        self._charge_progress = ctk.CTkProgressBar(inner_frame, width=350, height=15)
        self._charge_progress.grid(row=0, column=2, padx=100)

        # Command BMS State
        cmd_state_frame = ctk.CTkFrame(inner_frame, fg_color="transparent")
        cmd_state_frame.grid(row=0, column=3)
        self._state_options = ctk.CTkSegmentedButton(cmd_state_frame, values=["Balance", "Charge", "Precharge"])
        self._state_options.grid(row=0, column=0)
        command_state_btn = ctk.CTkButton(cmd_state_frame, text="Command BMS State", command=self._command_bms_state)
        command_state_btn.grid(row=0, column=1, padx=(10, 0))
    
    def update_data(self, mcd: MonitorCanData, monitor_state: str) -> None:
        # Update states
        self._monitor_state_text.configure(text=monitor_state.title())
        self._bms_state_text.configure(text=mcd.bms.state.title())

        # Update charge progress bar
        avg_voltage = mcd.bms.voltage.get_stats()["mean"]
        charge_percentage = (avg_voltage - constants.CELL_VOLT_MIN) / constants.CELL_VOLT_RANGE
        charge_percentage = max(min(charge_percentage, 1), 0)
        self._charge_progress.set(charge_percentage)
    
    def _command_bms_state(self) -> None:
        state = self._state_options.get().lower()
        match state:
            case "balance":
                print("Balance")
            case "charge":
                print("Charge")
            case "precharge":
                print("Precharge")
        