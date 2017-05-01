# all import here
import alsaaudio as alsa
from multiprocessing import Process, Queue 
import numpy as np
import math
import struct
import array
import time
import numpy.fft


__author__="Quentin MASCRET <quentin.mascret.1 ulaval.ca>"
__date__="2017-04-14"
__version__="1.0-dev"

THRESHOLD=0.1
""" Normalize the audio samples from an array of integers"""
def normalize(data,max_value) :
    data=np.array(data)
  #  biais=int(0.5 * max_value)
    fac=1.0/(max_value)
    data=fac * (data)# - biais )
    return data

def subframe(data,window_ms=0.025,step_ms=0.01, rate=16000) :
    window_sample=int(window_ms*rate)
    step_sample= int(step_ms*rate)
    nbFrame=int((len(data)-window_sample)/step_sample)
    print(nbFrame)
    for i in range(0,(nbFrame+1)) : # 197 frames
        vector= data[1+(i-1)*step_sample:1+(i-1)*step_sample+window_sample]
        yield vector
    

def threshold(snd_data):
    # return 'true' if below the 'silent' threshold
    return np.max(snd_data) > THRESHOLD

def logEnergy(frame):
    nfft=256 # the nextpow of 200 samples
    fft = numpy.fft.rfft(frame, nfft)
    # Square of absolute value
    power = fft.real * fft.real + fft.imag * fft.imag
    return np.log(np.sum(power))

    """
    Denormalize the data from an array of floats with unity level into an array of integers.
    """
def denormalize(data, max_val):
        #bias = int(0.5 * max_val)
        fac = max_val
        data = np.array(data)
        data = (fac * data)#.astype('H') + bias
        return data
