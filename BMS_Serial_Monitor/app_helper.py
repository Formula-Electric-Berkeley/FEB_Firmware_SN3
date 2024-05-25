import customtkinter as ctk
import tkinter as tk

def create_cell(master, row: int, col: int, text: str, rs: int = 1, cs: int = 1, wsf: float | int = 1,
                    bd: int = 1) -> tk.Label:
    cell = tk.Label(master, text=text, width=int(8*wsf), height=1, relief="solid", bd=bd)
    cell.grid(row=row, rowspan=rs, column=col, columnspan=cs, sticky="nesw")
    return cell

def create_empty_row(master, row: int) -> int:
    cell = ctk.CTkLabel(master, text="", width=8, height=1)
    cell.grid(row=row, column=0)
    return row + 1
