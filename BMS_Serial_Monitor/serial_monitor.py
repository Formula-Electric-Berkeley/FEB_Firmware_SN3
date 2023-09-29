from graph import Graph
from gui import GUI
from serial_connection import Serial_Connection
from table import Table
import threading

stopThread = False

def main():
    sc = Serial_Connection()
    gui = GUI()
    tb = Table(gui.root)
    graph = Graph()

    gui.root.mainloop()

    # TODO: Add thread to update volt-temp table stats

    global stopThread
    stopThread = True

if __name__ == "__main__":
    main()