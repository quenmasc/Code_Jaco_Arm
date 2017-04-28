# all import here
import numpy as np
import math
import struct
import array
import time
import DSP
import wave
import os
import scipy
import numpy.matlib as mat
from Tkinter import END
from scipy.signal import lfilter

__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-04-26"
__version__="1.0-dev"

def sigmoid(Lambda,x):
       return 1/(5+math.exp(-Lambda*x)) # expit also allows it faster than mine.

    
def correlation_1D(x,x_noise):
    return scipy.spatial.distance.correlation(x,x_noise)

def distance(x,x_noise):
    return math.fabs(x-x_noise)

def updateMFCCsNoise(x,x_noise, p):
    # x and x_noise need to be an np.array
      return p*x_noise+(1-p)*x


def MeanStandardDeviation(x, Lambda):
    return np.mean(x)+Lambda*np.std(x)


def deltaMFCCs(MFCCs,w):
       hlen=int(np.floor(w/2))
       win=np.linspace(4,-4,w)
       x_b=mat.repmat(MFCCs[:,0],hlen,1).T
       x_e=mat.repmat(MFCCs[:,(MFCCs.size/len(MFCCs))-1],hlen,1).T 
       x_mfcc=np.array(MFCCs)
       xx=np.concatenate((x_b,x_mfcc,x_e),axis=1)
       d=lfilter(win,1,xx,axis=1)
       last_step=d[:,2*hlen+np.arange(MFCCs.size/len(MFCCs))]
       return last_step
