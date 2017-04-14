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
class Record(object) :
    """Initialize audio buffer"""
    def __init__(self):
        self.__rate=16000 # sample rate
        self.__channels=1 # number of channel use in record
        self.__read_queue = Queue()
        self.__read_frame = Queue()

    """"Reads audio from ALSA audio device """
    def __read(self) :
        card='sysdefault:CARD=Device'
        inp = alsa.PCM(
        alsa.PCM_CAPTURE, alsa.PCM_NORMAL,card)
        inp.setchannels(1)
        inp.setrate(self.__rate)
        inp.setformat(alsa.PCM_FORMAT_S32_LE)
        inp.setperiodsize(self.__rate / 100)
        print("Audio Device is parameted")

        while True :
            frame_count, data = inp.read()
            self.__read_queue.put(data)
            self.__read_frame.put(frame_count)

    """ I don't know if it's necessary but runs the read processes"""
    def run(self):
        read_process = Process(target=self.__read)
        read_process.start()

    def read(self):
        return self.__read_queue.get() , self.__read_frame.get()


    def push_up(self,buffer):
        return buffer
       

    def read_data(self):
        print("Reading data")
        buffer=[[],[],[]]
        i=0
        while True:
                raw_data=b''
                while len(raw_data)<2*self.__rate :
                    data, length = audio.read()
                    if length >0 :
                        raw_data+=data
                if buffer[i]!=[] :
                    print "Buffer ", i ," is not empty -> fatal error ! "
                    break
                else :
                    buffer[i]=np.fromstring(raw_data[:2*self.__rate],
                                        dtype=np.int16)
                yield buffer[i]
                buffer[i]=[]
                i+=1
                i=i%3
               # print(len(np.fromstring(raw_data[:2*rate],dtype=np.int16)))
               # print(np.fromstring(raw_data[:2*rate],dtype=np.int16))


if __name__=='__main__' :
    audio= Record()
    audio.run()
    r=audio.read_data()
    while True :
        print(r.next())
        print("end of transmission -> waiting new data")
    
        
    
         
