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
	# params  
        self.__window_ms=0.015 # length of working window
        self.__step_ms=0.005 # shift between two windows
	self.__format=alsa.PCM_FORMAT_S16_LE # format of sample 
	self.__max=192324 # length max of ring buffer for float values
	self.__byte =4 # size of each sample 
	self.__rate=16000 # sample rate
        self.__channels=1 # number of channel use in record
	# buffer params
	self.__tail=0
	self.__cur=0
	self.__start=0 
	# params for bufer push out 
	self.__shift=0
	self.__push_value=[self.__max/3, self.__max/2,self.__max]
	# change some parameters in terms of sample rate 
	if self.__format==alsa.PCM_FORMAT_S16_LE :
		self.__max=self.__max/2
		self.__byte=self.__byte/2
		self.__push_value=[self.__max/3, self.__max/2,self.__max]
	# define ring buffer
        self.__raw_data=[None for i in xrange(self.__max)]
	
    """"Reads audio from ALSA audio device """
    def __read(self) :
        card='sysdefault:CARD=Device'  # define default recording card 
        inp = alsa.PCM(alsa.PCM_CAPTURE, alsa.PCM_NONBLOCK,card) 
        inp.setchannels(1) # number of channels
        inp.setrate(self.__rate) # sample  rate
        inp.setformat(self.__format) # format of sample
        inp.setperiodsize(self.__rate / 100) # buffer period size
        print("Audio Device is parameted")

        while True :
            frame_count, data = inp.read()  # process to get all value from alsa buffer -> period size * bytes per sample
            self.__read_queue.put(data) # put data in queue -> string type
            self.__read_frame.put(frame_count) # put length -> over 0 data else None
    """ Write data into a list (ring buffer) -> intialize with None value at beggining"""     
    def __write(self):
        buff_str=[]
        buff_int=[]
	while True : 
		data=self.__write_queue.get() # retrieve data 
                self.__tail+=len(data) # define tail (end of current data in buffer) as length of data 
                self.__raw_data[self.__cur:self.__tail]=data# put data in ring buffer at a given position
                self.__cur=self.__tail # change current value (begin of data position in current loop) -> tail value 
                if self.__cur >=self.__max : # definition of ring buffer 
                        self.__cur=0
                        self.__tail=0
# here I would like to push out a buffer when it's full. But I don't
# know how to proceed -> perhap thread in process but that seems special, isn't it ? Process not use same memories while p
# thread use the same 
##### begin idea here  ########
                if self.__cur >= self.__push_value[self.__shift] and len(buff_str)==0: # if current position in ring buffer is equal or over push value
                    buff_str=self.__raw_data[self.__start:self.__push_value[self.__shift]] # put a part of ring buffer in a buffer
                    buff_str=','.join(str(buff_str))
                    buff_int= np.fromstring(buff_str[:self.__byte*self.__rate], dtype=np.int16) # convert string to array of integers 
                    if len(buff_int) == self.__rate : # check if 1second of data in buffer 
                        self.__start=self.__push_value[self.__shift] # change start value 
                        self.__shift+=1
                        self.__shift=self.__shift%(len(self.__push_value))# change shift value [0 .. 1 .. 2 ]
                        self.__push_queue.put(buff_int)
                        print("buffer pushed")
                        buff_int=[]
                        buff_str=[]
                    else :
                        print("Error")
                       # yield buffer_int # return buffer_int with yield to avoid to leave process 
##### end of idea ######
    """ Run proccesses """
    def run(self):
        self.__read_process = Process(target=self.__read)
        self.__read_process.start()
        self.__write_process = Process(target=self.__write)
        self.__write_process.start()
    """ Stop processes """		
    def stop(self):
        # not really usefull for the moment -> need to kill process with ctrl+z due to while loop in main
        self.__read_process.terminate()
        self.__write_process.terminate()
        

    """ get all data from audiuo devices """
    def read(self):
        return self.__read_queue.get() , self.__read_frame.get()       

    """ put in queue alla data record """
    def write(self,data,length):
	if length>0:
        	self.__write_queue.put(data)

    def push(self):
        print("In")
        return self.__push_queue.get()
            
if __name__=='__main__' :
    audio= Record()
    audio.run()
    while True :
        data, length = audio.read()
        audio.write(data,length)
        #buf=audio.push()
        #print(buf)
    print("out of loop")
    print("end of transmission -> waiting new data")
    audio.stop()
    
        
    
         
