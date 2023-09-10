/**************************************************************************//**

   \file          TLPAX.h
   \ingroup       TLPAX_x
   \brief         Thorlabs PAX Polarimeter Instrument Driver


   Thorlabs GmbH - PAX - Instrument Driver


   \date          May-11-2015
   \copyright     copyright(c) 2015 Thorlabs GmbH. All Rights Reserved.
   \author        Thomas Schlosser (tschlosser@thorlabs.com)

******************************************************************************/

#ifndef __TLPAX_H__
#define __TLPAX_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include <vpptype.h>

//==============================================================================
// HOWTO for adding a new command to the driver
// 1. Create FP


/*=============================================================================
   Macros
=============================================================================*/
// #define ENABLE_OBSOLETE_FOR_FIRMW_094
/*
When you want to compile for OLD version, overwrite
TLPAX.fp with TLPAX_FWREV_093_and_earlier.fp

When you want to compile for NEW version, overwrite
TLPAX.fp with TLPAX_FWREV_094_and_later.fp

ATTENTION
When you change the FP-File you have to update also one of:
TLPAX_FWREV_093_and_earlier.fp or TLPAX_FWREV_094_and_later.fp (!)
*/

/*-----------------------------------------------------------------------------
   USB stuff
-----------------------------------------------------------------------------*/
#define TLPAX_USB_VID_THORLABS      (0x1313)    ///< Thorlabs
#define TLPAX_USB_PID_PAX100_DFU    (0x8031)    ///< PAX100 with DFU interface enabled
#define TLPAX_USB_PID_PAX100        (0x8039)    ///< PAX100 w/o DFU interface

/*---------------------------------------------------------------------------
 Buffers
---------------------------------------------------------------------------*/
#define TLPAX_BUFFER_SIZE             (1024)      ///< General buffer size
#define TLPAX_ERR_DESCR_BUFFER_SIZE   (1024)      ///< Buffer size for error messages

/*---------------------------------------------------------------------------
 Error/Warning Codes
   Note: The instrument returns errors within the range -512 .. +1023.
   The driver adds the value VI_INSTR_ERROR_OFFSET (0xBFFC0900). So the
   driver returns instrument errors in the range 0xBFFC0700 .. 0xBFFC0CFF.
---------------------------------------------------------------------------*/
// / < todo check value of VI_INSTR_ERROR_OFFSET
// Offsets
#undef VI_INSTR_WARNING_OFFSET
#undef VI_INSTR_ERROR_OFFSET

#define VI_INSTR_WARNING_OFFSET        (0x3FFC0900L)
#define VI_INSTR_ERROR_OFFSET          (_VI_ERROR + 0x3FFC0900L)   //0xBFFC0900

// Driver warnings
#undef VI_INSTR_WARN_OVERFLOW
#undef VI_INSTR_WARN_UNDERRUN
#undef VI_INSTR_WARN_NAN
#undef VI_INSTR_WARN_LEGACY_FW

#define VI_INSTR_WARN_OVERFLOW         (VI_INSTR_WARNING_OFFSET + 0x01L)   //0x3FFC0901
#define VI_INSTR_WARN_UNDERRUN         (VI_INSTR_WARNING_OFFSET + 0x02L)   //0x3FFC0902
#define VI_INSTR_WARN_NAN              (VI_INSTR_WARNING_OFFSET + 0x03L)   //0x3FFC0903
#define VI_INSTR_WARN_LEGACY_FW        (VI_INSTR_WARNING_OFFSET + 0x10L)   //0x3FFC0910

#define VI_INSTR_WARN_FLAG_MOTOR_LOW        (VI_INSTR_WARNING_OFFSET + 0x11L)
#define VI_INSTR_WARN_FLAG_MOTOR_HIGH       (VI_INSTR_WARNING_OFFSET + 0x12L)
#define VI_INSTR_WARN_FLAG_ADC_UNDERRUN     (VI_INSTR_WARNING_OFFSET + 0x13L)
#define VI_INSTR_WARN_FLAG_ADC_OVERFLOW     (VI_INSTR_WARNING_OFFSET + 0x14L)
#define VI_INSTR_WARN_FLAG_TEM_LOW          (VI_INSTR_WARNING_OFFSET + 0x15L)
#define VI_INSTR_WARN_FLAG_TEM_HIGH         (VI_INSTR_WARNING_OFFSET + 0x16L)
#define VI_INSTR_WARN_FLAG_USB_POWER_LOW    (VI_INSTR_WARNING_OFFSET + 0x17L)
#define VI_INSTR_WARN_FLAG_USB_POWER_HIGH   (VI_INSTR_WARNING_OFFSET + 0x18L)
#define VI_INSTR_WARN_FLAG_TIA_SWITCH       (VI_INSTR_WARNING_OFFSET + 0x19L)
#define VI_INSTR_WARN_FLAG_TIA_AUTO         (VI_INSTR_WARNING_OFFSET + 0x20L)
#define VI_INSTR_WARN_FLAG_OUT_OF_SYNC      (VI_INSTR_WARNING_OFFSET + 0x21L)
#define VI_INSTR_WARN_FLAG_EXTERNAL_POWER   (VI_INSTR_WARNING_OFFSET + 0x22L)
#define VI_INSTR_WARN_FLAG_MOTOR_LOCK       (VI_INSTR_WARNING_OFFSET + 0x23L)
#define VI_INSTR_WARN_FLAG_MOTOR_OVER_CON   (VI_INSTR_WARNING_OFFSET + 0x24L)
#define VI_INSTR_WARN_FLAG_MOTOR_OVER_TEM   (VI_INSTR_WARNING_OFFSET + 0x25L)
#define VI_INSTR_WARN_FLAG_MOTOR_HARDWARE   (VI_INSTR_WARNING_OFFSET + 0x26L)
#define VI_INSTR_WARN_FLAG_MOTOR_SPEED      (VI_INSTR_WARNING_OFFSET + 0x27L)
#define VI_INSTR_WARN_FLAG_MOTOR_KT         (VI_INSTR_WARNING_OFFSET + 0x28L)
#define VI_INSTR_WARN_FLAG_MOTOR_OPENLOOP   (VI_INSTR_WARNING_OFFSET + 0x29L)
#define VI_INSTR_WARN_FLAG_MOTOR_CLOSEDLOOP (VI_INSTR_WARNING_OFFSET + 0x30L)


// Driver errors
#define VI_INSTR_ERR_NSUP_VIRTUAL      (VI_INSTR_ERROR_OFFSET + 0x01L)    //0xBFFC0901
#define VI_INSTR_ERR_WRONG_THREAD      (VI_INSTR_ERROR_OFFSET + 0x02L)    //0xBFFC0902
#define VI_INSTR_ERR_INVALID_SNAPSHOT  (VI_INSTR_ERROR_OFFSET + 0x03L)    //0xBFFC0903
#define VI_INSTR_ERR_INV_VAL_CNT       (VI_INSTR_ERROR_OFFSET + 0x04L)    //0xBFFC0904
#define VI_INSTR_ERR_INV_SCAN          (VI_INSTR_ERROR_OFFSET + 0x05L)    //0xBFFC0905
#define VI_INSTR_ERR_INV_RANGE         (VI_INSTR_ERROR_OFFSET + 0x06L)    //0xBFFC0906
#define VI_INSTR_ERR_OBSOLETE          (VI_INSTR_ERROR_OFFSET + 0x07L)    //0xBFFC0907

/*========================================================================*//**
\defgroup   TLPAX_SCAN_EVAL_x Scan Quality Flags
\brief   Scan Quality Flags
\details Detailled information about the devices condition during sampling the
         scan data.
@{
*//*=========================================================================*/
#define TLPAX_SCAN_LOW_SPEED                 (0x00000001)   ///< Set if the PAX motor speed is too low
#define TLPAX_SCAN_HIGH_SPEED                (0x00000002)   ///< Set if the PAX motor speed is too high
#define TLPAX_SCAN_FLAG_LOW                  (0x00000004)   ///< Set if input hardware underrun occured during this scan
#define TLPAX_SCAN_FLAG_HIGH                 (0x00000008)   ///< Set if input hardware overrun occured during this scan
#define TLPAX_SCAN_TEMP_LOW                  (0x00000010)   ///< Set if the PAX temperature is below limit
#define TLPAX_SCAN_TEMP_HIGH                 (0x00000020)   ///< Set if the PAX temperature is above limit
#define TLPAX_SCAN_VUSB_LOW                  (0x00000040)   ///< Set if the USB voltage is too low
#define TLPAX_SCAN_IUSB_HIGH                 (0x00000080)   ///< Set if the current drawn from USB is too high
#define TLPAX_SCAN_GAINSWITCHING             (0x00000100)   ///< Set if there is an gain range switching in progress
#define TLPAX_SCAN_AUTORNG_ENABLED           (0x00000200)   ///< Set if autoranging is enabled
#define TLPAX_SCAN_OTAUTORNG                 (0x00000400)   ///< Set if there is an one time autoranging in progress
#define TLPAX_SCAN_OUTOFSYNC                 (0x00000800)   ///< Set if an out of synchronisation event ocurred in hardware during this scan
#define TLPAX_SCAN_VEXT_PRESENT              (0x00001000)   ///< Set if the external power supply is present
// from the motor status register
#define TLPAX_SCAN_MOTOR_TRIMMED             (0x00010000)   ///< Set if the PAX motor controller indicates 'the device is trimmed'
#define TLPAX_SCAN_MOTOR_LOCK                (0x00100000)   ///< Set if the PAX motor controller indicates a motor lock condition
#define TLPAX_SCAN_MOTOR_OVERCURR            (0x00200000)   ///< Set if the PAX motor controller indicates a motor over current condition
#define TLPAX_SCAN_MOTOR_OTP                 (0x00800000)   ///< Set if the PAX motor controller indicates over temperature
// from the motor fault code register
#define TLPAX_SCAN_MOTOR_HW_LIMIT            (0x01000000)   ///< Set if the PAX motor controller indicates 'hardware current limit'
#define TLPAX_SCAN_MOTOR_SPEED_ABNORM        (0x02000000)   ///< Set if the PAX motor controller indicates that the motor speed is abnormal
#define TLPAX_SCAN_MOTOR_KT_ABNORM           (0x04000000)   ///< Set if the PAX motor controller indicates that the motors Kt is abnormal
#define TLPAX_SCAN_MOTOR_MISSING             (0x08000000)   ///< Set if the PAX motor controller indicates that the motor is missing
#define TLPAX_SCAN_MOTOR_STUCK_OL            (0x10000000)   ///< Set if the PAX motor controller indicates that the motor is stuck in open loop
#define TLPAX_SCAN_MOTOR_STUCK_CL            (0x20000000)   ///< Set if the PAX motor controller indicates that the motor is stuck in closed loop
/**@}*/  // defgroup TLPAX_MEASMODE_x

/*========================================================================*//**
\defgroup   TLPAX_MEASMODE_x Measurement Modes
\brief   Measurement Modes
@{
*//*=========================================================================*/
#define TLPAX_MEASMODE_IDLE         (0)   ///< Idle, no measurements are taken
#define TLPAX_MEASMODE_HALF_512     (1)   ///< 0.5 revolutions for one measurement, 512 points for FFT
#define TLPAX_MEASMODE_HALF_1024    (2)   ///< 0.5 revolutions for one measurement, 1024 points for FFT
#define TLPAX_MEASMODE_HALF_2048    (3)   ///< 0.5 revolutions for one measurement, 2048 points for FFT
#define TLPAX_MEASMODE_FULL_512     (4)   ///< 1 revolution for one measurement, 512 points for FFT
#define TLPAX_MEASMODE_FULL_1024    (5)   ///< 1 revolution for one measurement, 1024 points for FFT
#define TLPAX_MEASMODE_FULL_2048    (6)   ///< 1 revolution for one measurement, 2048 points for FFT
#define TLPAX_MEASMODE_DOUBLE_512   (7)   ///< 2 revolutions for one measurement, 512 points for FFT
#define TLPAX_MEASMODE_DOUBLE_1024  (8)   ///< 2 revolutions for one measurement, 1024 points for FFT
#define TLPAX_MEASMODE_DOUBLE_2048  (9)   ///< 2 revolutions for one measurement, 2048 points for FFT
/**@}*/  // defgroup TLPAX_MEASMODE_x

/*========================================================================*//**
\defgroup   TLPAX_SAVE_PARAMETER_x Save Parameter
\brief   Save Parameter
@{
*//*=========================================================================*/
#define TLPAX_SAVE_MOTREG        (1)      ///< motor controller registers
#define TLPAX_SAVE_PDSENS        (2)      ///< photodiode sensitivity list
#define TLPAX_SAVE_ATTEN         (3)      ///< polarizer attenuation list
#define TLPAX_SAVE_WAVEPL        (4)      ///< waveplate constant
#define TLPAX_SAVE_ALPHAWL       (5)      ///< correction angle aWL list
// obsolete for Firmware Rev. 0.9.4 and later
// #define TLPAX_SAVE_ALPHASPEED    (6)   ///< correction angle aSPEED parameter list
#define TLPAX_SAVE_AZIMUTH       (7)      ///< azimuth correction angle ?
#define TLPAX_SAVE_GAINADJUST    (8)      ///< TIA amplifier gain/offset list
#define TLPAX_SAVE_POWERCOR      (9)      ///< additional power correction factor
#define TLPAX_SAVE_MOTLIMITS     (10)     ///< motor speed limits
#define TLPAX_SAVE_WAVELIMITS    (11)     ///< wavelength limits
#define TLPAX_SAVE_MOTORCHAR     (12)     ///< motor characteristic
#define TLPAX_SAVE_REFINDEX      (13)     ///< refractive index table
#define TLPAX_SAVE_QEIOFFSETS    (14)     ///< quadrature encoder interface offsets
#define TLPAX_SAVE_TIATHRESHOLD  (15)     ///< TIA threshold (for auto gain)
#define TLPAX_SAVE_PAXLIMITS     (16)     ///< PAX limits (current,voltage,temp...)
#define TLPAX_SAVE_GPDATA        (17)     ///< PAX general pupose data (flags)
// obsolete for Firmware Rev. 0.9.4 and later
// #define TLPAX_SAVE_ALPHAGAIN     (18)  ///< correction angle aGAIN parameter list
#define TLPAX_SAVE_OPMODELIMITS  (19)     ///< PAX mode enable/disable and dependent speed limits
// new for PAX Firmware Rev. 0.9.4 and later
#define TLPAX_SAVE_TIAOFFSET     (20)     ///< TIA offset(temp) parameters (I0offset)
#define TLPAX_SAVE_FFTCORR       (21)     ///< FFT correction (speed, gain) parameters (I0,I2,I4,Phi2,Phi4)

#define TLPAX_SAVE_ADJUSTMENT    (1017)   ///< all of the above
#define TLPAX_SAVE_DEVICENAME    (1018)   ///< device name
#define TLPAX_SAVE_SERIALNUMBER  (1019)   ///< device serial number
#define TLPAX_SAVE_CALSTRING     (1020)   ///< calibration string
#define TLPAX_SAVE_MANUFACTURER  (1021)   ///< manufacturer name

/**@}*/  // defgroup TLPAX_SAVE_PARAMETER_x

/*========================================================================*//**
\defgroup   TLPAX_MOTOR_REGISTER_x Low Level Motor Register
\brief   Motor REgister
@{
*//*=========================================================================*/
#define TLPAX_MOTOR_REGISTER_SPEED_CONTROL            (0)
#define TLPAX_MOTOR_REGISTER_DEVICE_CONTROL           (2)
#define TLPAX_MOTOR_REGISTER_EEPROM_CONTROL           (3)
#define TLPAX_MOTOR_REGISTER_STATUS                   (16)
#define TLPAX_MOTOR_REGISTER_MOTOR_SPEED              (17)
#define TLPAX_MOTOR_REGISTER_MOTOR_PERIOD             (19)
#define TLPAX_MOTOR_REGISTER_MOTOR_KT                 (21)
#define TLPAX_MOTOR_REGISTER_IPD_POSITION             (25)
#define TLPAX_MOTOR_REGISTER_SUPPLY_VOLTAGE           (26)
#define TLPAX_MOTOR_REGISTER_SPEED_COMMAND            (27)
#define TLPAX_MOTOR_REGISTER_SPEED_COMMAND_BUFFERED   (28)
#define TLPAX_MOTOR_REGISTER_FAULT_CODE               (30)
#define TLPAX_MOTOR_REGISTER_MOTOR_PARAMETER_1        (32)
#define TLPAX_MOTOR_REGISTER_MOTOR_PARAMETER_2        (33)
#define TLPAX_MOTOR_REGISTER_MOTOR_PARAMETER_3        (34)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_1          (35)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_2          (36)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_3          (37)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_4          (38)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_5          (39)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_6          (40)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_7          (41)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_8          (42)
#define TLPAX_MOTOR_REGISTER_SYSTEM_OPTION_9          (43)
/**@}*/  // defgroup TLPAX_MOTOR_REGISTER_x

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================

/*========================================================================*//**
\defgroup   FPCLASS_ROOT_x Thorlabs PAX VISA Instrument Driver Functions Tree
\brief   Thorlabs PAX VISA Instrument Driver
@{
*//*=========================================================================*/
ViStatus _VI_FUNC TLPAX_init (ViRsrc resourceName, ViBoolean IDQuery,
                              ViBoolean resetDevice, ViPSession vi);


/*========================================================================*//**
\defgroup   FPCLASS_ACTION_x Action/Status Functions
\brief   Action/Status Functions
\details This class allows the user to execute actions on the instrument and to
         determine the current state of the instrument.
@{
*//*=========================================================================*/
ViStatus _VI_FUNC TLPAX_getLatestScan (ViSession vi, ViPUInt32 scanId);
ViStatus _VI_FUNC TLPAX_releaseScan (ViSession vi, ViUInt32 scanID);

ViStatus _VI_FUNC TLPAX_getPolarization   (ViSession vi, ViUInt32 scanID, ViPReal64 azimuthRad, ViPReal64 ellipticityRad);
ViStatus _VI_FUNC TLPAX_getStokesNormalized (ViSession vi, ViUInt32 scanID, ViPReal64 s1, ViPReal64 s2, ViPReal64 s3);
ViStatus _VI_FUNC TLPAX_getStokes         (ViSession vi, ViUInt32 scanID, ViPReal64 S0, ViPReal64 S1, ViPReal64 S2, ViPReal64 S3);
ViStatus _VI_FUNC TLPAX_getDOP            (ViSession vi, ViUInt32 scanID, ViPReal64 DOP, ViPReal64 DOLP, ViPReal64 DOCP);
ViStatus _VI_FUNC TLPAX_getPower          (ViSession vi, ViUInt32 scanID, ViPReal64 totalPower, ViPReal64 polarizedPower, ViPReal64 unpolarizedPower);
ViStatus _VI_FUNC TLPAX_getSaturation     (ViSession vi, ViUInt32 scanID, ViPReal64 minimumLevel, ViPReal64 maximumLevel, ViPUInt32 saturationFlags);
ViStatus _VI_FUNC TLPAX_getTimeStamp      (ViSession vi, ViUInt32 scanID, ViPUInt32 timeStamp);
ViStatus _VI_FUNC TLPAX_getRevTime        (ViSession vi, ViUInt32 scanID, ViPReal64 revolutionTime);
ViStatus _VI_FUNC TLPAX_getWaveplateCount (ViSession vi, ViUInt32 scanID, ViPUInt32 waveplateCount);
ViStatus _VI_FUNC TLPAX_getMisalignment   (ViSession vi, ViUInt32 scanID, ViPReal64 misalignment);
ViStatus _VI_FUNC TLPAX_getUsedTIA        (ViSession vi, ViUInt32 scanID, ViPInt32 usedTIA);
ViStatus _VI_FUNC TLPAX_getFlags            (ViSession vi, ViUInt32 scanID, ViPUInt32 flags);
ViStatus _VI_FUNC TLPAX_getIsTemperatureTooHigh(ViSession instr, ViUInt32 scanID, ViPBoolean isTemperatureTooHigh);
ViStatus _VI_FUNC TLPAX_getExtPowerSupply(ViSession instr, ViUInt32 scanID, ViPBoolean Connected);
/**@}*/  // defgroup FPCLASS_ACTION_x


/*========================================================================*//**
\defgroup   FPCLASS_RESOURCE_x Resource Functions
\brief   Resource Functions
\details This class provides functions to find and identify available devices
         in your system.
@{
*//*=========================================================================*/

ViStatus _VI_FUNC TLPAX_findRsrc (ViSession vi, ViPUInt32 deviceCount);

ViStatus _VI_FUNC TLPAX_getRsrcName (ViSession vi, ViUInt32 index,
                                      ViChar resourceName[]);

ViStatus _VI_FUNC TLPAX_getRsrcInfo (ViSession vi, ViUInt32 index,
                                     ViChar modelName[], ViChar serialNumber[],
                                     ViChar manufacturer[], ViPBoolean deviceAvailable);

/**@}*/  // defgroup FPCLASS_RESOURCE_x


/*========================================================================*//**
\defgroup   FPCLASS_UTILITY_x Utility Functions
\brief   Utility Functions
\details This class of functions provides utility and lower level functions to
         communicate with the instrument.
@{
*//*=========================================================================*/
ViStatus _VI_FUNC TLPAX_identificationQuery (ViSession vi,
                                             ViChar manufacturerName[],
                                             ViChar deviceName[],
                                             ViChar serialNumber[],
                                             ViChar firmwareRevision[]);

ViStatus _VI_FUNC TLPAX_calibrationMessage (ViSession vi,
                                            ViChar message[]);

ViStatus _VI_FUNC TLPAX_reset (ViSession vi);

ViStatus _VI_FUNC TLPAX_selfTest (ViSession vi,
                                  ViPInt16 selfTestResult,
                                  ViChar selfTestMessage[]);

ViStatus _VI_FUNC TLPAX_errorQuery (ViSession vi,
                                    ViPInt32 errorNumber, ViChar errorMessage[]);

ViStatus _VI_FUNC TLPAX_errorMessage (ViSession vi,
                                      ViStatus statusCode, ViChar description[]);

ViStatus _VI_FUNC TLPAX_revisionQuery (ViSession vi,
                                       ViChar instrumentDriverRevision[],
                                       ViChar firmwareRevision[]);

/*========================================================================*//**
\defgroup   TLPAX_STATBIT_OPER_x Operation Status Register Bits
\brief   Operation Status Register Bits
@{
*//*=========================================================================*/
#define TLPAX_STATBIT_OPER_VEXT_PRESENT   (0x00000100)   ///< The external power supply is present
#define TLPAX_STATBIT_OPER_IUSB_HIGH      (0x00000200)   ///< The current drawn from USB is too high
#define TLPAX_STATBIT_OPER_VUSB_LOW       (0x00000400)   ///< The USB supply voltage is too low
#define TLPAX_STATBIT_OPER_TEMP_LOW       (0x00000800)   ///< The PAX sensor temperature is above limit
#define TLPAX_STATBIT_OPER_TEMP_HIGH      (0x00001000)   ///< The PAX sensor temperature is below limit
/**@}*/  // defgroup TLPAX_STATBIT_OPER_x

ViStatus _VI_FUNC TLPAX_getControlOperationEventRegister (ViSession vi,ViPUInt32 eventRegister);
ViStatus _VI_FUNC TLPAX_getControlOperationConditionRegister (ViSession vi, ViPUInt32 conditionRegister);
ViStatus _VI_FUNC TLPAX_setControlOperationEnableRegister (ViSession vi, ViUInt32 enableRegister);
ViStatus _VI_FUNC TLPAX_getControlOperationEnableRegister (ViSession vi, ViPUInt32 enableRegister);

/*========================================================================*//**
\defgroup   TLPAX_STATBIT_QUES_x Questionable Status Register Bits
\brief   Questionable Status Register Bits
@{
*//*=========================================================================*/
#define TLPAX_STATBIT_QUES_LOW_SPEED                  (0x00000001)   ///< The PAX motor speed is too low
#define TLPAX_STATBIT_QUES_HIGH_SPEED                 (0x00000002)   ///< The PAX motor speed is too high
#define TLPAX_STATBIT_QUES_IPD_LOW                    (0x00000004)   ///< The ADC had an underrun during last measurement
#define TLPAX_STATBIT_QUES_IPD_HIGH                   (0x00000008)   ///< The ADC had an overflow during last measurement
#define TLPAX_STATBIT_QUES_GAINSWITCHING_IN_PROGRES   (0x00000010)   ///< There is an gain range switching in progres
#define TLPAX_STATBIT_QUES_AUTORNG_ENABLED            (0x00000020)   ///< Autoranging is enabled
#define TLPAX_STATBIT_QUES_OTAUTORNG_IN_PROGRES       (0x00000040)   ///< There is an one time autoranging in progres
#define TLPAX_STATBIT_QUES_OUT_OF_SYNC                (0x00000080)   ///< An out of sync condition during one of the last measurements was detected
/**@}*/  // defgroup TLPAX_STATBIT_QUES_x

ViStatus _VI_FUNC TLPAX_getControlQuestionableEventRegister (ViSession vi,ViPUInt32 eventRegister);
ViStatus _VI_FUNC TLPAX_getControlQuestionableConditionRegister (ViSession vi, ViPUInt32 conditionRegister);
ViStatus _VI_FUNC TLPAX_setControlQuestionableEnableRegister (ViSession vi,ViUInt32 enableRegister);
ViStatus _VI_FUNC TLPAX_getControlQuestionableEnableRegister (ViSession vi,ViPUInt32 enableRegister);

ViStatus _VI_FUNC TLPAX_configureStatusStructures (ViSession vi);

/*========================================================================*//**
\defgroup   TLPAX_STATBIT_AUX_x Auxiliary Status Register Bits
\brief   Auxiliary Status Register Bits
@{
*//*=========================================================================*/
#define TLPAX_STATBIT_AUX_MOTOR_TRIMMED      (0x00000001)   ///< The PAX motor controller indicates 'the device is trimmed'
#define TLPAX_STATBIT_AUX_MOTOR_LOCK         (0x00000010)   ///< The PAX motor controller indicates a motor lock condition
#define TLPAX_STATBIT_AUX_MOTOR_OVERCURR     (0x00000020)   ///< The PAX motor controller indicates a motor over current condition
#define TLPAX_STATBIT_AUX_MOTOR_OTP          (0x00000080)   ///< The PAX motor controller indicates over temperature
#define TLPAX_STATBIT_AUX_MOTOR_HW_LIMIT     (0x00000100)   ///< The PAX motor controller indicates 'hardware current limit'
#define TLPAX_STATBIT_AUX_MOTOR_SPEED_ABNORM (0x00000200)   ///< The PAX motor controller indicates that the motor speed is abnormal
#define TLPAX_STATBIT_AUX_MOTOR_KT_ABNORM    (0x00000400)   ///< The PAX motor controller indicates that the motors Kt is abnormal
#define TLPAX_STATBIT_AUX_MOTOR_MISSING      (0x00000800)   ///< The PAX motor controller indicates that the motor is missing
#define TLPAX_STATBIT_AUX_MOTOR_STUCK_OL     (0x00001000)   ///< The PAX motor controller indicates that the motor is stuck in open loop
#define TLPAX_STATBIT_AUX_MOTOR_STUCK_CL     (0x00002000)   ///< The PAX motor controller indicates that the motor is stuck in closed loop
/**@}*/  // defgroup TLPAX_STATBIT_AUX_x

ViStatus _VI_FUNC TLPAX_getControlAuxiliaryEventRegister (ViSession vi,ViPUInt32 eventRegister);
ViStatus _VI_FUNC TLPAX_getControlAuxiliaryConditionRegister (ViSession vi, ViPUInt32 conditionRegister);
ViStatus _VI_FUNC TLPAX_setControlAuxiliaryEnableRegister (ViSession vi,ViUInt32 enableRegister);
ViStatus _VI_FUNC TLPAX_getControlAuxiliaryEnableRegister (ViSession vi, ViPUInt32 enableRegister);


/**@}*/  // defgroup FPCLASS_UTILITY_x
/*========================================================================*//**
\defgroup   FPCLASS_CONFIGURATION_x Configuration Functions
\brief   Configuration Functions
\details This class of functions provides setup/configuration functions to
         communicate with the instrument.
@{
*//*=========================================================================*/
/*------------------------------------------------------------------------*//**
\brief   Set the measurement mode for the polarisation analysis.
\note    The measurement mode might limit the maximum rotation speed. In this
         case the basic measurement speed is coerced to the maximum value
         allowed in the new measurement mode.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[in] mode               The new measurement mode. See \ref TLPAX_MEASMODE_x
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setMeasurementMode (ViSession vi, ViInt32 mode);

/*------------------------------------------------------------------------*//**
\brief   Get the measurement mode for the polarisation analysis.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] mode              The measurement mode. See \ref TLPAX_MEASMODE_x
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getMeasurementMode (ViSession vi, ViPInt32 mode);

/*------------------------------------------------------------------------*//**
\brief   Set the optical power range.
\details Configures the sensor input circuit for the expected maximum input
         power at any possible polarization.
\note    Setting the power range disables auto power ranging.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[in] power              Specifies the highest optical power level
                              expected for the sensor input in Watt.
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setPowerRange (ViSession vi, ViReal64 power);

/*------------------------------------------------------------------------*//**
\brief   Get the optical power range.
\details Get the maximum input power the sensor input circuit is currently
         configured for to handle in any possible polarization.
\note    The returned value depends on the set wavelength.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] power             The maximum power input level in [W] the input
                              circuit's currently active configuration can
                              handle with any possible polarization.
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getPowerRange (ViSession vi, ViReal64 *power);

/*------------------------------------------------------------------------*//**
\brief   Get the optical power range limits.
\details Get the input power limits the sensor input circuit can handle with
         any possible polarization.
\note    The returned values depend on the set wavelength.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] minimum           The minimum power input level in [W].
\param[out] maximum           The maximum power input level in [W].
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getPowerRangeLimits (ViSession vi, ViReal64 *minimum, ViReal64 *maximum);

/*------------------------------------------------------------------------*//**
\brief   Configures the sensor input circuit with the specified input amplifier
         circuit.
\note    Setting the power range index disables auto power ranging.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[in] TIAIndex           Specifies the transimpedance amplifiere
                              configuration index. Higher index numbers result
                              in higher input gain.
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setPowerRangeIndex (ViSession vi, ViInt32 TIAIndex);

/*------------------------------------------------------------------------*//**
\brief   Get the currently active sensor input transinpedance amplifier
         configuration.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] TIAIndex          The transimpedance amplifiere configuration
                              index. Higher index numbers result in higher
                              input gain.
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getPowerRangeIndex (ViSession vi, ViInt32 *TIAIndex);

/*------------------------------------------------------------------------*//**
\brief   Get the availabel transimpedance amplifier configurations.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] minimum           The minimum transimpedance amplifier
                              configuration index.
\param[out] maximum           The maximum transimpedance amplifier
                              configuration index.
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getPowerRangeIndexLimits (ViSession vi, ViInt32 *minimum, ViInt32 *maximum);

/*------------------------------------------------------------------------*//**
\brief   Get the nominal maximum input power for the specified transimpedance
         amplifier configuration.
\note    The returned value depends on the set wavelength.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[in] TIAIndex           Specifies the transimpedance amplifiere
                              configuration index. Higher index numbers result
                              in higher input gain.
\param[out] power             The maximum power input level in [W] the input
                              circuit's specified configuration can handle with
                              any possible polarization.
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getPowerRangeIndexNominal (ViSession vi, ViInt32 TIAIndex, ViReal64 *power);

/*------------------------------------------------------------------------*//**
\brief   Set the power auto range mode.
\note    Enabling power auto ranging modifies the power range set value.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[in] autorange          Specifies the power auto range mode.
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setPowerAutoRange (ViSession vi, ViBoolean autorange);

/*------------------------------------------------------------------------*//**
\brief   Get the power auto range mode.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[in] autorange          Specifies the power auto range mode.
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getPowerAutoRange (ViSession vi, ViBoolean *autorange);

ViStatus _VI_FUNC TLPAX_setPowerOneTimeAutoRange (ViSession vi);

/*------------------------------------------------------------------------*//**
\brief   Set the basic scan rate.
\details Configures the basic scan rate. According to the measurement mode each
         half rotation each full rotation or two full rotations of the
         waveplate produce one measurement data set (scan). The basic scan rate
         describes how many half rotation scans are possible per second.\n\n

         The actual scan rate calculates depending on the selected measurement
         mode:
         - half rotation measurement modes: sample rate = basic sample rate
         - full rotation measurement modes: sample rate = basic sample rate / 2
         - double rotation measurement modes: sample rate = basic sample rate / 4

         The basic scan rate is directly connected to the waveplate rotaion
         speed. The set value limits depend on the currently selected
         measurement mode and the availability of the external power supply.

\note    When changing measurement mode or external power supply the basic scan
         rate set value will automatically be coerced to the new limits.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[in] basicScanRate      Specifies the basic scan rate in [1/s].
\return  Error code
\sa      TLPAX_getBasicScanRateLimits()
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setBasicScanRate (ViSession vi, ViReal64 basicScanRate);

/*------------------------------------------------------------------------*//**
\brief   Get the basic scan rate.
\details The actual scan rate calculates depending on the selected measurement
         mode:
         - half rotation measurement modes: sample rate = basic sample rate
         - full rotation measurement modes: sample rate = basic sample rate / 2
         - double rotation measurement modes: sample rate = basic sample rate / 4

         The basic scan rate is directly connected to the waveplate rotaion
         speed. The set value limits depend on the currently selected
         measurement mode and the availability of the external power supply.

\note    When changing measurement mode or external power supply the basic scan
         rate set value will automatically be coerced to the new limits.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] basicScanRate     Returns the basic scan rate in [1/s].
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getBasicScanRate (ViSession vi, ViReal64 *basicScanRate);

/*------------------------------------------------------------------------*//**
\brief   Get the basic scan rate limits.
\details The set value limits depend on the currently selected measurement mode
         and the availability of the external power supply.

\note    When changing measurement mode or external power supply the basic scan
         rate set value will automatically be coerced to the new limits.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] minimum           The minimum basic scan rate in [1/s].
\param[out] maximum           The maximum basic scan rate in [1/s].
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getBasicScanRateLimits (ViSession vi, ViReal64 *minimum, ViReal64 *maximum);

ViStatus _VI_FUNC TLPAX_getMotorSpeedLimits (ViSession vi, ViPReal64 withExternalPower, ViPReal64 withoutExternalPower);

/*------------------------------------------------------------------------*//**
\brief   Set the wavelength.
\details This value is used for calculating the measurement data.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[in] wavelength         Specifies the wavelength in [nm].
\return  Error code
\sa      TLPAX_getWavelengthLimits()
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setWavelength (ViSession vi, ViReal64 wavelength);

/*------------------------------------------------------------------------*//**
\brief   Get the wavelength.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] wavelength        Returns the wavelength in [nm].
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getWavelength (ViSession vi, ViReal64 *wavelength);

/*------------------------------------------------------------------------*//**
\brief   Get the wavelength limits.
\param[in] instrumentHandle   the handle obtained by TLPAX_init()
\param[out] minimum           The minimum wavelength in [nm].
\param[out] maximum           The maximum wavelength in [nm].
\return  Error code
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getWavelengthLimits (ViSession vi, ViReal64 *minimum, ViReal64 *maximum);

/**@}*/  // defgroup FPCLASS_CONFIGURATION_x

ViStatus _VI_FUNC TLPAX_set_service_mode (ViSession vi, ViBoolean mode);

ViStatus _VI_FUNC TLPAX_get_service_mode (ViSession vi, ViPBoolean mode);

ViStatus _VI_FUNC TLPAX_setAccessLevel (ViSession vi, ViInt16 level, ViUInt32 code);

ViStatus _VI_FUNC TLPAX_getSystemLogEntry (ViSession vi, ViChar systemLog[]);

/*========================================================================*//**
\defgroup   FPCLASS_SERVICE_x Service Functions
\brief   Service Functions
\details This class of functions provides service functions to
         communicate with the instrument.
@{

*//*=========================================================================*/

ViStatus _VI_FUNC TLPAX_getUniqueID (ViSession vi, ViChar uniqueID[]);

ViStatus _VI_FUNC TLPAX_setMotorData (ViSession vi, ViInt32 index, ViReal64 motorSpeed,  ViUInt32 registerValue);
ViStatus _VI_FUNC TLPAX_getMotorData (ViSession vi, ViInt32 index, ViPReal64 motorSpeed, ViPUInt32 registerValue);

ViStatus _VI_FUNC TLPAX_setGeneralPurposeFlag (ViSession vi, ViUInt32 GPFlag);
ViStatus _VI_FUNC TLPAX_getGeneralPurposeFlag (ViSession vi, ViPUInt32 GPFlag);

ViStatus _VI_FUNC TLPAX_setMotorDataCount (ViSession vi,  ViInt32 count);
ViStatus _VI_FUNC TLPAX_getMotorDataCount (ViSession vi, ViPInt32 count);
ViStatus _VI_FUNC TLPAX_setMotorStrategy (ViSession vi, ViInt32 strategy);
ViStatus _VI_FUNC TLPAX_getMotorStrategy (ViSession vi, ViPInt32 strategy);

ViStatus _VI_FUNC TLPAX_setMotorRegister (ViSession vi, ViUInt32 registerIndex, ViUInt32 motorRegister);
ViStatus _VI_FUNC TLPAX_getMotorRegister (ViSession vi, ViUInt32 registerIndex, ViPUInt32 motorRegister);
ViStatus _VI_FUNC TLPAX_getMotorRotationSpeed (ViSession vi, ViPReal64 rotationSpeed);
ViStatus _VI_FUNC TLPAX_setMotorSpeedRange (ViSession vi, ViReal64 minimum, ViReal64 maximum, ViReal64 def, ViReal64 deviation, ViReal64 maximumWithoutExtPower);
ViStatus _VI_FUNC TLPAX_getMotorSpeedRange (ViSession vi, ViPReal64 minimum, ViPReal64 maximum, ViPReal64 def, ViPReal64 deviation, ViPReal64 maximumWithoutExtPower);

ViStatus _VI_FUNC TLPAX_setOperatingLimits (ViSession vi,
                                            ViReal64 maximumMotorSpeed,
                                            ViReal64 maximumUSBCurrent,
                                            ViReal64 minimumUSBVoltage,
                                            ViReal64 mininumOperatingTemperature,
                                            ViReal64 maximumOperatingTemperature,
                                            ViReal64 TIAGainThreshold,
                                            ViUInt32 adcLowThreshold,
                                            ViUInt32 adcHighThreshold);

ViStatus _VI_FUNC TLPAX_getOperatingLimits (ViSession vi,
                                            ViPReal64 maximumMotorSpeed,
                                            ViPReal64 maximumUSBCurrent,
                                            ViPReal64 minimumUSBVoltage,
                                            ViPReal64 mininumOperatingTemperature,
                                            ViPReal64 maximumOperatingTemperature,
                                            ViPReal64 TIAGainThreshold,
                                            ViPUInt32 adcLowThreshold,
                                            ViPUInt32 adcHighThreshold);

ViStatus _VI_FUNC TLPAX_setOpModeConfiguration (ViSession instrumentHandle, ViInt32 mode, ViBoolean  enable, ViReal64  maximumMotorSpeed);
ViStatus _VI_FUNC TLPAX_getOpModeConfiguration (ViSession instrumentHandle, ViInt32 mode, ViBoolean *enable, ViReal64 *maximumMotorSpeed);

ViStatus _VI_FUNC TLPAX_getPowerSupply (ViSession vi, ViPReal64 current, ViPReal64 voltage);
ViStatus _VI_FUNC TLPAX_getDeviceTemperature (ViSession vi, ViPReal64 temperature);

ViStatus _VI_FUNC TLPAX_getPDSensitivity (ViSession vi, ViPReal64 sensitivity);
ViStatus _VI_FUNC TLPAX_setPDSensitivityData (ViSession vi, ViInt32 index,ViReal64 sensitivity,  ViReal64 wavelength);
ViStatus _VI_FUNC TLPAX_getPDSensitivityData (ViSession vi, ViInt32 index,ViPReal64 sensitivity, ViPReal64 wavelength);
ViStatus _VI_FUNC TLPAX_setPDSensitivityCount (ViSession vi,ViInt32 count);
ViStatus _VI_FUNC TLPAX_getPDSensitivityCount (ViSession vi,ViPInt32 count);

ViStatus _VI_FUNC TLPAX_getPolarizerAttenuation (ViSession vi, ViPReal64 attenuation);
ViStatus _VI_FUNC TLPAX_setPolarizerAttenuationData (ViSession vi, ViInt32 index, ViReal64 attenuation, ViReal64 wavelength);
ViStatus _VI_FUNC TLPAX_getPolarizerAttenuationData (ViSession vi, ViInt32 index, ViPReal64 attenuation, ViPReal64 wavelength);
ViStatus _VI_FUNC TLPAX_setPolarizerAttenuationCount (ViSession vi,ViInt32 count);
ViStatus _VI_FUNC TLPAX_getPolarizerAttenuationCount (ViSession vi, ViPInt32 count);

#ifdef ENABLE_OBSOLETE_FOR_FIRMW_094
/*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setWaveplateConstantX (ViSession vi,ViReal64 constantX);
/*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getWaveplateConstantX (ViSession vi, ViPReal64 constantX);
#endif

#define TLPAX_MAX_WPX_SUPPORT_POINTS         (5)
ViStatus _VI_FUNC TLPAX_setWPXSupportPointsSize (ViSession instr, ViInt32 size);
ViStatus _VI_FUNC TLPAX_setWPXSupportPointValue (ViSession instr, ViInt32 index, ViReal64 constantX);
ViStatus _VI_FUNC TLPAX_setWPXSupportPointTemperature (ViSession instr, ViInt32 index, ViReal64 temperature);
ViStatus _VI_FUNC TLPAX_getWPXSupportPointsValues (ViSession instr, ViPInt32 size, ViReal64 constantsX[]);
ViStatus _VI_FUNC TLPAX_getWPXSupportPointsTemperatures (ViSession instr, ViPInt32 size, ViReal64 temperatures[]);
ViStatus _VI_FUNC TLPAX_getWPXSupportPointValueOfTemperature(ViSession instr, ViReal64 temperature, ViPReal64 constantX);

#define TLPAX_MAX_I0_OFFSET_SUPPORT_POINTS   (5)
ViStatus _VI_FUNC TLPAX_setI0OffSupportPointsSize (ViSession instr, ViInt32 size);
ViStatus _VI_FUNC TLPAX_setI0OffSupportPointTemperature (ViSession instr, ViInt32 index, ViReal64 temperature);
ViStatus _VI_FUNC TLPAX_setI0OffSupportPointValue (ViSession instr, ViInt32 gainIndex, ViInt32 temperatureIndex, ViReal64 offset);
ViStatus _VI_FUNC TLPAX_getI0OffSupportPointsTemperatures (ViSession instr, ViPInt32 size, ViReal64 temperatures[]);
ViStatus _VI_FUNC TLPAX_getI0OffSupportPointValue (ViSession instr, ViInt32 gainIndex, ViInt32 temperatureIndex, ViPReal64 offset);
ViStatus _VI_FUNC TLPAX_getI0OffSupportPointValueOfTemperature (ViSession instr, ViInt32 gainIndex, ViReal64 temperature, ViPReal64 offset);

#define TLPAX_MAX_FFTCORR_SUPPORT_POINTS     (5)
ViStatus _VI_FUNC TLPAX_setI024Phi24SupportPointsSize (ViSession instr, ViInt32 size);
ViStatus _VI_FUNC TLPAX_setI024Phi24SupportPointSpeed(ViSession instr, ViInt32 index, ViReal64 speed);
ViStatus _VI_FUNC TLPAX_setI024Phi24SupportPointValues(ViSession instr, ViInt32 gainIndex, ViInt32 speedIndex, ViReal64 i0, ViReal64 i2, ViReal64 i4, ViReal64 phi2, ViReal64 phi4);
ViStatus _VI_FUNC TLPAX_getI024Phi24SupportPointsSpeeds(ViSession instr, ViPInt32 size, ViReal64 speeds[]);
ViStatus _VI_FUNC TLPAX_getI024Phi24SupportPointValues(ViSession instr, ViInt32 gainIndex, ViInt32 speedIndex, ViPReal64 i0, ViPReal64 i2, ViPReal64 i4, ViPReal64 phi2, ViPReal64 phi4);
ViStatus _VI_FUNC TLPAX_getI024Phi24SupportPointValuesOfSpeed(ViSession instr, ViInt32 gainIndex, ViReal64 speed, ViPReal64 i0, ViPReal64 i2, ViPReal64 i4, ViPReal64 phi2, ViPReal64 phi4);

ViStatus _VI_FUNC TLPAX_setQEIPhase (ViSession vi, ViInt32 mode_2048, ViInt32 mode_4096);
ViStatus _VI_FUNC TLPAX_getQEIPhase (ViSession vi, ViPInt32 mode_2048, ViPInt32 mode_4096);

ViStatus _VI_FUNC TLPAX_hasExternalPowerSupply(ViSession instr, ViPBoolean Connected);


ViStatus _VI_FUNC TLPAX_getCorrectionAngle (ViSession vi, ViPReal64 angle);
ViStatus _VI_FUNC TLPAX_setCorrectionAngleData (ViSession vi, ViInt32 index, ViReal64 angle, ViReal64 wavelength);
ViStatus _VI_FUNC TLPAX_getCorrectionAngleData (ViSession vi, ViInt32 index, ViPReal64 angle, ViPReal64 wavelength);
ViStatus _VI_FUNC TLPAX_setCorrectionAngleCount (ViSession vi, ViInt32 count);
ViStatus _VI_FUNC TLPAX_getCorrectionAngleCount (ViSession vi,ViPInt32 count);
ViStatus _VI_FUNC TLPAX_setCorrectionAngleMotorSpeed (ViSession vi, ViReal64 speed);
ViStatus _VI_FUNC TLPAX_getCorrectionAngleMotorSpeed (ViSession vi, ViPReal64 speed);

#ifdef ENABLE_OBSOLETE_FOR_FIRMW_094

/*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setCorrectionAngleParameter (ViSession vi, ViReal64 offset, ViReal64 slope);
/*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getCorrectionAngleParameter (ViSession vi, ViPReal64 offset, ViPReal64 slope);
/*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getCorrectionAngleAlphaGain (ViSession vi, ViPReal64 angle);
/*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setCorrectionAngleDataAlphaGain (ViSession vi, ViInt32 index, ViReal64 angle);
/*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getCorrectionAngleDataAlphaGain (ViSession vi, ViInt32 index, ViPReal64 angle);
  /*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_setCorrectionAngleMotorSpeedAlphaGain (ViSession vi, ViReal64 speed);
/*------------------------------------------------------------------------*//**
\brief   Obsolete for firmware version 0.9.4
*//*-------------------------------------------------------------------------*/
ViStatus _VI_FUNC TLPAX_getCorrectionAngleMotorSpeedAlphaGain (ViSession vi, ViPReal64 speed);

#endif

ViStatus _VI_FUNC TLPAX_setAzimuthCorrectionAngle (ViSession vi,ViReal64 azimuthAngle);
ViStatus _VI_FUNC TLPAX_getAzimuthCorrectionAngle (ViSession vi,ViPReal64 azimuthAngle);

#ifdef ENABLE_OBSOLETE_FOR_FIRMW_094
ViStatus _VI_FUNC TLPAX_setGainAdjustment (ViSession vi,ViInt32 index, ViReal64 gain, ViReal64 offset);
ViStatus _VI_FUNC TLPAX_getGainAdjustment (ViSession vi,ViInt32 index, ViPReal64 gain,ViPReal64 offset);
#else    // ENABLE_OBSOLETE_FOR_FIRMW_094
ViStatus _VI_FUNC TLPAX_setGainAdjustment (ViSession vi,ViInt32 index, ViReal64 gain);
ViStatus _VI_FUNC TLPAX_getGainAdjustment (ViSession vi,ViInt32 index, ViPReal64 gain);
#endif   // ENABLE_OBSOLETE_FOR_FIRMW_094

ViStatus _VI_FUNC TLPAX_setTiaThreshold (ViSession instr,ViInt32 index, ViUInt32 threshold);
ViStatus _VI_FUNC TLPAX_getTiaThreshold (ViSession instr, ViInt32 index, ViPUInt32 threshold);

ViStatus _VI_FUNC TLPAX_setPowerCorrection (ViSession vi,ViReal64 correctionFactor);
ViStatus _VI_FUNC TLPAX_getPowerCorrection (ViSession vi,ViPReal64 correctionFactor);

ViStatus _VI_FUNC TLPAX_setWavelengthRange (ViSession vi,ViReal64 minimum, ViReal64 maximum,  ViReal64 def);
ViStatus _VI_FUNC TLPAX_getWavelengthRange (ViSession vi,ViPReal64 minimum, ViPReal64 maximum, ViPReal64 def);

ViStatus _VI_FUNC TLPAX_getRefractiveIndex (ViSession vi, ViPReal64 deltaN, ViPReal64 wavelength, ViPReal64 delta);
ViStatus _VI_FUNC TLPAX_setRefractiveIndexData (ViSession vi,ViInt32 index, ViReal64 no, ViReal64 ne, ViReal64 wavelength);
ViStatus _VI_FUNC TLPAX_getRefractiveIndexData (ViSession vi,ViInt32 index,ViPReal64 no, ViPReal64 ne, ViPReal64 wavelength);
ViStatus _VI_FUNC TLPAX_setRefractiveIndexCount (ViSession vi, ViInt32 count);
ViStatus _VI_FUNC TLPAX_getRefractiveIndexCount (ViSession vi, ViPInt32 count);

ViStatus _VI_FUNC TLPAX_saveDataRecord (ViSession vi,ViInt32 record);
ViStatus _VI_FUNC TLPAX_restoreDataRecord (ViSession vi, ViInt32 record);
ViStatus _VI_FUNC TLPAX_resetDataRecord (ViSession vi, ViInt32 record);

// Snapshot functions
ViStatus _VI_FUNC TLPAX_startStopDataAquisition (ViSession vi, ViBoolean state);
ViStatus _VI_FUNC TLPAX_getDataAquisitionThreadState (ViSession instrumentHandle, ViPBoolean state);

ViStatus _VI_FUNC TLPAX_getOperatingHoursMotorRotations (ViSession vi, ViInt32 *opHours, ViInt32 *opMinutes, ViInt32 *motorRevs);
ViStatus _VI_FUNC TLPAX_raw_write(ViSession instr, ViChar command[]);

ViStatus _VI_FUNC TLPAX_raw_query(ViSession instr, ViChar command[], ViChar response[]);

ViStatus _VI_FUNC TLPAX_triggerSnapshot (ViSession vi);


ViStatus _VI_FUNC TLPAX_getSnapshotScan (ViSession vi,
                                         ViPUInt32 revolutionCount,
                                         ViPUInt32 timeStamp,
                                         ViPUInt32 statusFlag);

ViStatus _VI_FUNC TLPAX_getSnapshotResult1 (ViSession vi,
                                            ViPReal64 i0Korr, ViPReal64 i2Korr,
                                            ViPReal64 i4Korr, ViPReal64 p2Korr,
                                            ViPReal64 p4Korr);

ViStatus _VI_FUNC TLPAX_getSnapshotResult2 (ViSession vi,
                                            ViPReal64 theta, ViPReal64 eta,
                                            ViPReal64 ppol, ViPReal64 punpol,
                                            ViPReal64 ptotal, ViPReal64 DOP);

ViStatus _VI_FUNC TLPAX_getSnapshotSetup1 (ViSession vi,
                                           ViPUInt32 operatingMode,
                                           ViPUInt32 phaseOffset,
                                           ViPUInt32 FFTSize,
                                           ViPUInt32 DMASize,
                                           ViPUInt32 incrementCount  /*,
                                           ViPUInt32 DMAChunks       */);

ViStatus _VI_FUNC TLPAX_getSnapshotSetup2 (ViSession vi,
                                           ViPUInt32 miniPhaseReloadValue,
                                           ViPUInt32 clocksPerIncrement,
                                           ViPUInt32 revolutions,
                                           ViPUInt32 gainIndex,
                                           ViPUInt32 statusFlags,
                                           ViPReal64 wavelength);

ViStatus _VI_FUNC TLPAX_getSnapshotSetup3 (ViSession vi,
                                           ViPReal64 photoDiodeSensitivity,
                                           ViPReal64 polarizerAttenuation,
                                           ViPReal64 overallPowerFactor,
                                           ViPReal64 delta,
                                           ViPReal64 phi2Korr,
                                           ViPReal64 phi4Korr,
                                           ViPReal64 azimuthCorrectionAngle);

ViStatus _VI_FUNC TLPAX_getSnapshotRawResult1 (ViSession vi,
                                               ViPUInt32 revolutionCount,
                                               ViPUInt32 timeStamp,
                                               ViPReal64 revolutionTime,
                                               ViPUInt32 statusFlag,
                                               ViPUInt32 ADCMinimalValue,
                                               ViPUInt32 ADCMaximalValue);

ViStatus _VI_FUNC TLPAX_getSnapshotRawResult2 (ViSession vi,
                                               ViPReal64 misalignmentIndicator,
                                               ViPReal64 i0Raw,
                                               ViPReal64 i2Raw,
                                               ViPReal64 i4Raw,
                                               ViPReal64 p2Raw,
                                               ViPReal64 p4Raw);

ViStatus _VI_FUNC TLPAX_getSnapshotADCValues (ViSession vi,ViUInt16 ADCValues[], ViPUInt32 valuesCount);
ViStatus _VI_FUNC TLPAX_setSnapshotADCValues (ViSession vi,ViUInt16 ADCValues[], ViUInt32 valuesCount);
ViStatus _VI_FUNC TLPAX_recalculateSnapshot (ViSession vi);

ViStatus _VI_FUNC TLPAX_setSerialNumber (ViSession vi, ViChar serialNumber[]);
ViStatus _VI_FUNC TLPAX_setName (ViSession vi, ViChar name[]);
ViStatus _VI_FUNC TLPAX_setCalMessage (ViSession vi,ViChar calibrationMessage[]);

/**@}*/  // defgroup FPCLASS_SERVICE_x

ViStatus _VI_FUNC TLPAX_close (ViSession vi);

/**@}*/  // defgroup FPCLASS_ROOT_x

// Global functions

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __TLPAX_H__ */
