from threading import Lock

lock = Lock()

class BmsData:
    def __init__(self):
        self.cell_data = CellData()
        self.state = BmsState()
    
    def store_message(self, message: list[str]):
        self.cell_data.store_message(message)
        self.state.store_message(message)

class SerialData:
    def store_message(self, message: str) -> None:
        raise NotImplementedError("Error: store_message method not implemented")
    
class CellData(SerialData):
    def __init__(self):
        self.cell = dict()  # (bank, cell) : {voltage, temperature, voltage-fault, temperature-fault}
    
    def store_message(self, message: list[str]) -> None:
        if message[0] == "cell":
            key = (int(message[1]), int(message[2]))
            voltage = round(int(message[3]) * 1e-3, 3)
            temperature = round(int(message[4]) * 1e-1, 3)
            voltage_fault = int(message[5]) == 1
            temperature_fault = int(message[6]) == 1
            balance = int(message[7]) == 1
            value = {"voltage": voltage, "temperature": temperature,
                     "voltage-fault": voltage_fault, "temperature-fault": temperature_fault,
                     "balance": balance}

            lock.acquire()
            self.cell[key] = value
            lock.release()

class BmsState(SerialData):
    def __init__(self):
        self.bms_state = ""
    
    def store_message(self, message: list[str]) -> None:
        if message[0] == "state":
            lock.acquire()
            self.bms_state = message[1]
            lock.release()