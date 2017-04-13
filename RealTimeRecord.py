# all import here
import alsaaudio as alsa
from multiprocessing import Process, Queue 
import numpy as np
import math
import struct
import array
import time

class Record :
    """Initialize audio buffer"""
    def __init__(self) :
        self.__rate=16000 # sample rate
        self.__channels=1 # number of channel use in record
        self.__read_queue = Queue()
        self.__read_frame = Queue()

    """"Reads audio from ALSA audio device """
    def __read(self) :
        card='sysdefault:CARD=Device'
        inp = alsa.PCM(alsa.PCM_CAPTURE, alsa.PCM_NORMAL,card)
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

audio= Record()
audio.run()
timeout=0
start=time.time()
while timeout<=10 :
    end=time.time()
    data , length = audio.read()
    print len(data) , length
    timeout=end-start
    
        
    
         
