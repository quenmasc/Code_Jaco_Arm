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

__author__="Quentin MASCRET <quentin.mascret.1 ulaval.ca>"
__date__="2017-04-14"
__version__="1.1-dev"
class Record(object) :
    """Initialize audio buffer"""
    def __init__(self):
        self.__rate=16000 # sample rate
        self.__channels=1 # number of channel use in record
        # all queues
        self.__read_queue = Queue()
        self.__read_frame = Queue()
	self.__write_queue=Queue()
	# params  
        self.__window_ms=0.015
        self.__step_ms=0.005
	self.__format=alsa.PCM_FORMAT_S16_LE
	self.__max=192324 
	self.__byte =4
	self.__head=0
	self.__tail=0
	self.__cur=0
	self.__start=0
	self.__length=0
	if self.__format==alsa.PCM_FORMAT_S16_LE :
		self.__max=self.__max/2
		self.__byte=self.__byte/2
	self.__str_data=""
        self.__raw_data=[None for i in xrange(self.__max)]
	
    """"Reads audio from ALSA audio device """
    def __read(self) :
        card='sysdefault:CARD=Device'
        inp = alsa.PCM(
        alsa.PCM_CAPTURE, alsa.PCM_NONBLOCK,card)
        inp.setchannels(1)
        inp.setrate(self.__rate)
        inp.setformat(self.__format)
        inp.setperiodsize(self.__rate / 100)
        print("Audio Device is parameted")

        while True :
            frame_count, data = inp.read()
            self.__read_queue.put(data)
            self.__read_frame.put(frame_count)
    """ Write data into a list (ring buffer) -> intialize with None value at beggining"""     
    def __write(self):
        raw_data=[None for i in xrange(self.__max)]
	while True :
		data=self.__write_queue.get()
		# not really use just for test 
		
		# USE 
                self.__tail+=len(data)
                raw_data[self.__cur:self.__tail]=data
                self.__cur=self.__tail
                if self.__cur >= self.__max :
                        self.__str_data+=''.join(raw_data)
			f=wave.open("test.wav",'w')
			f.setnchannels(1)
			f.setsampwidth(self.__byte)
			f.setframerate(self.__rate)
			f.writeframes(self.__str_data)
			f.close()
			print("End of recording")
                if self.__cur >=self.__max :
                            self.__cur=0
                            self.__tail=0

    """ Run proccesses """
    def run(self):
        self.__read_process = Process(target=self.__read)
        self.__read_process.start()
	self.__write_process = Process(target=self.__write)
	self.__write_process.start()
    """ Stop processes """		
    def stop(self):
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
        cur=self.__cur
        return cur
            

if __name__=='__main__' :
    audio= Record()
    audio.run()
    while True :
        data, length = audio.read()
        audio.write(data,length)
     #   print(audio.push)
    print("out of loop")
    print("end of transmission -> waiting new data")
    audio.stop()
    
        
    
         
