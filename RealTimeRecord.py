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
        self.__read_queue = Queue()
        self.__read_frame = Queue()
	self.__write_queue=Queue()
        self.__window_ms=0.015
        self.__step_ms=0.005
	self.__format=alsa.PCM_FORMAT_S32_LE
	self.__max=192323 # max-1
	self.__byte =4
	self.__head=0
	self.__tail=0
	self.__cur=0
	# for a try
	self.__start=0
	self.__data=b''
	self.__length=0
	if self.__format==alsa.PCM_FORMAT_S16_LE :
		self.__max=self.__max/2
		self.__byte=self.__byte/2
	self.__raw_data=[None for i in xrange(self.__max)]
	print "buffer size is " ,len(self.__raw_data)
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

    """ I don't know if it's necessary but runs the read processes"""
    def run(self):
        self.__read_process = Process(target=self.__read)
        self.__read_process.start()
	self.__write_process = Process(target=self.__write)
	self.__write_process.start()
    def stop(self):
        self.__read_process.terminate()
        
    def read(self):
        return self.__read_queue.get() , self.__read_frame.get()       
    
    def write(self,data,length):
	if length>0:
        	self.__write_queue.put(data)
    def __write(self):
	while True :
		data=self.__write_queue.get()
		if self.__cur >= self.__max :
			f=wave.open("test.wav",'wb')
			f.setnchannels(1)
			f.setsampwidth(self.__byte)
			f.setframerate(self.__rate)
			f.writeframes(','.join(str(self.__raw_data)))
			#f.writeframesraw(np.fromstring(self.__raw_data[:self.__byte*self.__rate],dtype=np.int32))
			f.close()
			audio.stop()
		self.__tail+=len(data)
		print "current position is :",self.__cur," finale position is :", self.__tail-1
		self.__raw_data[self.__cur:self.__tail-1]=data
		self.__cur=self.__tail
		

    def read_data(self):
        print("Reading data")
    #    while True :
#	        data, length = audio.read()
#		audio.write(data,length)
 #       buffer=[]
#	i=0
#	self.__start=time.time()
#        while True:
#		self.__data, self.__length=audio.read()
#		audio.write()
	#if length>0 :
#		audio.append()
               # raw_data=b''
        
        #        while len(raw_data)<self.__byte*self.__rate :
        #            data, length = audio.read()
		    
        #            if length >0 :
        #                raw_data+=data
	#			i+=1
	#			print(i)
	#			print(len(data))
	#			print "leng of raw_data is" , len(raw_data)
	#			print(len(np.fromstring(raw_data[:self.__byte*self.__rate],dtype=np.int32)))
	#                if raw_data==[] :
	#                  print ("Buffer is empty -> fatal error ! "	)
	#                    break
	#                else :
	#                    buffer=np.fromstring(raw_data[:2*self.__rate],
	#                                        dtype=np.int16)
	#                yield buffer
	#                buffer=[]
	#                del raw_data
	#                i+=1
	#                i=i%3
               # print(len(np.fromstring(raw_data[:2*rate],dtype=np.int16)))
               # print(np.fromstring(raw_data[:2*rate],dtype=np.int16))
         
  
if __name__=='__main__' :
    audio= Record()
    audio.run()
    while True :
    	data, length = audio.read()
    	r=audio.write(data,length)
    print("out of loop")
    #while True :
#	self.__data, self.__length=audio.read()
#        audio.write()
    audio.read_data()
   # a=r.next()
#    d=DSP.normalize(a)
#    f=DSP.subframe(d)
#    for i in range(0,198):
#        print(f.next())
    print("end of transmission -> waiting new data")
    audio.stop()
    
        
    
         
