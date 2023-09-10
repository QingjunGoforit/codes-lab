import re, os, time
import pyvisa

## libraries

## main
class ozopticsEPC400():
    """
    tested with EPC400 controller EPC OEM Driver V2.10
    refer to programming manual (digital where?)

    | property type | datatype | read/write | page in handbook | unit | description                                 |
    |---------------|----------|------------|------------------|------|---------------------------------------------|
    """

    def __init__(self, portAddr, timeout=30000):
        """
        constructor
        portAddr: string
        """
        resMan = pyvisa.ResourceManager()
        self.instHandler = resMan.open_resource(portAddr, write_termination='\r\n', read_termination='\r\n', timeout=timeout)


    def write(self, command):
        """
        write command
        """
        self.instHandler.write(command)


    def read(self):
        """
        read command
        """
        return self.instHandler.read()


    def query(self, command):
        """
        query command. basically write then read
        unfortunately need to wait until the device replied with 'Done' to end the poll
        """
        response = ''
        if self.instHandler is not None:
            self.write(command)
            if command == 'RST': [self.read() for _ in range(2)]
            else:
                while True:
                    next_line = self.read()
                    if next_line == 'Done': break
                    else: response += f',{next_line}'
        return command, response[1:]
        
    
    def resetDev(self):
        """
        reset
        """
        return self.query('RST')
    
    
    def getConfig(self):
        """
        query config
        """
        return self.query('CD')

    
    def identifyDevice(self):
        """
        query IDN
        """
        return {'serial': self.getSerialNumber()[1], 'revision': self.getVersion()[1]}
    

    def getSerialNumber(self):
        """
        get the serial number
        """
        _, serialNumber = self.query('SN?')
        if serialNumber != "":
            serialNumber = serialNumber.split(",")[0].split(":")[1][1:]
        return serialNumber


    def getVersion(self):
        """
        get the current version
        """
        _, version = self.query('VER?')
        if version != "":
            version = version.split(":")[1][1:]
        return version


    def getMode(self):
        """
        get the current mode
        """
        _, response = self.query('M?')
        return response[-2:]
    

    def setMode(self, value):
        """
        set the mode to either AC or DC
        with MAC or MDC
        """
        self.query(f'M{str(value)}')[0]
        return 0


    def getVoltages(self):
        """
        get the voltages for all channel
        device format is in mV
        assert if length of the returned array is 4

        return volt in float

        todo: get the number of channel of the device and assert if array length match
        """
        _, response = self.query('V?')
        dataVoltages = re.findall(r"CH\d\s*([\d+-]+)", response)
        if len(dataVoltages) != 4:
            raise RuntimeError('unexpected voltage query response: {}'.format(response))
        return [float(i)/1e3 for i in dataVoltages]


    def setVoltage(self, channel, voltage):
        """
        set the voltage for a single channel
        limit of -5 to 5V is hardcoded

        todo: 
        bypass the MDC command
        assert whether the given number is under/over the limit
        """
        ## lower & upper limit
        if voltage < -5.0: voltage = -5.0
        elif voltage > 5.0: voltage = 5.0
        
        self.query('V{:d},{:04d}'.format(channel+1, int(voltage*1e3)))[0]
        self.query(f'MDC')[0] ## not sure why but we need this one
        return 0
    

    def setVoltages(self, voltages):
        """
        set the voltages for all channel
        basically calling setVoltage for enumerate of the given voltage array

        todo: get the number of channel of the device and assert if array length match
        """
        
        for chan, voltage in enumerate(voltages):
            self.setVoltage(chan, voltage)
        return 0

    
    def closeDevice(self):
        """
        close the device
        """
        return self.instHandler.close()