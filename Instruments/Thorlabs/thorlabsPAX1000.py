import os, time
import pyvisa
from ctypes import *

## libraries
os.add_dll_directory(r'C:\Program Files\IVI Foundation\VISA\Win64\Bin')
libPAX = cdll.LoadLibrary('TLPAX_64.dll')

## main
class thorlabsPAX1000():
    """
    tested with PAX1000IR2. not sure whether will work with other PAX1000
    we all hate thorlabs drivers. refer to TLPAX.h !! AFTER INSTALLING THE PAX SOFTWARE !!

    | property type | datatype | read/write | page in handbook | unit | description                                 |
    |---------------|----------|------------|------------------|------|---------------------------------------------|
    """

    def __init__(self, portAddr=c_char_p(b''), measMode=c_int(9), wavelength=c_double(1550e-9), scanRate=c_double(100)):
        """
        constructor
        measMode: int, wavelength: double, scanRate: double
        """
        self.instrHandler = c_ulong()
        self.deviceCount = c_int()
        self.manuName = c_char_p(b'')
        self.modelName = c_char_p(b'')
        self.serialNumber = c_char_p(b'')
        self.manufacturer = c_char_p(b'')
        self.deviceAvailable = c_bool()
        self.latestScanID = c_int()

        self.portAddr = portAddr
        self.IDQuery, self.resetDevice = True, False
        self.measMode, self.wavelength, self.scanRate = measMode, wavelength, scanRate


    def getResourceID(self):
        """
        get pyvisa resources
        """
        resMan = pyvisa.ResourceManager()
        return resMan.list_resources()


    def getDevsNum(self):
        """
        get PAXs available in the system
        """
        libPAX.TLPAX_findRsrc(self.handler, byref(self.deviceCount))
        return self.deviceCount.value


    def getDevModel(self):
        """
        get device model
        """
        libPAX.TLPAX_getRsrcInfo(0, 0, self.modelName, 0, 0, byref(self.deviceAvailable))
        libPAX.TLPAX_getRsrcInfo(0, 0, 0, 0, self.manufacturer, byref(self.deviceAvailable))
        libPAX.TLPAX_getRsrcInfo(0, 0, 0, self.serialNumber, 0, byref(self.deviceAvailable))
        print('availability: {}, model: {}, manufacturer: {}, serial number: {}'\
                .format(self.deviceAvailable.value, self.modelName.value, self.manufacturer.value, self.serialNumber.value))
        

    def initDev(self):
        """
        connect blindly
        
        todo: 
        method by address
        get max basic scan rate limit and use that as default
        benchmark the time delay
        """
        ## try to find any connected PAX
        libPAX.TLPAX_findRsrc(self.instrHandler, byref(self.deviceCount))
        if self.deviceCount.value < 1 :
            print('no PAX1000IR2 device found...')
            exit()

        ## if found, get the resource and connect to the first in the list
        libPAX.TLPAX_getRsrcName(self.instrHandler, 0, self.portAddr) 
        if libPAX.TLPAX_init(self.portAddr, self.IDQuery, self.resetDevice, byref(self.instrHandler)) != 0: 
            print('error with init...')
            exit()
        time.sleep(2)

        ## set default
        libPAX.TLPAX_setMeasurementMode(self.instrHandler, self.measMode) ## default: < 2 revs for one measurement, 2048 points for FFT
        libPAX.TLPAX_setWavelength(self.instrHandler, self.wavelength) ## default: 1550nm
        libPAX.TLPAX_setBasicScanRate(self.instrHandler, self.scanRate) ## default: 100Hz
        time.sleep(5)
    

    def getCurrSetting(self):
        """
        get current setting
        """
        wavelength, mode, scanrate = c_double(), c_int(), c_double()
        libPAX.TLPAX_getWavelength(self.instrHandler, byref(wavelength))
        libPAX.TLPAX_getMeasurementMode(self.instrHandler, byref(mode))
        libPAX.TLPAX_getBasicScanRate(self.instrHandler, byref(scanrate))
        return '*CFG?', {
            'wavelength [nm]': wavelength.value*1e9,
            'mode': mode.value,
            'scan rate [Hz]': scanrate.value}
    

    def getSingleMeas(self):
        """
        single measurement
        
        return an array of:
        datetime [integer]
        normalized S1, normalized S2, normalized S3 [floats]
        power, degree of polarization, azimuth, ellipticity [floats]
        """
        currID = c_int()
        datetime = c_int()
        s1, s2, s3 = c_double(), c_double(), c_double()
        power, powerPolarized, powerUnpolarized = c_double(), c_double(), c_double()
        dop, dolp, docp = c_double(), c_double(), c_double()
        azimuthal, ellipticity = c_double(), c_double()

        libPAX.TLPAX_getLatestScan(self.instrHandler, byref(currID))
        libPAX.TLPAX_getTimeStamp(self.instrHandler, currID.value, byref(datetime))
        libPAX.TLPAX_getStokesNormalized(self.instrHandler, currID.value, byref(s1), byref(s2), byref(s3))
        libPAX.TLPAX_getPower(self.instrHandler, currID.value, byref(power), byref(powerPolarized), byref(powerUnpolarized))
        libPAX.TLPAX_getDOP(self.instrHandler, currID.value, byref(dop), byref(dolp), byref(docp))
        libPAX.TLPAX_getPolarization(self.instrHandler, currID.value, byref(azimuthal), byref(ellipticity))
        
        libPAX.TLPAX_releaseScan(self.instrHandler, currID)
        self.latestScanID = currID
        return [int(datetime.value), float(s1.value), float(s2.value), float(s3.value), float(power.value), float(dop.value), float(azimuthal.value), float(ellipticity.value)]
    

    def getPower(self):
        """
        single measurement
        
        return power in float
        """
        currID = c_int()
        power, powerPolarized, powerUnpolarized = c_double(), c_double(), c_double()

        libPAX.TLPAX_getLatestScan(self.instrHandler, byref(currID))
        libPAX.TLPAX_getPower(self.instrHandler, currID.value, byref(power), byref(powerPolarized), byref(powerUnpolarized))
        
        libPAX.TLPAX_releaseScan(self.instrHandler, currID)
        self.latestScanID = currID
        return float(power.value)


    def getStokes(self):
        """
        single measurement
        
        return an array of floats:
        normalized S1, normalized S2, normalized S3
        """
        currID = c_int()
        power, powerPolarized, powerUnpolarized = c_double(), c_double(), c_double()
        s1, s2, s3 = c_double(), c_double(), c_double()

        libPAX.TLPAX_getLatestScan(self.instrHandler, byref(currID))
        libPAX.TLPAX_getPower(self.instrHandler, currID.value, byref(power), byref(powerPolarized), byref(powerUnpolarized))
        libPAX.TLPAX_getStokesNormalized(self.instrHandler, currID.value, byref(s1), byref(s2), byref(s3))
        
        libPAX.TLPAX_releaseScan(self.instrHandler, currID)
        self.latestScanID = currID
        return [float(power.value), float(s1.value), float(s2.value), float(s3.value)]


    def closeDevice(self):
        """
        close the device
        """
        return libPAX.TLPAX_close(self.instrHandler)