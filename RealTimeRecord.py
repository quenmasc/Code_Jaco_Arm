# all import here
import alsaaudio as alsa
from multiprocessing import Process, Queue 
import numpy as np
import math
import struct
import array
import time
import DSP

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
        self.__window_ms=0.015
        self.__step_ms=0.005

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
        self.__read_process = Process(target=self.__read)
        self.__read_process.start()

    def stop(self):
        self.__read_process.terminate()
        
    def read(self):
        return self.__read_queue.get() , self.__read_frame.get()


    def push_up(self,buffer):
        return buffer
       

    def read_data(self):
        print("Reading data")
        buffer=[]
        while True:
                raw_data=b''
                while len(raw_data)<2*self.__rate :
                    data, length = audio.read()
                    if length >0 :
                        raw_data+=data
                if raw_data==[] :
                    print ("Buffer is not empty -> fatal error ! ")
                    break
                else :
                    buffer=np.fromstring(raw_data[:2*self.__rate],
                                        dtype=np.int16)
                yield buffer
                buffer=[]
                del raw_data
                i+=1
                i=i%3
               # print(len(np.fromstring(raw_data[:2*rate],dtype=np.int16)))
               # print(np.fromstring(raw_data[:2*rate],dtype=np.int16))


if __name__=='__main__' :
    audio= Record()
    audio.run()
    r=audio.read_data()
    a=r.next()
    d=DSP.normalize(a)
    f=DSP.subframe(d)
    for i in range(0,198):
        print(f.next())
    print("end of transmission -> waiting new data")
    audio.stop()
    
        
    
         
