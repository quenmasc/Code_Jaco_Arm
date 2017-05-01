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
import mfcc
import spectral_entropy
import function
import mfccbuffer

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
        self.__RingBufferWrite_queue=Queue()
        self.__RingBufferRead_queue=Queue()
	# params  
	self.__format=alsa.PCM_FORMAT_S16_LE # format of sample 
	self.__byte =4 # size of each sample 
	self.__rate=8000 # sample rate
        self.__channels=1 # number of channel use in record
	self.__max=48000 # length max of ring buffer for float values
	# change some parameters in terms of sample rate 
	if self.__format==alsa.PCM_FORMAT_S16_LE :
		self.__mgax=self.__max/2
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

        for i in range(0, gself.__byte):
            self.__write_queue.put(zeros)


            

    """ Run proccesses """
    def run(self):
        #self.__pre_post_data()
       # index=Queue()
        read_process = Process(target=self.__read)
        write_process = Process(target = self.__write)
        ring=RingBuffer.RingBuffer(RingLength,window_sample,step_sample)
        RingBuffer_write_process = Process (target =self.__RingBufferWrite, args=(ring,))
        read_process.start()
        RingBuffer_write_process.start()
        write_process.start()
        
        
    """__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-04-14"
__version__="1.0-dev" get all data from audiuo devices """
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
    def __RingBufferWrite(self,ring):
        flag=0
        temp=np.zeros(400)
        while True :
            data=self.__RingBufferWrite_queue.get()
            ring.extend(data)
            if flag==2 :
                for i in range(0,2):
                    temp.append(ring.get().tolist())
            else :
                flag+=1
                temp=np.zeros(400)
           # print "temp is " ,temp
            self.__RingBufferRead_queue.put(temp)
            temp=[]

    def RingBufferWrite(self,data):
        self.__RingBufferWrite_queue.put(data)
        
    def __RingBufferRead(self,ring):
        flag=0
        while True :
           # print("in")
            if flag==0:
                temp=0
                flag=1
            else :
                temp=ring.get()
          #  print(temp)

            self.__RingBufferRead_queue.put(temp)

    def RingBufferRead(self):
        return self.__RingBufferRead_queue.get()
    

if __name__=='__main__' :
    audio= Record()
    mfcc = mfcc.MFCC()
    entropy = spectral_entropy.SPECTRAL_ENTROPY()
    buff=mfccbuffer.MFFCsRingBuffer()
    RingLength=24650
    window_sample=200
    step_sample=85

   # store=RingBuffer.WaitingBuffer(10,window_sample)
    audio.run()
    cur=0
    tail=0
    i=0 
    c=[[],[]]
    d=[[],[]]
    j=0
    fl="out"
    count=0
    c=[]

    d=[[],[]]
    energy=[[],[]]
    f=[]
    mfc=np.empty((26,200),'d')
    audioData=[]
    spectral_entropy=np.empty(2,'f')
    th=[[],[]]
    endpoint=np.empty(2,'f')
    corr=[[],[]]
    flag=0
    while True :

        data, length = audio.read()
        pdata=audio.pseudonymize(data)
        ndata=DSP.normalize(pdata,0xff)
        audio.RingBufferWrite(ndata)
        if (c==[]) :
            c=audio.RingBufferRead()
        else :
            print ("Overwrite")
            break
        if flag < 3:
            flag+=1
        if flag ==3:
            for i in range(0,2) :
                d[i]=mfcc.frame2s2mfc(np.array(c[i]))
                energy[i]=DSP.logEnergy(np.array(c[i]))
                spectral_entropy[i]=entropy.SpectralEntropy(np.array(c[i]))
                if j==0 :
                    j+=1
                    f=np.array(d[0])
                
                corr[i]=function.correlation_1D(np.array(d[i]),f)
                if j==1 :
                  f=function.updateMFCCsNoise(np.array(d[i]),f, 0.9)
                  th[i]=function.sigmoid(1,corr[i])
                else :
                    th[i]=0.000001
                fl=buff.flag(corr[i],th[i],d[i],energy[i],np.array(c[i]))
                if fl=="admit" :
                    mfc,audioData=buff.get()
                    ### playback
                    file=wave.open('test.wav','wb')
                    file.setparams((1,2,8000,len(audioData),"NONE","not compressed"))
                    file.writeframes(audio.depseudonymize(DSP.denormalize(audioData,0xff)))
                    file.close()
                    ### end of playback
                    print("  ##########################################################################################")
                    print( " _________________________________________OVER ___________________________________________")
                    print("  ##########################################################################################")
        print ( " _____________________________NEW ________________________________")
        print "flag is : " , fl
        c=[]
        endpoint=[[],[]]
       # ndata=DSP.denormalize(pdata,0xFF)
        ndata=audio.depseudonymize(pdata)
        audio.write(ndata)
#
    print("out of loop")
    print("end of transmission -> waiting new data")
