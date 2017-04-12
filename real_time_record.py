# all import here
import alsaaudio as alsa
import numpy as np


# constant
fs = 16000 #sample rate fixed at 16kHz
n_channel=1 # number of channel use for record : 1 -> mono, 2 -> stereo.
format_style = alsa.PCM_FORMAT_S32_LE # sample is defined with float 32
buffer_period = 1024

# initialize sound card -> choose by default : -arecord -l in terminal allow to know name of default card
print("Initialization of sound card -> set parameters to sound card")
card='sysdefault:CARD=Device'
inp = alsa.PCM(alsa.PCM_CAPTURE, alsa.PCM_NONBLOCK, card)

# define paramters
inp.setchannels(n_channel)
inp.setrate(fs)
inp.setformat(format_style)
inp.setperiodsize(buffer_period)

# creation of an array of arrays
print("Creation of array to store data")
buffer = np.empty([3,32000],dtype=np.float32)

# store data
data=0.12
i=0
while True :
    l, data=inp.read()
    
   # buffer[0,i]=np.fromstring(inp.write(data),dtype=np.float32)
   # print(buffer[0])
   # i=i+1

