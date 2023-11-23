import ctypes
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
    def __init__(self):
        self.state = None
        self.relay_state = None
        self.voltage = None
        self.balance = None
        self.temp = None
        self.enabled_temp = None

    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        match message.ID.value:
            case feb_can_id.IVT_CURRENT:
                ...
            case feb_can_id.IVT_VOLTAGE_1:
                ...
            case feb_can_id.IVT_VOLTAGE_2:
                ...
            case feb_can_id.IVT_VOLTAGE_3:
                ...

class ChargerCanData(CanData):
    def __init__(self):
        # BMS -> Charger
        self.bms_max_voltage = None
        self.bms_max_current = None
        self.bms_control = None

        # Charger -> BMS
        self.ccs_voltage = None
        self.ccs_current = None
        self.ccs_status = None

    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        match message.ID.value:
            case feb_can_id.CHARGER_BMS:
                ...
            case feb_can_id.CHARGER_CCS:
                ...

class IvtCanData(CanData):
    def __init__(self):
        self.current = None
        self.voltage_1 = None
        self.voltage_2 = None
        self.voltage_3 = None

    def store_message(self, message: PCANBasic.TPCANMsg) -> None:
        match message.ID.value:
            case feb_can_id.IVT_CURRENT:
                ...
            case feb_can_id.IVT_VOLTAGE_1:
                ...
            case feb_can_id.IVT_VOLTAGE_2:
                ...
            case feb_can_id.IVT_VOLTAGE_3:
                ...
