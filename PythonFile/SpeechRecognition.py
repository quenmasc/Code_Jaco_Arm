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

__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-05-11"
__version__="1.0-dev"

class SpeechRecognition(object):
    def __init__(self):
        self.__mfcc=np.zeros(5850)
        self.__start_SVM=False

    def Recorder(self):
        audio= alsa_record.Record()
        mfcc = MFCC.MFCCs()
        entropy = spectral_entropy.SPECTRAL_ENTROPY()
        buff=mfccbuffer.MFFCsRingBuffer()
        RingLength=24650
        window_sample=200
        step_sample=85

   # store=RingBuffer.WaitingBuffer(10,window_sample)
        audio.run()
        print "running"
        cur=0

        tail=0

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
        mfc=np.empty((26,200),'f')
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
            audio.RingBufferWrite(ndata)
            if (c==[]) :
                c=audio.RingBufferRead()
            else :
                print ("Overwrite")
                break
            if flag < 3:
                flag+=1

            else :
                for i in range(0,2) :

                # return MFCC and spectral entropy
                    coeff,energy=mfcc.MFCC(np.array(c[i]))
                    SEntropy=entropy.SpectralEntropy(np.array(c[i]))
                    if j<20 :
                        mfccNoise+=np.array(coeff)
                        entropyData.append(SEntropy)
                        j+=1
                        if j==20 :
                            mfccNoise=mfccNoise/20
                            entropyNoise = np.mean(entropyData)
                            Data=entropyData
                            entropyData=deque([])
                            for k in range(0,len(Data)) :
                                entropyData.append(function.distance(Data[k],entropyNoise))
                            entropyThreshNoise =function.MeanStandardDeviation(entropyData,3)
                    else :
                    # return MFCC and Spectral Entropy background noise
                        mfccN=function.updateMFCCsNoise(np.array(coeff),mfccNoise, 0.90)
                        entropyN=function.updateEntropyNoise(SEntropy,entropyNoise, 0.95)
                    
                    # return correlation and distance of MFCC and Entropy
                        corr=function.correlation_1D(np.array(coeff),mfccN)
                        entropyDistance=function.distance(SEntropy,entropyN)
                    
                    # rotate value in entropyData bufferT
                        entropyData.rotate(-1)
                        entropyData[19]=entropyDistance
                    # update threshold 
                        th[i]=function.sigmoid(10,5,corr)
                        entropyThresh=function.EntropyThresholdUpdate(entropyData, entropyThreshNoise,0.96)
                    

                   # print(entropyThreshNoise)
                        if entropyDistance >=  entropyThreshNoise:
                            print "dist" , entropyDistance , "th" , entropyThresh

                     # flag
                        fl=buff.flag(corr,th[i],entropyDistance,entropyThresh,coeff,energy,np.array(c[i]))
                        if fl=="admit" :
                            mfc,audioData=buff.get()
                            np.savetxt('coeff.out',mfc)
                            print("  #############################")
            print "flag is : " , fl
            c=[]
       # ndata=DSP.denormalize(pdata,0xFF)
            ndata=audio.depseudonymize(pdata)
            audio.write(ndata)
#
        print("out of loop")
        print("end of transmission -> waiting new data")        
        

if __name__=='__main__' :
    print "Running ...."
    speech=SpeechRecognition()
    speech.Recorder()
    print "end of running"
