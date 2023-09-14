import serial, serial.tools.list_ports

class Serial_Connection:
    BAUDRATE = 115200
    BYTESIZE = serial.SEVENBITS
    PARITY = serial.PARITY_EVEN
    STOPBITS = serial.STOPBITS_ONE

    def __init__(self):
        ports = self.__get_active_ports()
        self.__display_active_ports(ports)
        port_str = self.__select_active_port(ports)
        self.connection = self.__connect(port_str)

    """
    Args:
        port_str: String name of serial port to connect to.
    Returns:
        Serial object.
    Raises:
        Exception: Unable to connect to serial port.
    """
    def __connect(self, port_str):
        try:
            serial_connection = serial.Serial(port_str, Serial_Connection.BAUDRATE, Serial_Connection.BYTESIZE, 
                                              Serial_Connection.PARITY, Serial_Connection.STOPBITS)
            return serial_connection
        except serial.SerialException as err:
            raise Exception("Cannot connect to serial port.")
    
    def __display_active_ports(self, ports):
        ports_str = [p[0] for p in ports]
        print(f"Active ports: {ports_str}")

    """
    Returns:
        List of serial ports.
    Raises:
        Exception: No active serial ports.
    """
    def __get_active_ports(self):
        ports = serial.tools.list_ports.comports()
        if len(ports) == 0:
            raise Exception("No active ports.")
        return ports

    """
    Args:
        ports: List of serial ports, non-empty.
    Returns:
        port_str: String name of port selected by user.
    """
    def __select_active_port(self, ports):
        port_index = -1
        while not 1 <= port_index <= len(ports):
            try:
                port_index = int(input(f"Choose port [1, {len(ports)}]: "))
            except:
                pass
        return ports[port_index - 1][0]