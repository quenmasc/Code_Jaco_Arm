#all import below
import numpy as np
import math
import struct
import array
import time
import DSP
import wave
import os

__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-04-27"
__version__="1.0-dev"


# store MFFC coefficients in a ring buffer during data acquisition 
class MFFCsRingBuffer(object):
        """ Initialize the ring buffer"""
        def __init__(self):
            self.__data=np.zeros((13,800))
            self.__length=800
            self.__index=0
            self.__head=0
            self.__tail=0
            self.__sustain=0
            self.__count=0
            self.__flag="out"
            self.__numberOfWindowRejection=17 # 1600 samples


        def extend(self,data):
            data_index=(np.arange(data.size))
            if np.all(self.__data[data_index,self.__index]==np.zeros(len(data_index))) :
                    self.__data[data_index,self.__index]=data.T
                    self.__index+=1
                    self.__index=self.__index%self.__length
            else :
                print("Error : RingBuffer is overwritten ")

        def get(self):
                data_index=(self.__head+np.arange(self.__tail))
                return self.__data[:,data_index]

        def segmentation(self,data,threshold,coeff):
                # first case 
                if data >= thresold and flag=="out" :
                        self.__flag="in"
                        self.__tail+=1
                        
                if data<threshold and flag="in" :
                        self.__count+=1
                        self.extend(coeff)
                        if self.__count >=self.__numberOfWindowRejection:
                                return self.__get()
                
                                
                                
                
