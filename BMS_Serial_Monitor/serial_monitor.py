from gui import GUI
from serial_connection import Serial_Connection
from table import Table

def main():
    sc = Serial_Connection()
    gui = GUI()
    tb = Table(gui)

    gui.root.mainloop()

if __name__ == "__main__":
    main()