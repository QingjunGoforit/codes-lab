import os, time
import pyvisa

## libraries

## main
class agilent3320A():
    """
    tested with Agilent 3320A function generator
    refer to the pdf file

    | property type | datatype | read/write | page in handbook | unit | description                                 |
    |---------------|----------|------------|------------------|------|---------------------------------------------|


    example flow
    funcGenAddr = 'GPIB::10'
    funcGen = agilent3320A(funcGenAddr)

    funcGen.signalShape('SQUARE')       ## square waveform
    funcGen.signalFreq(24.5e3)          ## 24.5kHz
    funcGen.signalAmplitudeUnit('VPP')  ## Vpp
    funcGen.signalAmplitude(1)          ## 1 V
    funcGen.signalOffset(0.5)           ## 0.5V

    funcGen.burstState(1)               ## enable burst mode
    funcGen.burstCycle(10)              ## 10 cycles per burst
    funcGen.burstMode('TRIGGERED')      ## apply on trigger
    funcGen.triggerSource('BUS')        ## will be triggered with TRG*

    funcGen.enableOutput(True)          ## enable output
    funcGen.trigger()                   ## trigger the burst
    funcGen.waitForTrigger()            ## wait until the triggering is finished
    funcGen.beep()                      ## beep
    """

    def __init__(self, portAddr):
        """
        constructor
        """
        resMan = pyvisa.ResourceManager()
        self.instHandler = resMan.open_resource(portAddr)

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
        query command
        """
        return self.instHandler.query(command)
    

    def identifyDevice(self):
        """
        IDN command
        """
        return self.query('*IDN?').strip()
    

    def signalShape(self, shape):
        """
        control output waveform shape
        can be set to SIN<USOID>, SQU<ARE>, RAMP, PULS<E>, NOIS<E>, DC, USER
        """
        self.write('FUNC ' + str(shape))
        return 0
    

    def signalFreq(self, frequency):
        """
        control frequency of the output waveform in Hz
        from 1e-6 to 20e6 depending on the waveform shape, limit is hardcoded

        todo: assert whether the given number is under/over the limit
        """
        ## lower & upper limit
        if frequency < 1e-6: frequency = 1e-6
        elif frequency > 20e6: frequency = 20e6

        self.write('FREQ ' + str(frequency))
        return 0


    def signalAmplitudeUnit(self, amplitudeUnit):
        """
        control unit of the amplitude
        can be set to VPP, VRMS, DBM
        """
        self.write('VOLT:UNIT ' + str(amplitudeUnit))
        return 0
    

    def signalAmplitude(self, amplitude):
        """
        control voltage amplitude of the output waveform in V
        from 10e-3 to 10 V, limit is hardcoded

        todo: assert whether the given number is under/over the limit
        """
        ## lower & upper limit
        if amplitude < 10e-3: amplitude = 10e-3
        elif amplitude > 10.0: amplitude = 10.0

        self.write('VOLT ' + str(amplitude))
        return 0
    

    def signalOffset(self, offset):
        """
        control voltage offset of the output waveform in V
        from 0 to 4.995 V, depending on the set voltage amplitude: max offset = (10-voltage)/2
        limit is hardcoded

        todo: assert whether the given number is under/over the limit
        """
        ## lower & upper limit
        if offset < -4.995: offset = -4.995
        elif offset > 4.995: offset = 4.995

        self.write('VOLT:OFFS ' + str(offset))
        return 0
    

    def signalSquareDutyCycle(self, dutyCycle):
        """
        control the duty cycle of the output waveform in percent
        only works if the signal waveform is square
        from 20 to 80%, limit is hardcoded

        todo: 
        assert the signal waveform first
        assert whether the given number is under/over the limit
        """
        ## lower & upper limit
        if dutyCycle < 20: dutyCycle = 20
        elif dutyCycle > 80: dutyCycle = 80

        self.write('FUNC:SQU:DCYC ' + str(dutyCycle))
        return 0


    def burstState(self, burstState):
        """
        turns on/off the burst state
        """
        self.write('BURS:STAT ' + str(burstState))
        return 0
    

    def burstCycle(self, burstCycle):
        """
        control the burst number of cycles
        from 1 to 50000, limit is hardcoded

        todo: assert whether the given number is under/over the limit
        """
        ## lower & upper limit
        if burstCycle < 1: burstCycle = 1
        elif burstCycle > 50001: burstCycle = 50001

        self.write('BURS:NCYC ' + str(burstCycle))
        return 0
    

    def burstMode(self, burstMode):
        """
        control the burst mode
        can be set to TRIG<GERED>, GAT<ED>
        """
        self.write('BURS:MODE ' + burstMode)
        return 0
    

    def triggerSource(self, triggerSource):
        """
        set the source of the trigger
        can be set to IMM<EDIATE>, EXT<ERNAL>, BUS
        """
        self.write('TRIG:SOUR ' + triggerSource)
        return 0
    

    def enableOutput(self, enableOutput):
        """
        turns on/off the output
        """
        self.write('OUTP ' + str(enableOutput))
        return 0
    

    def trigger(self):
        """
        send a trigger signal to the function generator
        """
        self.write('*TRG;*WAI')
        return 0
    

    # def waitForTrigger(self, timeout=3600, shouldStop=lambda: False):
    #     """
    #     wait until triggering has finished or timeout is reached
    #     :param timeout: the maximum time the waiting is allowed to take. if exceeded, TimoutError is raised. if set to zero, no timeout will be used
    #     :param shouldStop: optional function for returning bool, to allow the waiting to be stopped before it ends 
    #     """
    #     self.query('*OPC?')

    #     timeInit = time.time()
    #     while True:
    #         try: ready = bool(self.read())
    #         except pyvisa.errors.VisaIOError: ready = False

    #         if ready:
    #             return

    #         if timeout != 0 and time.time()-timeInit>timeout:
    #             raise TimeoutError('timeout expired while waiting')

    #         if shouldStop():
    #             return


    def beep(self):
        """
        beep
        """
        self.write('SYST:BEEP')
        return 0


    def closeDevice(self):
        """
        close the device
        """
        ## maybe set back to local first
        return self.instHandler.close()