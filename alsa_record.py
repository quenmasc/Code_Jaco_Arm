# all import here
import alsaaudio as alsa
from multiprocessing import Process, Queue , Pool , Lock
import numpy as np
import math
import struct
import array
import time
import DSP
import wave
import os
from threading import Thread
import RingBuffer

__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-04-14"
__version__="1.1-dev"

class Record(object) :
    """Initialize audio buffer"""
    def __init__(self):
        # all queues
        self.__read_queue = Queue()
        self.__read_frame = Queue()
        self.__write_queue=Queue()
	# params  
	self.__format=alsa.PCM_FORMAT_S16_LE # format of sample 
	self.__byte =4 # size of each sample 
	self.__rate=8000 # sample rate
        self.__channels=1 # number of channel use in record
	self.__max=48000 # length max of ring buffer for float values
	# change some parameters in terms of sample rate 
	if self.__format==alsa.PCM_FORMAT_S16_LE :
		self.__max=self.__max/2
		self.__byte=self.__byte/2
		self.__push_value=[self.__max/3, 2*self.__max/3,self.__max]
       # self.__raw_data=[None for i in xrange(self.__max)]





        #### NO PCM-NONBLOCK ELSE ERROR NO THE SAME CADENCE BETWEEN PROCESSES ####
    """"Reads audio from ALSA audio device """
    def __read(self) :
        card='sysdefault:CARD=Device'  # define default recording card 
        inp = alsa.PCM(alsa.PCM_CAPTURE, alsa.PCM_NORMAL,card) 
        inp.setchannels(1) # number of channels
        inp.setrate(self.__rate) # sample  rate
        inp.setformat(self.__format) # format of sample
        inp.setperiodsize(self.__rate / 50) # buffer period size
        print("Audio Device is parameted")
        

        while True :
            frame_count, data = inp.read()  # process to get all value from alsa buffer -> period size * bytes per sample
            self.__read_queue.put(data) # put data in queue -> string type
            self.__read_frame.put(frame_count) # put length -> over 0 data else None

    def __write(self):
        card='sysdefault:CARD=Device'
        outp = alsa.PCM(alsa.PCM_PLAYBACK, alsa.PCM_NORMAL,card)
        outp.setchannels(1)
        outp.setrate(self.__rate)
        outp.setformat(alsa.PCM_FORMAT_S16_LE)
        outp.setperiodsize(self.__rate / 50)

        while True:
            data = self.__write_queue.get()

            outp.write(data)



    def __pre_post_data(self):
        zeros = np.zeros(self.__rate / 50, dtype = np.int16)

        for i in range(0, self.__byte):
            self.__write_queue.put(zeros)


            

    """ Run proccesses """
    def run(self):
        #self.__pre_post_data()
        read_process = Process(target=self.__read)
        write_process = Process(target = self.__write)
        read_process.start()
        write_process.start()


    
    """ get all data from audiuo devices """
    def read(self):
        return self.__read_queue.get() , self.__read_frame.get()     

    def write(self, data):
        self.__write_queue.put(data)


    # Pseudonymize the audio samples from a binary string into an array of integers
    def pseudonymize(self, s):

        sl=len(s)/self.__byte
        return struct.unpack('<%dh' % sl,s)
    #np.fromstring(s[:2*8000], dtype=np.uint16)

    def depseudonymize(self, a):
        s = ""

        for elem in a:
            s += struct.pack('h', elem)

        return s

    """  Ring Buffer -> READ AND WRITE METHODS """
    def RingBufferWrite(ring,data):
    
if __name__=='__main__' :
    audio= Record()
    RingLength=24650
    window_sample=200
    step_sample=85
    buff=RingBuffer.RingBuffer(RingLength,window_sample,step_sample)
    store=RingBuffer.WaitingBuffer(10,window_sample)
    audio.run()
    cur=0
    tail=0
    i=0 
    c=[[],[]]
    start=time.time()
    while True :
        data, length = audio.read()
        if length>0 :
            pdata=audio.pseudonymize(data)
            #print(pdata)
            ndata=DSP.normalize(pdata,0xFFFF)
            #print(ndata)
            buff.extend(ndata)
            a=buff.index()
            if a> window_sample :
                j=0
                while j<2 :
                   c[j]=buff.get()
                   j+=1  
            stop=time.time()
            print(stop-start)
            #ndata=DSP.denormalize(ndata,0xFFFF)
            ndata=audio.depseudonymize(pdata)
            audio.write(ndata)
#
    print("out of loop")
    print("end of transmission -> waiting new data")
