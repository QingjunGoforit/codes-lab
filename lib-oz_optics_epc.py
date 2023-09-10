
from .base import 

from ..utility import check_type_decorator, retry_on_exception
from enum import Enum


class OperatingModeEnum(Enum):
    DC = "DC"
    AC = "AC"


class OzOpticsEPCCommands(CommandsBase):
    def __init__(self, address):
        self.inst 

    def query(self, command: str) -> (str, str):
        response = ""
        if self.inst is not None:
            self.write(command)
            if command == "RESET":
                [self.read() for _ in range(2)]
            else:
                while True:
                    next_line = self.read()
                    if next_line == "Done":
                        break
                    else:
                        response += f",{next_line}"
        return command, response[1:]

    def identify(self) -> (str, dict):
        return "*IDN?", {
            "manufacturer": "OZ Optics Limited",
            "model_number": "EPC-OEM",
            "serial": self.get_serial_number()[1],
            "revision": self.get_version()[1],
        }

    def get_serial_number(self) -> (str, str):
        command, serial_number = self.query("SN?")
        if serial_number != "":
            serial_number = serial_number.split(",")[0].split(":")[1][1:]
        return command, serial_number

    def get_version(self) -> (str, str):
        command, version = self.query("VER?")
        if version != "":
            version = version.split(":")[1][1:]
        return command, version

    def reset(self) -> str:
        command, _ = self.query("RESET")
        return command

    def set_mode(self, mode) -> str:
        mode = self.as_enum(mode, OperatingModeEnum)
        return self.query(f"M{mode.value}")[0]

    def get_operating_mode(self) -> (str, OperatingModeEnum):
        command, operating_mode = self.query("M?")
        if operating_mode != "":
            operating_mode = OperatingModeEnum(operating_mode.split(",")[0].split(":")[1][1:])
        return command, operating_mode

    @check_type_decorator
    def set_voltages(self, voltage: float, channel: int) -> str:
        voltage_mv = voltage * 1000.0
        if channel < 1 or channel > 4:
            raise Exception(f"Channel number {channel} is out of range. Must be between 1 and 4.")
        if voltage_mv < -5000.0 or voltage_mv > 5000.0:
            raise Exception(f"Voltage {voltage} if out of range. Must be between -5.0 and +5.0 V.")
        response = self.query(f"V{channel},{voltage_mv:.0f}")[0]
        self.set_mode(OperatingModeEnum.DC)
        if self.check_state:
            self._check_voltages(voltage, channel)
        return response

    @retry_on_exception(max_attempts=2, allowed_exceptions=[ValueError])
    def get_voltages(self) -> (str, list):
        command, voltages = self.query("V?")
        if voltages != "":
            voltages = voltages.split(" ")
            voltages = [
                float(voltages[2]) / 1000,
                float(voltages[5]) / 1000,
                float(voltages[8]) / 1000,
                float(voltages[11]) / 1000,
            ]
        return command, voltages

    def _check_voltages(self, voltage: float, channel: int):
        actual = self.get_voltages()[1][channel - 1]
        if abs(voltage - actual) > 0.04:
            raise Exception(f"Attempted to set channel {channel} to {voltage}. Voltage set to {actual} instead.")
