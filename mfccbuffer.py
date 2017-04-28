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
            self.__data=np.zeros(3900)
            self.__length=3900
            self.__index=0
            self.__tail=0
            self.__count=0
            self.__flag="out"
            self.__numberOfWindowRejection=20 # 1600 samples
            self.__lengthOfWindowMinima=130  # need to adapt this value 10*39 


        def extend(self,data):
            data_index=(self.__index+np.arange(data.size))
            if np.all(self.__data[data_index]==np.zeros(len(data_index))) :
                    self.__data[data_index]=data
                    self.__index=data_index[-1]+1
                    self.__index=self.__index%self.__length
            else :
                    print("Error : RingBuffer is overwritten ")

        def get(self):
                temp=self.__data
                self.__data=np.zeros(3900)
                self.__out="out"
                return temp

        def flag(self,data,threshold,coeff):
                # first case
                if data<threshold and self.__flag=="rejeted" :
                        self.__flag="out"
                        
                if data<threshold and self.__flag=="admit" :
                        self.__flag="out"
                        
                if data >= threshold and self.__flag=="out" :
                        self.__flag="in"
                        
                if data<threshold and self.__flag=="in" :
                        self.__flag="io"

                if data >= threshold and self.__flag=="io" :
                        self.__flag="in"



                if self.__flag=="in" :
                        self.__tail=self.__index
                        self.extend(coeff)
                        self.__count=0
                        
                if self.__flag=="io" :

                        if self.__count <=self.__numberOfWindowRejection :
                                self.__count+=1
                                self.extend(coeff)
                        else :
                                delete_index=(self.__tail+np.arange(self.__index-self.__tail))
                                self.__data[delete_index]=0.
                                self.__count=0
                                self.__flag="done"
                                print "_____ TAIL ___________", self.__tail , " ____ HERE ____", " ____INDEX _____", self.__index
                                self.__index=0

                if self.__flag=="done" :
                        if self.__tail< self.__lengthOfWindowMinima :
                                self.__flag="rejeted"
                                self.__data=np.zeros(3900)
                        else :
                                self.__flag="admit"
                                self.__data=np.zeros(3900) # delete it when function is in main
                return self.__flag
                                
                
