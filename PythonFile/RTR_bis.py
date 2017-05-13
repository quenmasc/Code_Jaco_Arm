# all import here
import alsaaudio as alsa
from multiprocessing import Process, Queue 
import numpy as np
import math
import struct
import array
import time
import DSP
import wave
import os
from threading import Thread


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
	self.__push_queue= Queue()
	self.__checking_queue=Queue()
	# buffer params
	self.__head=0
	self.__tail=0
	self.__cur=0
	
	# params  
        self.__window_ms=0.015 # length of working window
        self.__step_ms=0.005 # shift between two windows
	self.__format=alsa.PCM_FORMAT_S16_LE # format of sample 
	self.__byte =4 # size of each sample 
	self.__rate=8000 # sample rate
        self.__channels=1 # number of channel use in record
	self.__max=48000 # length max of ring buffer for float values
	# change some parameters in terms of sample rate 
	if self.__format==alsa.PCM_FORMAT_S16_LE :
		self.__max=self.__max
/2
		self.__byte=self.__byte/2
		self.__push_value=[self.__max/3, 2*self.__max/3,self.__max]
       # self.__raw_data=[None for i in xrange(self.__max)]

        #### NO PCM-NONBLOCK ELSE ERROR NO THE SAME CADENCE BETWEEN PROCESSES ####
    """"Reads audio from ALSA audio device """
    def __read(self) :
        card='sysdefault:CARD=Device'  # define default recording card 
        inp = alsa.PCM(alsa.PCM_CAPTURE, alsa.PCM_NONBLOCK,card) 
        inp.setchannels(1) # number of channels
        inp.setrate(self.__rate) # sample  rate
        inp.setformat(self.__format) # format of sample
        inp.setperiodsize(self.__rate / 50) # buffer period size
        print("Audio Device is parameted")
        
        while True :
            frame_count, data = inp.read()  # process to get all value from alsa buffer -> period size * bytes per sample
            self.__read_queue.put(data) # put data in queue -> string type
            self.__read_frame.put(frame_count) # put length -> over 0 data else None
               # print(len(data))
            
    """ Write data into a list (ring buffer) -> intialize with None value at beggining"""     
    def __write(self,queue,fl):
        raw_data=[[],[],[]]
        i=0
        flag=0
	while True :
		data=self.__write_queue.get() # retrieve data
		data=np.array(data)
		flag=0
		self.__tail+=len(data)
                raw_data[self.__cur:self.__tail]=data
                self.__cur=self.__tail
		if self.__cur>=self.__push_value[i]:
                    queue.put(raw_data[self.__head:self.__push_value[i]])
                    self.__head=self.__push_value[i]
                    flag=1
                    i+=1
                    i=i%len(self.__push_value)
                
                if self.__cur >=self.__max :
                        self.__cur=0
                        self.__tail=0    
                
                fl.put(flag)
    """ Push buffer when is full"""
    def __push(self,queue) :
        while True :
            self.__push_queue.put(queue.get())
                
    def __checking(self,fl):
        while True :
                self.__checking_queue.put(0)
                if (fl.get()==1):
                    self.__checking_queue.put(1)
               
    """ Run proccesses """
    def run(self):
        queue=Queue()
        fl=Queue()
     #   self.__pre_post_data()
        self.__read_process = Process(target=self.__read)
        self.__read_process.start()
        self.__write_process = Process(target=self.__write,args=(queue,fl,))
        self.__write_process.start()
        self.__checking_process = Process(target=self.__checking,args=(fl,))
        self.__checking_process.start()
        self.__push_process = Process(target=self.__push,args=(queue,))
        self.__push_process.start()
    """ Stop processes """		
    def stop(self):
        # not really usefull for the moment -> need to kill process with ctrl+z due to while loop in main
        self.__read_process.terminate()
        self.__write_process.terminate()
        

    """ get all data from audiuo devices """
    def read(self):
        return self.__read_queue.get() , self.__read_frame.get()       


   # Pseudonymize the audio samples from a binary string into an array of integers
    def pseudonymize(self, s):
        return struct.unpack("<" + ("h" * (len(s) / self.__byte)), s)


   # Depseudonymize the audio samples from an array of integers into a binary string.

    def depseudonymize(self, a):
        s = ""



        for elem in a:
            s += structn.pack(">I", elem)

        return s


    """ put in queue alla data record """
    def write(self,data,length):
	#if length>0:
        	self.__write_queue.put(data)

    def __pre_post_data(self):
        zeros = np.zeros(self.__rate / 100, dtype = np.int16)

        for i in range(0, self.__byte):
            self.__write_queue.put(zeros)

    def push(self):
        return self.__push_queue.get()

    def checking(self):
        return self.__checking_queue.get(
)
        
        

if __name__=='__main__' :
    audio= Record()
    audio.run()
    start=time.time()
    while True :
        data, length = audio.read()
        if length>0 :
            pdata=audio.pseudonymize(data)
            data_n=DSP.normalize(pdata,0xffff)
            #audio.write(data_n,length)
            #a=audio.checking()
            #if a==1 :
               # r=audio.push()
            stop=time.time()
            #print(len(r))
        print(stop-start)
    print("out of loop")
    print("end of transmission -> waiting new data")
    
        
    
         
