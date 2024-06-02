from threading import Lock

lock = Lock()

class BmsData:
    def __init__(self):
        self.cell_data = CellData()
        self.state = BmsState()
        self.ivt = IVTData()
    
    def store_message(self, message: list[str]) -> bool:
        success = False
        success = self.cell_data.store_message(message) or success
        success = self.state.store_message(message) or success
        success = self.ivt.store_message(message) or success
        return success

class SerialData:
    def store_message(self, message: str) -> bool:
        raise NotImplementedError("Error: store_message method not implemented")
    
class CellData(SerialData):
    def __init__(self):
        self.cell = dict()  # (bank, cell) : {voltage, temperature, voltage-fault, temperature-fault}
    
    def store_message(self, message: list[str]) -> bool:
        success = False
        if message[0] == "cell":
            success = True
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

        return success

class BmsState(SerialData):
    def __init__(self):
        self.bms_state = ""
        self.relay = dict()
        self.sense = dict()
    
    def store_message(self, message: list[str]) -> bool:
        success = False
        if message[0] == "state":
            success = True
            state = message[1]
            relay_value = {
                "bms-shutdown": int(message[2]),
                "air-plus": int(message[3]),
                "precharge": int(message[4])
            }
            sense_value = {
                "air-minus": message[5],
                "air-plus": message[6],
                "bms-shutdown": message[7],
                "imd-shutdown": message[8]
            }

            lock.acquire()
            self.bms_state = state
            self.relay = relay_value
            self.sense = sense_value
            lock.release()

        return success

class ChargeData(SerialData):
    def __init__(self):
        self.charge = dict()

    def store_message(self, message: str) -> bool:
        success = False
        if message[0] == "charge":
            pass
            # lock.acquire()
            # lock.release()

        return success

class IVTData(SerialData):
    def __init__(self):
        self.voltage_1 = 0
        self.voltage_2 = 0
        self.voltage_3 = 0
        self.current = 0

    def store_message(self, message: str) -> bool:
        success = False
        if message[0] == "IVT":
            success = True
            voltage_1 = round(int(message[1]) * 1e-3, 3)
            voltage_2 = round(int(message[2]) * 1e-3, 3)
            voltage_3 = round(int(message[3]) * 1e-3, 3)
            current = round(int(message[4]) * 1e-3, 3)

            lock.acquire()
            self.voltage_1 = voltage_1
            self.voltage_2 = voltage_2
            self.voltage_3 = voltage_3
            lock.release()

            self.voltage_1 = round(int(message[1]) * 1e-3, 3)
            self.voltage_2 = round(int(message[2]) * 1e-3, 3)
            self.voltage_3 = round(int(message[3]) * 1e-3, 3)
            self.current = round(int(message[4]) * 1e-3, 3)
        return success
