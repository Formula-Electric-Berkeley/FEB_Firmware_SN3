from bms_data import BmsData
import serial, serial.tools.list_ports
import sys

class SerialConnection:
    BAUD_RATE = 115200
    BYTE_SIZE = serial.SEVENBITS
    PARITY = serial.PARITY_EVEN
    STOP_BITS = serial.STOPBITS_ONE

    def __init__(self, bms_data: BmsData):
        self.__bms_data = bms_data
        port = self.__get_port()
        self.__connection = self.__connect(port)
    
    def __get_port(self) -> str:
        while True:
            ports = serial.tools.list_ports.comports()
            if len(ports) == 0:
                print("Error: no active ports")
            else:
                ports_str = [p[0] for p in ports]
                print(f"\nPorts: {ports_str}")
                try:
                    port_index = int(input(f"Choose port [1, {len(ports)}]: ")) - 1
                    if 0 <= port_index < len(ports):
                        break
                    else:
                        print("Error: invalid port index")
                except KeyboardInterrupt:
                    sys.exit(1)
                except:
                    print("Error: invalid port index")
        return ports[port_index][0]
    
    def __connect(self, port: str) -> serial.Serial:
        try:
            serial_connection = serial.Serial(port, SerialConnection.BAUD_RATE, SerialConnection.BYTE_SIZE,
                                              SerialConnection.PARITY, SerialConnection.STOP_BITS)
            return serial_connection
        except:
            print("Error: invalid serial connection")
            sys.exit(1)

    def read_data(self):
        self.__connection.readline()    # Ignore initial read
        while True:
            try:
                message = self.__connection.readline().decode("utf-8")
                message = message.replace("\n", "").split()
                self.__bms_data.store_message(message)
            except KeyboardInterrupt:
                sys.exit(1)
            except:
                print("Error read:", message)
