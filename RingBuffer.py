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
__version__="1.0-dev"

# RING BUFFER #
class RingBuffer(object):

    """  Initialize ring buffer """
    def __init__(self,length,window_sample,step_sample):
        self.__data=np.zeros(length)
        self.__index=0
        self.__length=length
        self.__window=window_sample
        self.__step=step_sample
        self.__shift=0 # index for get
        self.__clean=0

    """ Add data to the ring buffer -> data is anarray of float I suppose """
    def extend(self,data):
        data_index=(self.__index + np.arange(data.size))
        if np.all(self.__data[data_index]==np.zeros(len(data_index))) :
            self.__data[data_index]=data
        #    print( self.__data[data_index])
            self.__index=data_index[-1]+1
            self.__index=self.__index%self.__length
        else :
            print("Error : RingBuffer is overwritten ")
            
        
    """ get data from ring buffer FIFO -> idea : get working window at each time"""
    def get(self):
        idx=(self.__shift + np.arange(self.__window))
        idx_clean=(self.__clean + np.arange(self.__step))
        if self.__shift+self.__window > self.__length :
            temp_end=(np.arange(self.__shift,self.__length))
            temp_beg=(0+np.arange(self.__window-temp_end.size))
            idx= np.concatenate([temp_end,temp_beg])
        self.__shift+=self.__step
        self.__clean+=self.__step
        if self.__shift >= self.__length :
            self.__shift=self.__shift-self.__length
        if self.__clean >= self.__length :
            self.__clean=self.__clean-self.__length
        temp=self.__data[idx]
        self.__data[idx_clean]=np.zeros(self.__step)
       # print idx, self.__shift
        return temp

    def index(self):
        return self.__index


# DATASET TO SAVE WORKING WINDOW
class WaitingBuffer(object):

    """ Initialzation -> define the size of the array """
    def __init__(self,size,window_sample):
        self.__size=size
        self.__storeArray=np.ndarray((window_sample,size), dtype='f')
        self.__storeArray[:]=0.
       # print(self.__storeArray)
        self.__index=0
        
   # def store(self,DataArray) :
    #    for i in range(len(DataArray)):
