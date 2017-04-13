# all import here
import alsaaudio as alsa
from multiprocessing import Process, Queue 
import numpy as np
import math
import struct
import array
import time
import audioop


#constant
fs = 16000 #sample rate fixed at 16kHz
n_channel=1 # number of channel use for record : 1 -> mono, 2 -> stereo.
format_style = alsa.PCM_FORMAT_S16_LE # sample is defined with float 32
buffer_period = 160

# initialize sound card -> choose by default : -arecord -l in terminal allow to know name of default card
print("Initialization of sound card -> set parameters to sound card")
card='sysdefault:CARD=Device'
inp = alsa.PCM(alsa.PCM_CAPTURE, alsa.PCM_NORMAL, card)

# define paramters
inp.setchannels(n_channel)
inp.setrate(fs)
inp.setformat(format_style)
inp.setperiodsize(buffer_period)

# creation of an array of arrays
print("Recording ...")
# store data
buffer=[[],[],[]]

j = 0
i=0
while True:
        # Read data from device, if possible.
        raw_data=b''
        start=time.time()
        while len(raw_data)<2*fs:
            frame_count, data = inp.read()
            if frame_count >0 :
                raw_data+=data
        end=time.time()
        buffer[i]=raw_data
        print "buffer", i , " is full"
        i+=1
        i=i%3
        print(len(np.fromstring(raw_data[:2*fs],dtype=np.int16)))
        print(end-start)
   
