from threading import Lock

lock = Lock()

class BmsData:
    def __init__(self):
        self.cell_data = CellData()
        self.state = BmsState()
        self.ivt = IVTData()
        self.charge = ChargeData()
    
    def store_message(self, message: list[str]) -> bool:
        success = False
        success = self.cell_data.store_message(message) or success
        success = self.state.store_message(message) or success
        success = self.ivt.store_message(message) or success
        success = self.charge.store_message(message) or success
        return success

class SerialData:
    def store_message(self, message: str) -> bool:
        raise NotImplementedError("Error: store_message method not implemented")
    
class CellData(SerialData):
    def __init__(self):
        self.cell = dict()  # (bank, cell) : {voltage, temperature, voltage-fault, temperature-fault}
        self.stats = dict() # str : float
    
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
        elif message[1] == "accumulator-voltage":
            total_voltage_mV = int(message[1])
            min_voltage_mV = int(message[2])
            max_voltage_mV = int(message[3])

            lock.acquire()
            self.stats["total-voltage"] = round(total_voltage_mV * 1e-3, 3)
            self.stats["min-cell-voltage"] = round(min_voltage_mV * 1e-3, 3)
            self.stats["max-cell-voltage"] = round(max_voltage_mV * 1e-3, 3)
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
        self.data = dict()
        self.status = dict()

    def store_message(self, message: str) -> bool:
        success = False
        if message[0] == "charge":
            success = True

            max_voltage = round(int(message[1]) * 1e-1, 1)
            max_current = round(int(message[2]) * 1e-1, 1)
            control = int(message[3])
            operating_voltage = round(int(message[4]) * 1e-1, 1)
            operating_current = round(int(message[5]) * 1e-1, 1)
            status = int(message[6])

            lock.acquire()
            self.data["max-voltage"] = max_voltage
            self.data["max-current"] = max_current
            self.data["control"] = control
            self.data["operating-voltage"] = operating_voltage
            self.data["operating-current"] = operating_current

            self.status["hardware-failure"] = status & 1
            self.status["temperature"] = (status >> 1) & 1
            self.status["input-voltage"] = (status >> 2) & 1
            self.status["starting-state"] = (status >> 3) & 1
            self.status["communication-state"] = (status >> 4) & 1
            lock.release()

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
