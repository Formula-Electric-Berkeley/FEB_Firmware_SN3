from can_data import MonitorCanData
import customtkinter as ctk
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import time
import tkinter as tk

class IVT(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self._data_table = DataTable(self)
        self._data_table.pack(pady=15)

        self._current_graph = CurrentGraph(self, bg="#323232", borderwidth=2, relief="solid")
        self._current_graph.pack()
    
    def update_data(self, mcd: MonitorCanData) -> None:
        self._data_table.update_data(mcd)
        self._current_graph.update_data(mcd)
        self._current_graph.update_graph()

class DataTable(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        self._ivt_data = dict()     # str -> data cell
        self._setup_table()
    
    def update_data(self, mcd: MonitorCanData) -> None:
        self._ivt_data["u1"].configure(text=str(mcd.ivt.voltage_1))
        self._ivt_data["u2"].configure(text=str(mcd.ivt.voltage_2))
        self._ivt_data["u3"].configure(text=str(mcd.ivt.voltage_3))
        self._ivt_data["i1"].configure(text=str(mcd.ivt.current))

    def _setup_table(self) -> None:
        self._create_cell(row=0, col=0, text="IVT Data", rs=2, wsf=2)
        self._create_cell(row=0, col=1, text="U1", wsf=2)
        self._create_cell(row=0, col=2, text="U2", wsf=2)
        self._create_cell(row=0, col=3, text="U3", wsf=2)
        self._create_cell(row=0, col=4, text="I1", wsf=2)

        self._ivt_data["u1"] = self._create_cell(row=1, col=1, text="", wsf=2)
        self._ivt_data["u2"] = self._create_cell(row=1, col=2, text="", wsf=2)
        self._ivt_data["u3"] = self._create_cell(row=1, col=3, text="", wsf=2)
        self._ivt_data["i1"] = self._create_cell(row=1, col=4, text="", wsf=2)

    def _create_cell(self, row: int, col: int, text: str, rs: int = 1, cs: int = 1, wsf: float | int = 1,
                     bd: int = 1) -> tk.Label:
        cell = tk.Label(self, text=text, width=int(8*wsf), height=1, relief="solid", bd=bd, background="#323232",
                fg="white")
        cell.grid(row=row, rowspan=rs, column=col, columnspan=cs, sticky="nesw")
        return cell
    
class CurrentGraph(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        # Graph
        plt.rcParams.update({"figure.facecolor": "#323232", "xtick.color": "white", "ytick.color": "white",
                             "axes.labelcolor": "white", "grid.color": "black"})
        self._xdata = []
        self._ydata = []
        self._figure = Figure(figsize=(11, 4))
        self._plt = self._figure.add_subplot(111)
        self._canvas = FigureCanvasTkAgg(self._figure, self)
        self._canvas.draw()
        self._canvas.get_tk_widget().pack(pady=(0, 20))

        # Data
        self._start_time = time.time()

    def update_graph(self) -> None:
        self._plt.clear()
        self._plt.plot(self._xdata, self._ydata)
        self._plt.set_xlabel("Time / s")
        self._plt.set_ylabel("Current / A")
        self._figure.canvas.draw()

    def update_data(self, mcd: MonitorCanData) -> None:
        x = time.time() - self._start_time
        y = mcd.ivt.current
        self._xdata.append(x)
        self._ydata.append(y)
        if len(self._xdata) > 100:
            self._xdata.pop(0)
            self._ydata.pop(0)
