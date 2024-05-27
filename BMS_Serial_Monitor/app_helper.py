import customtkinter as ctk
import tkinter as tk

COLOR_STANDARD = "#ececec"
COLOR_OUT_OF_RANGE = "#ff0000"
COLOR_BALANCE = "#91a9ff"

def create_cell(master, row: int, col: int, text: str, rs: int = 1, cs: int = 1, wsf: float | int = 1,
                    bd: int = 1) -> tk.Label:
    cell = tk.Label(master, text=text, width=int(8*wsf), height=1, relief="solid", bd=bd, background=COLOR_STANDARD)
    cell.grid(row=row, rowspan=rs, column=col, columnspan=cs, sticky="nesw")
    return cell

def update_cell_color(cell: tk.Label, color: str):
    cell.configure(bg=color)

def create_empty_row(master, row: int) -> int:
    cell = ctk.CTkLabel(master, text="", width=8, height=1)
    cell.grid(row=row, column=0)
    return row + 1
