# all import here
import scipy.io.wavfile as wavfile
import os
import numpy as np
from pydub import AudioSegment
import inspect 
import mfccbuffer
import MFCC

__author__="Quentin MASCRET <quentin.mascret.1 ulaval.ca>"
__date__="2017-05-03"
__version__="1.0-dev"


def ReadAudioFile(path):
    extension=os.path.splitext(path)[1]
    if extension=='.wav':
        audioFile=AudioSegment.from_file(path)
        data=np.fromstring(audioFile._data, np.int16)
        fs=audioFile.frame_rate
        x=np.array(data)
        return fs, data
    else :
        print "Error in ReadAudioFile() : NO FILE TYPE"
        return (-1,-1)

def FindWavFile():
    listdirectory = os.listdir(".")
    for filename in listdirectory :
        if filename.endswith(".wav"):
	    fs,data=ReadAudioFile(filename)
	    Subframe(data)
            print(filename)

def Subframe(data,window_sample=200,window_shift=85):
     buff=mfccbuffer.MFFCsRingBuffer()    
     mfcc=MFCC.MFCCs()
     nbFrame=(len(data)-window_sample)/window_shift
     for i in xrange(nbFrame):
	signal=data[(i*window_shift+np.arange(window_sample))]
	buff.extend(mfcc.MFCC2(signal))	
     mfccs, fl=buff.get()
     print(mfccs)
FindWavFile()
