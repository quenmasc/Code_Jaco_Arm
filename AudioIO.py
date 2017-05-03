# all import here
import scipy.io.wavfile as wavfile
import os
import numpy as np
from pydub import AudioSegment
import inspect 

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
            print(filename)

FindWavFile()
#def Subframe(data, 
