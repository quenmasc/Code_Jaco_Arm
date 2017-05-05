# all import here
import os
import numpy as np
from pydub import AudioSegment
from scipy.io.wavfile import read
import inspect 
import mfccbuffer
import MFCC
import DSP
import struct
import fnmatch
import MachineLearning
import wave
__author__="Quentin MASCRET <quentin.mascret.1 ulaval.ca>"
__date__="2017-05-03"
__version__="1.0-dev"



def ReadAudioFile(path):
    extension=os.path.splitext(path)[1]
    if extension=='.wav':
        audioFile=AudioSegment.from_wav(path)
        data=np.fromstring(audioFile._data, np.int16)
        fs=audioFile.frame_rate
        x=np.array(data)
        return fs, x
    else :
        print "Error in ReadAudioFile() : NO FILE TYPE"
        return (-1,-1)

def FindWavFileAndStoreData():
    i=0
    os.chdir('.')
    listdirectory = os.listdir("speech/")
    for filename in listdirectory :
        if filename.endswith(".wav"):
            i+=1
	    fs,data=ReadAudioFile(filename)
	    mfccs=WAV2MFCCs(data)
            classLabel=ClassAttribution(filename)
            struct=[classLabel,mfccs]
            FeaturesSaved(struct)
            print "Features of", filename ," have been saved" , "remaining" , len(listdirectory)-i
    print "all done"

def WAV2MFCCs(data,window_sample=200,window_shift=85):
     data=DSP.normalize(data,32768.0)
     buff=mfccbuffer.MFFCsRingBuffer()    
     mfcc=MFCC.MFCCs()
     nbFrame=(len(data)-window_sample)/window_shift
     print "len data  :" ,len(data)
     print "frame :", nbFrame
     for i in range(0,nbFrame):
	signal=np.array(data[(i*window_shift+np.arange(window_sample))])
	m=mfcc.MFCC2(signal)
	buff.extend(m)
     mfccs, fl=buff.get()
     return mfccs

def ClassAttribution(endsfile):
    if fnmatch.fnmatch(endsfile,'*BACKWARD.wav' ):
        return 'Class_1'
    elif fnmatch.fnmatch(endsfile,'*FORWARD.wav' ):
        return 'Class_2'
    elif fnmatch.fnmatch(endsfile,'*LEFT.wav' ):
        return 'Class_3'
    elif fnmatch.fnmatch(endsfile,'*RIGHT.wav' ):
        return 'Class_4'
    elif fnmatch.fnmatch(endsfile,'*GOTO.wav' ):
        return 'Class_5'
    elif fnmatch.fnmatch(endsfile,'*READY.wav' ):
        return 'Class_6'
    elif fnmatch.fnmatch(endsfile,'*MODE.wav' ):
        return 'Class_7'
    else :
        return 'Class_8'

def FeaturesSaved(struct):
    os.chdir('../')
    if not os.path.exists(struct[0]) :
        os.makedirs(struct[0])
        print "folder :" ,struct[0], "has been created"
    os.chdir(struct[0])
    if not os.path.isfile(ClassDictionnaryFile(struct[0])):
        file(ClassDictionnaryFile(struct[0]),'a').close()
        np.savetxt(ClassDictionnaryFile(struct[0]),struct[1])
    else :
        data=np.loadtxt(ClassDictionnaryFile(struct[0]))
        data=np.vstack([data,struct[1]])
        np.savetxt(ClassDictionnaryFile(struct[0]),data)
    os.chdir('../AudioData')

def ClassDictionnaryFile(className):
    return {
        'Class_1' :'Class_1.txt',
        'Class_2' :'Class_2.txt',
        'Class_3' :'Class_3.txt',
        'Class_4' :'Class_4.txt',
        'Class_5' :'Class_5.txt',
        'Class_6' :'Class_6.txt',
        'Class_7' :'Class_7.txt',
        'Class_8' :'Class_8.txt',
        }[className]

def FolderClassDictionnary(listOfDirs):
     return { 
        'Class_1' : 1,
        'Class_2' : 2,
        'Class_3' : 3,
        'Class_4' : 4,
        'Class_5' : 5,
        'Class_6' : 6,
        'Class_7' : 7,
        'Class_8' : 8,
        }.get(listOfDirs,0) # zero is default class
    
def ReadFeatureClass():
    listdirectory = os.listdir(".")
    Features=np.array([]).reshape(7800,0)
    ClassLabel=np.array([]).reshape(1,0)
    for filename in listdirectory :
        if FolderClassDictionnary(filename)>0:
            os.chdir(filename)
            data=(np.loadtxt(os.listdir(".")[0])).T
            classLabel=np.matlib.repmat(FolderClassDictionnary(filename),1,data.shape[1])
            os.chdir('../')
            Features=np.hstack([Features,data])
            ClassLabel=np.hstack([ClassLabel,classLabel])
    print "In AudioIO - ReadFeatureClass : all features have been read. size of features matrix :", Features.shape[0], Features.shape[1]
    return Features , ClassLabel

def test():
    features, classL=ReadFeatureClass()
    fT,fTT ,ct, ctt = MachineLearning.Splitfeatures(features.T,classL.T,0.9)
    print "class" , ct , "data" , fT.T
    print(ctt)
FindWavFileAndStoreData()
test()
