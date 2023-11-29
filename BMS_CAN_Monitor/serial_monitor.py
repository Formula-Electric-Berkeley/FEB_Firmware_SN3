from graph import Graph
from gui import GUI
from serial_connection import Serial_Connection
from serial_data import Serial_Data
from bms_table import BmsTable
import threading

def main():
    vg = Graph.visualize_graph()
    sd = Serial_Data()
    # sc = Serial_Connection(sd)
    gui = GUI()
    tb = BmsTable(gui.root, sd, 50)
    gui.root.after(0, tb.update)
    # threading.Thread(target=sc.read_data).start()
    # if vg:
        # Graph(sd, 50, 30)
    gui.root.mainloop()

if __name__ == "__main__":
    main()
