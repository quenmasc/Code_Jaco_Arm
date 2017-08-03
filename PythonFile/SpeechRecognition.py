import alsaaudio as alsa
from multiprocessing import Process, Queue , Pool , Lock
import numpy as np
import alsa_record
import AudioIO
import MFCC
import spectral_entropy
import mfccbuffer
from collections import deque
import DSP
import function
import threading
import MachineLearning
import tools
import os
import time
import errno
import Sphere

__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-05-11"
__version__="1.0-dev"

class Speech_Recognition(object):
    """ Semaphore """
    MfccsCoeff=np.empty((39,100),'f')
    audioData=np.array([])
    ClassLab=0
    """ Thread """ 
    def __init__(self):
        self.__maxconnections=2
        self.__semaphore=threading.BoundedSemaphore(self.__maxconnections)
        self.__condition=threading.Condition()
        self.__semaphore2=threading.BoundedSemaphore(self.__maxconnections)
        self.__condition2=threading.Condition()
        
        self.__fifo_name= 'fifo'
        
        
    def Recorder(self):

        global MfccsCoeff
        global audioData
        audio= alsa_record.Record()

        mfcc = MFCC.MFCCs()
        entropy = spectral_entropy.SPECTRAL_ENTROPY()
        buff=mfccbuffer.MFFCsRingBuffer()
        RingLength=24650
        window_sample=200
        step_sample=80

   # store=RingBuffer.WaitingBuffer(10,window_sample)

        audio.run()
        i=0 
        c=[[],[]]
        j=0
        fl="out"
        count=0
        c=[] 
        coeff=np.empty(13,'f')
        energy=np.zeros(1)

    # mfcc
        mfccN=np.zeros(13)
        mfccNoise=np.zeros(13)
    # entropy
        SEntropy=np.zeros(13)

        entropyNoise=0
        entropyDistance =0
        entropyN=0
         # threshold

        entropyData =deque([])

        entropyThreshNoise=0
        entropyThresh = 0
    #audio 
        audioData=[]
        s=0
        th=[[],[]]
        endpoint=np.empty(2,'f')
        corr=np.empty((2,1),'f')
        flag=0

        while True :
            data, length = audio.read()
            pdata=audio.pseudonymize(data)
            ndata=DSP.normalize(pdata,32767.0)
            audio.RingBufferWrite(function.LowPass(ndata))
            if (c==[]) :
                c=audio.RingBufferRead()
 
            else :

                print ("Overwrite")
                break
            if flag < 4:

                flag+=1

            else :
                for i in range(0,2) :
					# return MFCC and spectral entropy
                    coeff,energy=mfcc.MFCC(np.array((c[i])))
                    SEntropy=entropy.SpectralEntropy(np.array((c[i])))
                    if j<100 :

                        mfccNoise+=np.array(coeff)
                        entropyData.append(SEntropy)
                        j+=1
                        if j==100 :
                            mfccNoise=mfccNoise/100
                            entropyNoise = np.mean(entropyData)
                            Data=entropyData
                            entropyData=deque([])
                            for k in range(0,len(Data)) :
                                entropyData.append(function.distance(Data[k],entropyNoise))
                            entrrolpyThreshNoise =function.MeanStandardDeviation(entropyData,3)
                            print tools.bcolors.OKBLUE + "Recording is now allowed" +tools.bcolors.ENDC
                            n=deque([])
                            for i in range(0,20):
								n.append(entropyData[79+i])
                            entropyData=deque([])
                            entropyData=n
                    else :
                    # return MFCC and Spectral Entropy background noise
                        mfccN=function.updateMFCCsNoise(np.array(coeff),mfccNoise, 0.95)
                        entropyN=function.updateEntropyNoise(SEntropy,entropyNoise, 0.95)
                    
                    # return correlation and distance of MFCC and Entropy
                        corr=function.correlation_1D(np.array(coeff),mfccN)
                        entropyDistance=function.distance(SEntropy,entropyN)
                    
                    # rotate value in entropyData bufferT
                        entropyData.rotate(-1)
                        entropyData[19]=entropyDistance
                    # update threshold 

                        th[i]=function.sigmoid(10,5,corr)
                        entropyThresh=function.EntropyThresholdUpdate(entropyData, entropyThreshNoise,0.9)

                        fl=buff.flag(corr,th[i],entropyDistance,entropyThresh,coeff,energy,np.array(c[i]))
                        if fl=="admit" :
                            self.__semaphore.acquire()
                            self.__condition.acquire()
                            MfccsCoeff,audioData=buff.get()
                            self.__condition.notify()
                            self.__condition.release()
                            self.__semaphore.release()
            
            c=[]
       # ndata=DSP.denormalize(pdata,0xFF)
            ndata=audio.depseudonymize(pdata)
            audio.write(ndata)


        print("out of loop")
        print("end of transmission -> wait")
        
    def SVM(self):
        global MfccsCoeff
        global audioData
        CoeffSphere=Sphere.Sphere_calibration();
        try :
            os.remove('../examples/build/%s' %self.__fifo_name)
        except :
            print "Pipe already removed"
        try :
            os.mkfifo('../examples/build/%s'  %self.__fifo_name)
        except OSError as e :
            if e.errno==errno.EEXIST :
                print("File already exist")
            else :
                raise
        self.__svm=AudioIO.LoadClassifier("SVM_Trained")
        self.__svmL=AudioIO.LoadClassifier("LeftSVM_Trained")
        self.__svmR=AudioIO.LoadClassifier("RightSVM_Trained")
        print tools.bcolors.OKGREEN + "In SVM Method - All done" + tools.bcolors.ENDC
		
        
        
        while True :
				self.__condition.acquire()
				self.__condition.wait()
				self.__semaphore.acquire()
				MfccsCoeffGet=MfccsCoeff
				self.__semaphore.release()
				self.__condition.release()
				newcoeff=(CoeffSphere.ClassAndFeaturesSplit(MfccsCoeffGet,"test")).T
				classLab=(MachineLearning.ClassifierWrapper(self.__svm,self.__svmL ,  self.__svmR,newcoeff))
				classL=int(MachineLearning.ClassifierWrapper(self.__svm,self.__svmL , self.__svmR,newcoeff)[0][0])
				print classLab
				self.write_Pipe(classL)
				
           # return classLab
           
    def write_Pipe(self,classL):
            with open(self.__fifo_name,'wb') as f:
                f.write('{}\n'.format(len(bin(classL)[2:])).encode())
                f.write(bin(classL)[2:])
                f.flush()
           # print "Done ..."


        
    def run(self):
         i=0
         self.__t1=threading.Thread(target=self.Recorder) 
         self.__t2=threading.Thread(target=self.SVM)
         self.__t1.start()
         self.__t2.start()
         self.__t1.join()
         self.__t2.join()
         
    def stop(self):
        self.__t1.join()
        self.__t2.join()
    
if __name__=='__main__' :
    print "Running ...."
    speech=Speech_Recognition()
    speech.run()
        