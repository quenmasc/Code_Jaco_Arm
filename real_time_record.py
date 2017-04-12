# all import here
import alsaaudio as alsa
import numpy as np
import math

# constant
fs = 16000 #sample rate fixed at 16kHz
n_channel=1 # number of channel use for record : 1 -> mono, 2 -> stereo.
format_style = alsa.PCM_FORMAT_S32_LE # sample is defined with float 32
buffer_period = 160 
# change buffer_period to have 1 sample per write

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
print("Creation of array to store data")
#buffer = np.empty([3,32000],dtype=np.float32)
buffer=[]
# store data
while True:
    l, data = inp.read()
    buffer.append(np.fromstring(data,dtype=np.float32))
    print(len(buffer))
    if len(buffer)>=100:
        #out.write(buffer[0])
        
	#del buffer[0]
	print(buffer)
