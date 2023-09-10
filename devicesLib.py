import os, re
import time
import pyvisa
from ctypes import *
from sys import exit

## libraries
os.add_dll_directory(r'C:\Program Files\IVI Foundation\VISA\Win64\Bin')
libPAX1000 = cdll.LoadLibrary('TLPAX_64.dll')

class OZOpticsDevice(): ## generic
    def __init__(self, portAddr, timeout=30):
        rm = pyvisa.ResourceManager()
        self.instr = rm.open_resource(portAddr, write_termination="\r\n", read_termination="\r\n", timeout=timeout)
     
    def write(self, command):
        self.instr.write(command)
    
    def read(self):
        return self.instr.read()

    def query(self, command):
        response = ''
        if self.instr is not None:
            self.write(command)
            if command == 'RST': [self.read() for _ in range(2)]
            else:
                while True:
                    next_line = self.read()
                    if next_line == 'Done': break
                    else: response += f',{next_line}'
        return command, response[1:]
    
    def reset(self):
        return self.query('RST')
    
    def get_config(self):
        return self.query('CD')
    
class EPC400(): ## EPC400
    def __init__(self, portAddr, timeout=30000):
        rm = pyvisa.ResourceManager()
        self.instr = rm.open_resource(portAddr, write_termination='\r\n', 
                                      read_termination='\r\n', timeout=timeout)

    def write(self, command):
        self.instr.write(command)
    
    def read(self):
        return self.instr.read()

    def query(self, command):
        response = ''
        if self.instr is not None:
            self.write(command)
            if command == 'RST': [self.read() for _ in range(2)]
            else:
                while True:
                    next_line = self.read()
                    if next_line == 'Done': break
                    else: response += f',{next_line}'
        return command, response[1:]
    
    def identifyDevice(self):
        return '*IDN?', {
            'serial': self.getSerialNumber()[1],
            'revision': self.getVersion()[1]}
    
    def getSerialNumber(self):
        command, serial_number = self.query('SN?')
        if serial_number != "":
            serial_number = serial_number.split(",")[0].split(":")[1][1:]
        return command, serial_number

    def getVersion(self):
        command, version = self.query('VER?')
        if version != "":
            version = version.split(":")[1][1:]
        return command, version

    def getMode(self):
        command, response = self.query('M?')
        return command, response[-2:]
    
    def setMode(self, value):
        command, response  = self.query(f'M{str(value)}')
        return command, response

    def getVoltages(self):
        command, response = self.query('V?')
        dataV = re.findall(r"CH\d\s*([\d+-]+)", response)
        if len(dataV) != 4:
            raise RuntimeError('unexpected voltage query response: {}'.format(response))
        return [float(i)/1e3 for i in dataV]

    def setVoltage(self, channel, value):
        ## lower & upper limit
        # if value < -5.0: value = -5.0
        # elif value > 5.0: value = 5.0
        
        self.query('V{:d},{:04d}'.format(channel+1, int(value*1e3)))[0]
        self.query(f'MDC')[0] ## not sure why but we need this one
        return self.getVoltages()[channel]
    
    def setVoltages(self, value):
        for chan, volt in enumerate(value):
            self.setVoltage(chan, volt)
        return self.getVoltages()
    
    def closeDevice(self):
        return self.instr.close()
    
class PAX1000():
    def __init__(self, portAddr=c_char_p(b''), measMode=c_int(9), wavelength=c_double(1550e-9), scanRate = c_double(100)):
        self.instrHandler = c_ulong()
        self.deviceCount = c_int()
        self.manuName = c_char_p(b'')
        self.modelName = c_char_p(b'')
        self.serialNumber = c_char_p(b'')
        self.deviceAvailable = c_bool()
        self.latestScanID = c_int()

        self.portAddr = portAddr
        self.IDQuery, self.resetDevice = True, False
        self.measMode, self.wavelength, self.scanRate = measMode, wavelength, scanRate

        libPAX1000.TLPAX_findRsrc(self.instrHandler, byref(self.deviceCount))
        if self.deviceCount.value < 1 :
            print('no PAX1000 device found...')
            exit()

        libPAX1000.TLPAX_getRsrcName(self.instrHandler, 0, self.portAddr)
        if libPAX1000.TLPAX_init(self.portAddr, self.IDQuery, self.resetDevice, byref(self.instrHandler)) != 0:
            print('error with init...')
            exit()
        time.sleep(2)

        libPAX1000.TLPAX_setMeasurementMode(self.instrHandler, self.measMode)
        libPAX1000.TLPAX_setWavelength(self.instrHandler, self.wavelength)
        libPAX1000.TLPAX_setBasicScanRate(self.instrHandler, self.scanRate)
        time.sleep(5)
    
    def getCurrSetting(self):
        wavelength, mode, scanrate = c_double(), c_int(), c_double()
        libPAX1000.TLPAX_getWavelength(self.instrHandler, byref(wavelength))
        libPAX1000.TLPAX_getMeasurementMode(self.instrHandler, byref(mode))
        libPAX1000.TLPAX_getBasicScanRate(self.instrHandler, byref(scanrate))
        return '*CFG?', {
            'wavelength [nm]': wavelength.value*1e9,
            'mode': mode.value,
            'scan rate [Hz]': scanrate.value}
    
    def getSingleMeas(self):
        currID = c_int()
        datetime = c_int()
        s1, s2, s3 = c_double(), c_double(), c_double()
        power, powerPolarized, powerUnpolarized = c_double(), c_double(), c_double()
        dop, dolp, docp = c_double(), c_double(), c_double()
        azimuthal, ellipticity = c_double(), c_double()

        libPAX1000.TLPAX_getLatestScan(self.instrHandler, byref(currID))
        libPAX1000.TLPAX_getTimeStamp(self.instrHandler, currID.value, byref(datetime))
        libPAX1000.TLPAX_getStokesNormalized(self.instrHandler, currID.value, byref(s1), byref(s2), byref(s3))
        libPAX1000.TLPAX_getPower(self.instrHandler, currID.value, byref(power), byref(powerPolarized), byref(powerUnpolarized))
        libPAX1000.TLPAX_getDOP(self.instrHandler, currID.value, byref(dop), byref(dolp), byref(docp))
        libPAX1000.TLPAX_getPolarization(self.instrHandler, currID.value, byref(azimuthal), byref(ellipticity))
        
        libPAX1000.TLPAX_releaseScan(self.instrHandler, currID)
        self.latestScanID = currID
        return [int(datetime.value), float(s1.value), float(s2.value), float(s3.value), float(power.value), float(dop.value), float(azimuthal.value), float(ellipticity.value)]
    
    def getPower(self):
        currID = c_int()
        power, powerPolarized, powerUnpolarized = c_double(), c_double(), c_double()

        libPAX1000.TLPAX_getLatestScan(self.instrHandler, byref(currID))
        libPAX1000.TLPAX_getPower(self.instrHandler, currID.value, byref(power), byref(powerPolarized), byref(powerUnpolarized))
        
        libPAX1000.TLPAX_releaseScan(self.instrHandler, currID)
        self.latestScanID = currID
        return float(power.value)

    def closeDevice(self):
        return libPAX1000.TLPAX_close(self.instrHandler)