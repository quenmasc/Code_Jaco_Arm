# all import here
import alsaaudio as alsa
from multiprocessing import Process, Queue 
import numpy as np
import math
import struct
import array
import time
__author__="Quentin MASCRET <quentin.mascret.1 ulaval.ca>"
__date__="2017-04-14"
__version__="1.0-dev"


""" Normalize the audio samples from an array of integers"""
def normalize(data) :
    vector = data-np.mean(data)
    max_value=np.max(np.abs(data))
    normalize_vector = vector / max_value
    return normalize_vector

def subframe(data,window_ms=0.015,step_ms=0.005, rate=16000) :
    window_sample=int(window_ms*rate)
    step_sample= int(step_ms*rate)
    nbFrame=int((len(data)-window_sample)/step_sample)
    print(nbFrame)
    for i in range(0,(nbFrame+1)) : # 197 frames
        print(i)
        vector= data[1+(i-1)*step_sample:1+(i-1)*step_sample+window_sample]
        yield vector
    #return data[(nbFrame-1)*step_sample+window_sample: len(data)]
    
   
    




