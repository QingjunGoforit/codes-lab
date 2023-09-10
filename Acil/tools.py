import numpy as np
import scipy as sc

## Heebner
def heebnerSingleFun(x, a, b):
    return (a**2 - (2*b*a*np.cos(x)) + b**2)/(1 - (2*b*a*np.cos(x)) + b**2*a**2)

def heebnerSingle(x, params):
    off = params[0]; paramsRest = params[1:]
    return off + sum([heebnerSingleFun(x, *paramsRest[i:i+2]) for i in range(0, len(paramsRest), 2)])

def heebnerSingleRes(params, xData, yData):
    diff = [heebnerSingle(x, params)-y for x, y in zip(xData, yData)]
    return diff

## Mihai
def mihaiDoubletFun(x, a, b, c):
    a = np.abs(a); b = np.abs(b); c = np.abs(c)
    nominator = a + (1+a)**2*b*c + a*c**2 - 2*(np.sqrt(a*b*c)+np.sqrt(a**3*b*c)+np.sqrt(a*b*c**3)+np.sqrt(a**3*b*c**3))*np.cos(x) + 2*a*c*np.cos(2*(x))
    denominator = 1 + a*c*(4*b + a*c) - 4*(np.sqrt(a*b*c)+np.sqrt(a**3*b*c**3))*np.cos(x) + 2*a*c*np.cos(2*(x))
    return nominator/denominator

def mihaiDoublet(x, params):
    off = params[0]; paramsRest = params[1:]
    return off + sum([mihaiDoubletFun(x, *paramsRest[i:i+3]) for i in range(0, len(paramsRest), 3)])

def mihaiDoubletRes(params, xData, yData):
    diff = [mihaiDoublet(x, params)-y for x, y in zip(xData, yData)]
    return diff

## Aspelmeyer
def aspelmeyerSingleFun(x, a, b):
    return 1-(a/(0.5*b + x*1j))

def aspelmeyerSingle(x, params):
    off = params[0]; paramsRest = params[1:]
    return off + sum([aspelmeyerSingleFun(x, *paramsRest[i:i+2]) for i in range(0, len(paramsRest), 2)])

def aspelmeyerSingleRes(params, xData, yData):
    diff = [aspelmeyerSingle(x, params)-y for x, y in zip(xData, yData)]
    return diff


def aspelmeyerDoubletFun(x, a, b, c, d, e):
    return 1-(a/(1j*(x-b) + 0.5*c))-(a/(1j*(x-d) + 0.5*e))

def aspelmeyerDoublet(x, params):
    off = params[0]; paramsRest = params[1:]
    return off + sum([aspelmeyerDoubletFun(x, *paramsRest[i:i+5]) for i in range(0, len(paramsRest), 5)])

def aspelmeyerDoubletRes(params, xData, yData):
    diff = [aspelmeyerDoublet(x, params)-y for x, y in zip(xData, yData)]
    return diff


## four params Lorentzian, single and multiple
def lorentzFun(x, a, b, c, d):
    return -a*(c**2/((x-b)**2 + c**2)) + d ## a*pi amplitude, b center, c sigma, d offset


def lorentzSingle(x, params):
    off = params[0]; paramsRest = params[1:]
    return off + sum([lorentzFun(x, *paramsRest[i:i+4]) for i in range(0, len(paramsRest), 4)])

def lorentzSingleRes(params, xData, yData):
    diff = [lorentzSingle(x, params)-y for x, y in zip(xData, yData)]
    return diff


def lorentzMultiple(x, params):
    off = params[0]; paramsRest = params[1:]
    return off + sum([lorentzFun(x, *paramsRest[i:i+4]) for i in range(0, len(paramsRest), 4)])

def lorentzMultipleRes(params, xData, yData):
    diff = [lorentzMultiple(x, params)-y for x, y in zip(xData, yData)]
    return diff



## special functions
def nGroupFun(lamb, radius):
    """
    lamb is wavelength in m
    radius is ring resonator radius. for Antigua, either '1000' or '1600'
    """
    if radius == '1000':
        nGroup = 2.30025818 + -8.83471004e-4*(lamb*1e9) + 3.29677825e-7*(lamb*1e9)**2 + -4.01140785e-11*(lamb*1e9)**3
    elif radius == '1600':
        nGroup = 2.30326690 + -8.89655823e-4*(lamb*1e9) + 3.33826981e-7*(lamb*1e9)**2 + -4.10158405e-11*(lamb*1e9)**3
    return nGroup

def fsrLambFun(lamb, radius):
    """
    lamb is wavelength in m
    radius is ring resonator radius. for Antigua, either '1000' or '1600'
    """
    if radius == '1000':
        fsrLamb = 2.31671444e-11 + -6.40713386e-14*(lamb*1e9) + 1.07582145e-16*(lamb*1e9)**2 + -8.17663669e-21*(lamb*1e9)**3
    elif radius == '1600':
        fsrLamb = 2.29404535e-11 + -6.36331377e-14*(lamb*1e9) + 1.07309944e-16*(lamb*1e9)**2 + -8.12338473e-21*(lamb*1e9)**3
    return fsrLamb

def coupFun(lamb, radius, gap, coup):
    """
    lamb is wavelength in m
    radius is ring resonator radius. either '1000' or '1600'
    gap is '2.5' and whatnots
    coup is either 'self' or 'cross
    """
    if radius == '1000':
        if gap == '2.5':
            if coup == 'self': return 18.43249371 + -44176183.1658182*lamb + 41798309256950.29*lamb**2 + -1.7481632853688244e+19*lamb**3 + 2.722848959319754e+24*lamb**4
            elif coup == 'cross': return 2.00647493 + -6136688.23308723*lamb + 7016127518001.358*lamb**2 + -3.5587855413468104e+18*lamb**3 + 6.764992092898619e+23*lamb**4
    elif radius == '1600':
        if gap == '2.5':
            if coup == 'self': return 20.72516267 + -49451146.80521039*lamb + 46193624302956.73*lamb**2 + -1.902115046243164e+19*lamb**3 + 2.9056194418683216e+24*lamb**4
            elif coup == 'cross': return 4.98214596 + -14382803.33016569*lamb + 15630442777582.727*lamb**2 + -7.5811363164801e+18*lamb**3 + 1.3851694715287498e+24*lamb**4

def powerVoltCal(x, serial:2103):
    """
    x is wavelength in m
    serial is PD's serial number
    reference: https://www.newport.com/mam/celum/celum_assets/np/resources/Model_2103_Spectral_Calibration_Sample.pdf?0
    """

    if serial == 2348:
        volt = 3.5251 + 0.0075*(x*1e6) + -0.0179*(x*1e6)**2
    if serial == 2319:
        volt = 0.1204 + 4.3954*(x*1e6) + -1.4268*(x*1e6)**2
    else:
        volt = 3.3499 + 0.2336*(x*1e6) + -0.0925*(x*1e6)**2

    return volt

## stats
def getStats(x, q:0.95):
    """
    x is a numpy array
    q is 1 - significance level
    """
    sampleMean = np.mean(x)
    stDevPop = np.std(x, ddof=0)
    stDevSample = np.std(x, ddof=1)
    stError = stDevSample/np.sqrt(len(x))
    if len(x) >= 30:
        confInt = sc.stats.norm.ppf(q)*stError
    else:
        confInt = sc.stats.t.ppf(1-0.5*(1-q), len(x)-1)*stError

    return sampleMean, stDevPop, stDevSample, stError, confInt
