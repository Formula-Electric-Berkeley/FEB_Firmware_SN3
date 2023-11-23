import ctypes
from feb_can_id import *
import PCANBasic

class CanData:
    def __init__(self):
        self.bms = BmsCanData()
        self.charger = ChargerCanData()
        self.ivt = IvtCanData()

class BmsCanData:
    def __init__(self):
        self.state = None
        self.relay_state = None
        self.voltage = None
        self.balance = None
        self.temp = None
        self.enabled_temp = None

class ChargerCanData:
    def __init__(self):
        # BMS -> Charger
        self.bms_max_voltage = None
        self.bms_max_current = None
        self.bms_control = None

        # Charger -> BMS
        self.charger_voltage = None
        self.charger_current = None
        self.charger_status = None

    def store_message(self, message: PCANBasic.TPCANMsg):
        pass
    
    # def store_message(self, message: PCANBasic.TPCANMsg):
    #     match message.ID.value:
    #         case feb_can_id.FEB_CAN_ID_CHARGER_BMS:
    #             pass
    #         case feb_can_id.FEB_CAN_ID_CHARGER_CCS:
    #             pass
    
    # def _store_message_charger(self, message: PCANBasic.TPCANMsg):
    #     pass

class IvtCanData:
    def __init__(self):
        self.current = None
        self.voltage_1 = None
        self.voltage_2 = None
        self.voltage_3 = None
        self.temperature = None
        self.power = None
        self.current_counter = None
        self.energy_counter = None


