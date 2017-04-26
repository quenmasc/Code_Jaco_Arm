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

def sigmoid(Lambda,x):
    return 1/(5+math.exp(-Lambda*x)) # expit also allows it faster than mine.

def correlation_1D(x,x_noise)
    return scipy.spatial.distance.correlation(x,x_noise)

def correlation_ND(x,x_noise):
    

def update(x,x_old, p):
