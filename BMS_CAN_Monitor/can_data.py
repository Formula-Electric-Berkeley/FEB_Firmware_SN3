import ctypes
import feb_can_id
import PCANBasic

# TODO: Remove
"""
Files:
- can_data.py
- can_connection.py
- feb_can_id.py
- main.py

"""
# TODO: Remove

class CanData:
    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        raise NotImplementedError
    
class MonitorCanData(CanData):
    def __init__(self):
        self.bms = BmsCanData()
        self.charger = ChargerCanData()
        self.ivt = IvtCanData()
    
    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        self.bms.store_message(message)
        self.charger.store_message(message)
        self.ivt.store_message(message)


class BmsCanData(CanData):
    def __init__(self):
        self.state = None
        self.relay_state = None
        self.voltage = None
        self.balance = None
        self.temp = None
        self.enabled_temp = None

    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        match message.ID.value:
            case feb_can_id.BMS_TEMPERATURE:
                pass
            case feb_can_id.BMS_ENABLED_TEMPERATURE_SENSORS:
                pass
            case feb_can_id.BMS_VOLTAGE:
                pass
            case feb_can_id.BMS_STATE:
                pass
            case feb_can_id.BMS_BALANCE:
                pass

class ChargerCanData(CanData):
    VOLT_FACTOR = 0.1
    CURR_FACTOR = 0.1

    def __init__(self):
        # BMS -> Charger
        self.bms_max_voltage = 0
        self.bms_max_current = 0
        self.bms_control = 0

        # Charger -> BMS
        self.ccs_voltage = 0
        self.ccs_current = 0
        self._ccs_status = 0

    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        match message.ID.value:
            case feb_can_id.CHARGER_BMS:
                data = list(message.DATA)
                self.bms_max_voltage = ((data[0] << 8) + data[1]) * ChargerCanData.VOLT_FACTOR
                self.bms_max_current = ((data[2] << 8) + data[3]) * ChargerCanData.CURR_FACTOR
                self.bms_control = data[4]
            case feb_can_id.CHARGER_CCS:
                data = list(message.DATA)
                self.ccs_voltage = ((data[0] << 8) + data[1]) * ChargerCanData.VOLT_FACTOR
                self.ccs_current = ((data[2] << 8) + data[3]) * ChargerCanData.CURR_FACTOR
                self._ccs_status = data[4]
    
    @property
    def ccs_status(self):
        status = [(self._ccs_status >> (7 - i)) & 1 for i in range(8)]
        return {
            "hardware": status[0],
            "temperature": status[1],
            "input_voltage": status[2],
            "start_state": status[3],
            "comm_state": status[4]
        }

class IvtCanData(CanData):
    CURR_FACTOR = 10 ** -3
    VOLT_FACTOR = 10 ** -3

    def __init__(self):
        self._current = ctypes.c_int32(0)
        self._voltage_1 = ctypes.c_int32(0)
        self._voltage_2 = ctypes.c_int32(0)
        self._voltage_3 = ctypes.c_int32(0)

    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        dest = None
        match message.ID.value:
            case feb_can_id.IVT_CURRENT:
                dest = ctypes.pointer(self._current)
            case feb_can_id.IVT_VOLTAGE_1:
                dest = ctypes.pointer(self._voltage_1)
            case feb_can_id.IVT_VOLTAGE_2:
                dest = ctypes.pointer(self._voltage_2)
            case feb_can_id.IVT_VOLTAGE_3:
                dest = ctypes.pointer(self._voltage_3)
        if dest != None:
            src_arr = message.DATA[::-1]
            src_arr = (ctypes.c_uint8 * len(src_arr))(*src_arr)
            src = ctypes.byref(src_arr, 3 * ctypes.sizeof(ctypes.c_uint8))
            ctypes.memmove(dest, src, 4 * ctypes.sizeof(ctypes.c_uint8))
    
    @property
    def current(self):
        return self._current.value * IvtCanData.CURR_FACTOR
    
    @property
    def voltage_1(self):
        return self._voltage_1.value * IvtCanData.VOLT_FACTOR
    
    @property
    def voltage_2(self):
        return self._voltage_2.value * IvtCanData.VOLT_FACTOR
    
    @property
    def voltage_3(self):
        return self._voltage_3.value * IvtCanData.VOLT_FACTOR
