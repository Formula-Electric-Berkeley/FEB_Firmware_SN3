from bank_statistic import BankStatistic
import ctypes
import constants
import feb_can_id
import PCANBasic

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
    STATE_MAP = {
        0: "Precharge",
        1: "Charge",
        2: "Balance",
        3: "Drive",
        4: "Shutdown"
    }

    def __init__(self):
        self.state = ""
        self.relay_state = dict()       # str -> str
        self.voltage = BankStatistic()
        self.balance = dict()           # bank -> list[bool]
        self.balance_volt = 0
        self.temp = BankStatistic()
        self.enabled_temp = dict()      # bank -> list[bool]

    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        match message.ID.value:
            case feb_can_id.BMS_TEMPERATURE:
                data = list(message.DATA)
                key = (data[0], data[1])
                value_upper = -(data[2] & 0x80) + (data[2] & 0x7F)
                value = ((value_upper << 8) + data[3]) * 10**-4
                self.temp[key] = value
            case feb_can_id.BMS_ENABLED_TEMPERATURE_SENSORS:
                data = list(message.DATA)
                key = data[0]
                raw_value = (data[1] << 16) + (data[2] << 8) + data[3]
                value = [((raw_value >> i) & 1) == 1 for i in range(constants.NUM_CELLS_PER_BANK)]
                self.enabled_temp[key] = value
            case feb_can_id.BMS_VOLTAGE:
                data = list(message.DATA)
                key = (data[0], data[1])
                value = ((data[2] << 8) + data[3]) * 10**-4
                self.voltage[key] = value
            case feb_can_id.BMS_STATE:
                data = list(message.DATA)
                self.state = self._state_map(data[0])
                self.relay_state = self._relay_state_map()
            case feb_can_id.BMS_BALANCE:
                data = list(message.DATA)
                key = data[0]
                raw_value = (data[1] << 16) + (data[2] << 8) + data[3]
                value = [((raw_value >> i) & 1) == 1 for i in range(constants.NUM_CELLS_PER_BANK)]
                self.balance[key] = value
                self.balance_volt = ((data[4] << 8) + data[5]) * 10**-4
        
    def _state_map(self, value : int) -> str:
        return BmsCanData.STATE_MAP[value]
    
    def _relay_state_map(self, value : int) -> dict[str, str]:
        relay_open = "open"
        relay_closed = "close"
        return {
            "shutdown": relay_closed if ((value >> 2) & 1) == 0 else relay_open,
            "air+": relay_closed if ((value >> 1) & 1) == 0 else relay_open,
            "pre-charge": relay_closed if (value & 1) == 0 else relay_open
        }

class ChargerCanData(CanData):
    VOLT_FACTOR = 0.1
    CURR_FACTOR = 0.1

    def __init__(self):
        # BMS -> Charger
        self.bms_max_voltage = 0
        self.bms_max_current = 0
        self.bms_control = 1

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
            "hardware_failure": status[0],
            "temperature_protection": status[1],
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
