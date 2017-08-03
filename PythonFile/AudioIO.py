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
import wave
import MachineLearning
from sklearn.externals import joblib
import tools
import Sphere2
import Sphere
from scipy.stats import pearsonr
import time
import scipy.io


__author__="Quentin MASCRET <quentin.mascret.1 ulaval.ca>"
__date__="2017-05-03"
__version__="1.0-dev"



def ReadAudioFile(path):
    extension=os.path.splitext(path)[1]
    if extension=='.wav':
        f=open(path,'rb')
        data=f.read()
        data=np.fromstring(data, np.int16)
        x=np.array(data)
        return  x
    else :
        print "Error in ReadAudioFile() : NO FILE TYPE"

        return (-1,-1)

def FindWavFileAndStoreData():
    i=0

    os.chdir('mat/') #speech
    listdirectory = os.listdir(".")
    for filename in listdirectory :
        if filename.endswith(".mat"): #wav
				i+=1
				classLabel,data=ReadAudioFile2(filename)
				#mfccs=WAV2MFCCs(data)
				classLabel=ClassAttribution2(filename)
				struct=[classLabel,data]#mfccs
				FeaturesSaved2(struct)
				print tools.bcolors.HEADER +"Features of", filename ," have been saved"  + tools.bcolors.ENDC
				print "remaining" ,len(listdirectory)-i
    print tools.bcolors.OKGREEN +"all done" +tools.bcolors.ENDC

def ReadAudioFile2(path):
    extension=os.path.splitext(path)[1]
    if extension=='.mat': #wav
			mat = scipy.io.loadmat(path)
			gender=mat['local_gender'][0][0]
			classLabel=mat['local_classe'][0][0]
			features=mat['local_feature'][0]
			return  classLabel, features
    else :
			print "Error in ReadAudioFile() : NO FILE TYPE"

			return (-1,-1)




def WAV2MFCCs(data,window_sample=200,window_shift=85):
		data=DSP.normalize(data,32768.0)
		newData=DSP.NormalizeData(data)
		buff=mfccbuffer.MFFCsRingBuffer()    
		mfcc=MFCC.MFCCs()
		nbFrame=int((len(newData)-window_sample)/window_shift)
		for i in range(0,nbFrame):
			signal=np.array(newData[(i*window_shift+np.arange(window_sample))])
			m=mfcc.MFCC2(signal)
			buff.extend2(m)
		mfccs, fl=buff.get()
		return mfccs

def Data2MFCCs(data,window_sample=200,window_shift=85):
		#data=DSP.normalize(data,32768.0)
		newData=DSP.NormalizeData(data)
		buff=mfccbuffer.MFFCsRingBuffer()    
		mfcc=MFCC.MFCCs()
		nbFrame=int(len(data)-window_sample)/window_shift
		for i in range(0,nbFrame):
			signal=np.array(data[(i*window_shift+np.arange(window_sample))])
			m=mfcc.MFCC2(signal)
			buff.extend2(m)
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
        
def ClassAttribution2(endsfile):
    if fnmatch.fnmatch(endsfile,'*BACKWARD.mat' ):
        return 'Class_1'
    elif fnmatch.fnmatch(endsfile,'*FORWARD.mat' ):
        return 'Class_2'
    elif fnmatch.fnmatch(endsfile,'*LEFT.mat' ):
        return 'Class_3'
    elif fnmatch.fnmatch(endsfile,'*RIGHT.mat' ):
        return 'Class_4'
    elif fnmatch.fnmatch(endsfile,'*GOTO.mat' ):
        return 'Class_5'
    elif fnmatch.fnmatch(endsfile,'*READY.mat' ):
        return 'Class_6'
    elif fnmatch.fnmatch(endsfile,'*MODE.mat' ):
        return 'Class_7'
    else :
        return 'Class_8'

def FeaturesSaved(struct):
    os.chdir('../')
    if not os.path.exists(struct[0]) :
        os.makedirs(struct[0])
        print tools.bcolors.OKBLUE +"folder :" ,struct[0], "has been created" + tools.bcolors.ENDC
    os.chdir(struct[0])
    if not os.path.isfile(ClassDictionnaryFile(struct[0])):
        file(ClassDictionnaryFile(struct[0]),'a').close()
        np.savetxt(ClassDictionnaryFile(struct[0]),struct[1])
    else :

        data=np.loadtxt(ClassDictionnaryFile(struct[0]))
        data=np.vstack([data,struct[1]])
        np.savetxt(ClassDictionnaryFile(struct[0]),data)
    os.chdir('../speech') #speech
    
def FeaturesSaved2(struct):
    os.chdir('../')
    if not os.path.exists(struct[0]) :
        os.makedirs(struct[0])
        print tools.bcolors.OKBLUE +"folder :" ,struct[0], "has been created" + tools.bcolors.ENDC
    os.chdir(struct[0])
    if not os.path.isfile(ClassDictionnaryFile(struct[0])):
        file(ClassDictionnaryFile(struct[0]),'a').close()
        np.savetxt(ClassDictionnaryFile(struct[0]),struct[1])
    else :

        data=np.loadtxt(ClassDictionnaryFile(struct[0]))
        data=np.vstack([data,struct[1]])
        np.savetxt(ClassDictionnaryFile(struct[0]),data)
    os.chdir('../mat') #speech

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
        'Class_5' : 0,
        'Class_6' : 6,
        'Class_7' : 0,
        'Class_8' : 8,
        'Class_9' : 0,
        'Class_10' : 0,
        'Class_11' : 0,
        'Class_12': 12,
        'Class_13': 13,
        'Class_14': 14,
        'Class_15': 15,
        'Class_16':16,
        'Class_17':17,
        'Class_18':18,
        }.get(listOfDirs,0) # zero is default class

def FolderClassDictionnary2(listOfDirs):
     return { 
        'Class_1' : 1,
        'Class_2' : 2,
        'Class_3' : 3,
        'Class_4' : 4,
        'Class_6' : 5,
        'Class_12': 6,
        'Class_13': 7,
        'Class_14': 8,
        'Class_15': 9,
        'Class_16':10,
        'Class_17':11,
        'Class_18':12,
        }.get(listOfDirs,0) # zero is default class

def InvertClass(Label):
		return { 
			1 :1,
			2 : 2,
			3 : 3,
			4 : 4,
			6 : 5,
			12 : 6,
			13 : 7,
			14 : 8,
			15 : 9,
			16 : 10,
			17 : 11,
			18 : 12,
        }.get(Label,0)
        
def ClassName(Label):
     return { 
         1 : "BACKWARD",
         2 : "FORWARD",
         3 : "LEFT",
         4 : "RIGHT" ,
         5 : "GOTO",
         6 : "READY",
         7 : "MODE",
         8 : "NOISE" ,
         9 : "ONE",
         10: "TWO",
         11: "THREE",
         12: "UP",
         13: "DOWN",
         14: "OPEN",
         15: "CLOSE",
         16: "TRANSLATION",
         17: "ROTATION",
         18: "STOP",
        }.get(Label,0)

def TrainClasse(listOfDirs):
     return { 
         1 : "Class_1",
         2 : "Class_2",
         3 : "Class_3",
         4 : "Class_4" ,
         5 : "Class_5",
         6 : "Class_6",
         7 : "Class_7",
         8 : "Class_8" ,
         9 : "Class_9",
         10: "Class_10",
         11: "Class_11",
         12: "Class_12",
         13: "Class_13",
         14: "Class_14",
         15: "Class_15",
         16: "Class_16",
         17: "Class_17",
         18: "Class_18",
        }.get(listOfDirs,0)
    
def FirstSVMClass(listOfDirs):
     return { 
        'Class_1' : 1,
        'Class_2' : 1,
        'Class_3' : 2,
        'Class_4' : 2,
        'Class_5' : 0,
        'Class_6' : 1,
        'Class_7' : 2,
        'Class_8' : 3,
        'Class_9' : 0,
        'Class_10' :0,
        'Class_11' :0,
        'Class_12' :2,
        'Class_13' :2,
        'Class_14' :1,
        'Class_15' :2,
        'Class_16' :1,
        'Class_17' :1,
        'Class_18' :2,
        
        }.get(listOfDirs,0) # zero is default class

def ReadFeatureClass(path):
		os.chdir("DataBase/%s/"%path)
		listdirectory = os.listdir(".")
		Normalization=Sphere2.Sphere_calibration()
		Features=np.array([]).reshape(4680,0)
		FeaturesLeft=np.array([]).reshape(4680,0)
		FeaturesRight=np.array([]).reshape(3900,0)
		FeaturesCenter=np.array([]).reshape(3900,0)
		ClassNormal=np.array([]).reshape(1,0)
		ClassLeft=np.array([]).reshape(1,0)
		ClassRight=np.array([]).reshape(1,0)
		ClassCenter=np.array([]).reshape(1,0)
		ClassFirstLevel=np.array([]).reshape(1,0)
		for filename in listdirectory :
			if FolderClassDictionnary(filename)>0 : #and (FolderClassDictionnary(filename)!=9 and FolderClassDictionnary(filename)!=10 and FolderClassDictionnary(filename)!=11):
				os.chdir(filename)
				print filename , "folder"
				listoffile=os.listdir(".")
				for allfile in listoffile :
						data=(np.loadtxt(allfile)).reshape(4680,1)#.T #os.listdir(".")[0]
					#classFistLevel=np.matlib.repmat(FistSVMClass(filename),1,data.shape[1])
					#classFistLevel=FirstSVMClass(filename)
					#classNormal=np.matlib.repmat(FolderClassDictionnary(filename),1,data.shape[1])s
						Features=np.hstack([Features,data])
				classFirstLevel=np.matlib.repmat(FirstSVMClass(filename),1,len(listoffile))
				classNormal=np.matlib.repmat(FolderClassDictionnary(filename),1,len(listoffile))
				ClassFirstLevel=np.hstack([ClassFirstLevel,classFirstLevel])
				ClassNormal=np.hstack([ClassNormal,classNormal])
				if (FirstSVMClass(filename)==1):
							classLeft=np.matlib.repmat(FolderClassDictionnary(filename),1,len(listoffile))	
							ClassLeft=np.hstack([ClassLeft,classLeft])
				if (FirstSVMClass(filename)==2):
							classRight=np.matlib.repmat(FolderClassDictionnary(filename),1,len(listoffile))
							ClassRight=np.hstack([ClassRight,classRight])
			#	if (FirstSVMClass(filename)==3):
			#				classCenter=np.matlib.repmat(FolderClassDictionnary(filename),1,len(listoffile))
			#				ClassCenter=np.hstack([ClassCenter,classCenter])
				
				os.chdir('../')
			"""	
			if FolderClassDictionnary(filename)>0 and (FolderClassDictionnary(filename)==9 or FolderClassDictionnary(filename)==10 or FolderClassDictionnary(filename)==11):
				os.chdir(filename)
				listoffile=os.listdir(".")
				for allfile in listoffile :
						data=(np.loadtxt(allfile)).reshape(4680,1) #classNormal=np.matlib.repmat(FolderClassDictionnary(filename),1,data.shape[1])s
						FeaturesCenter=np.hstack([FeaturesCenter,data[(0+np.arange(3900))]])
				classCenter=np.matlib.repmat(FolderClassDictionnary(filename),1,len(listoffile))
				ClassCenter=np.hstack([ClassCenter,classCenter])
				os.chdir('../')
			"""
		Features=Normalization.ClassAndFeaturesSplit(Features,"train")
		
		
		#print FeaturesCenter.shape
		#FeaturesCenter=Normalization.ClassAndFeaturesSplit(FeaturesCenter,"train")
		for i in range (ClassFirstLevel.size):
				if (ClassFirstLevel[0][i]==1):
							#classLeft=np.matlib.repmat(FolderClassDictionnary(filename),1,data.shape[1])	
							#ClassLeft=np.hstack([ClassLeft,classLeft])
						FeaturesLeft=np.hstack([FeaturesLeft,Features[:,i].reshape(4680,1)])#data
				if (ClassFirstLevel[0][i]==2):
							#classRight=FolderClassDictionnary(filename)#np.matlib.repmat(FolderClassDictionnary(filename),1,data.shape[1])
							#ClassRight=np.hstack([ClassRight,classRight])
						FeaturesRight=np.hstack([FeaturesRight,Features[(0+np.arange(3900)),i].reshape(3900,1)])#data[(0+np.arange(3900))]]
				#if (ClassFirstLevel[0][i]==3):
							#classLeft=np.matlib.repmat(FolderClassDictionnary(filename),1,data.shape[1])	
							#ClassLeft=np.hstack([ClassLeft,classLeft])
				#		FeaturesCenter=np.hstack([FeaturesCenter,Features[(0+np.arange(3900)),i].reshape(3900,1)])#data
			
		
		print tools.bcolors.OKGREEN + "In AudioIO - ReadFeatureClass : all features have been read." +tools.bcolors.ENDC
		os.chdir('../../')
		return Features, FeaturesLeft, FeaturesRight, FeaturesCenter , ClassLeft[0],ClassRight[0], ClassCenter[0],ClassFirstLevel[0],ClassNormal[0]#Normalization.ClassAndFeaturesSplit(Features,"train"),Normalization.ClassAndFeaturesSplit(FeaturesLeft,"train"), Normalization.ClassAndFeaturesSplit(FeaturesRight,"train") , Normalization.ClassAndFeaturesSplit(FeaturesCenter, "train"), ClassLeft[0],ClassRight[0],ClassFirstLevel[0],ClassNormal[0]


def SaveClassifier(modelName,svmClassifier):
    joblib.dump(svmClassifier,"SVModel/%s.pkl"%modelName)
    print tools.bcolors.OKGREEN + "In AudioIO - SaveClassifier : Model has been saved correctly" + tools.bcolors.ENDC
    
def LoadClassifier(SVMModelName):
    try : 
        model=joblib.load("SVModel/%s.pkl"%SVMModelName)
        print tools.bcolors.OKGREEN + "Classifier loaded %s"%SVMModelName + tools.bcolors.ENDC
        return model
    except IOError :
        print tools.bcolors.FAIL +"Unable to load the Classifier %s"%SVMModelName+tools.bcolors.ENDC
        return

    
def test():
    [features, featuresL, featuresR, featuresC, classL, classR, classC,ClassFistLevel, CN]=ReadFeatureClass('mine')
   # [features_Rox, featuresL_Rox, featuresR_Rox, featuresC_Rox, classL_Rox, classR_Rox, classC_Rox,ClassFistLevel_Rox, CN_Rox]=ReadFeatureClass('Roxane')
    #[features_Gab, featuresL_Gab, featuresR_Gab, featuresC_Gab, classL_Gab, classR_Gab, classC_Gab,ClassFistLevel_Gab, CN_Gab]=ReadFeatureClass('Gabriel')
    if len(features)==0 :#or len(features_Rox)==0 :
        print tools.bcolors.FAIL+ "In Classifier - Error : folders are empty"+tools.bcolors.ENDC
        return
    print tools.bcolors.HEADER +"Size of features :" , featuresL.shape , len(classL), featuresR.shape , len(classR),features.shape , len(ClassFistLevel) , "Features, FeaturesL , FeaturesR"  , len(classC), featuresC.shape, "FeaturesC"+ tools.bcolors.ENDC
    #split test/train features
    
    featuresTr,featuresTe ,classTr, classTe =MachineLearning.Split_Features(features.T,CN)
   # featuresTr_Rox,featuresTe_Rox ,classTr_Rox, classTe_Rox =MachineLearning.Split_Features(features_Rox.T,ClassFistLevel_Rox)
   # featuresTr_Gab,featuresTe_Gab ,classTr_Gab, classTe_Gab =MachineLearning.Split_Features(features_Gab.T,ClassFistLevel_Gab)
    featuresTrain=featuresTr#np.concatenate([featuresTr,featuresTr_Rox ], axis=1)
    featuresTest=featuresTe#np.concatenate([featuresTe,featuresTe_Rox ],axis=1)
    classTrain=classTr#np.concatenate([classTr,classTr_Rox  ])
    classTest=classTe#np.concatenate([classTe,classTe_Rox ])
    FistSvmModel=MachineLearning.TrainSVM_RBF_Features(featuresTrain, featuresTest, classTrain, classTest)#ClassFistLevel
    SaveClassifier("SVM",FistSvmModel)
    print tools.bcolors.HEADER + "First SVM saved" + tools.bcolors.ENDC
    
    # features L
    
    """
    featuresTr,featuresTe ,classTr, classTe =MachineLearning.Split_Features(featuresL.T,classL)
    #featuresTr_Rox,featuresTe_Rox ,classTr_Rox, classTe_Rox =MachineLearning.Split_Features(featuresL_Rox.T,classL_Rox) 
   # featuresTr_Gab,featuresTe_Gab ,classTr_Gab, classTe_Gab =MachineLearning.Split_Features(featuresL_Gab.T,classL_Gab)
    featuresTrain=featuresTr#np.concatenate([featuresTr,featuresTr_Rox  ], axis=1)
    featuresTest=featuresTe#np.concatenate([featuresTe,featuresTe_Rox  ],axis=1)
    classTrain=classTr#np.concatenate([classTr,classTr_Rox  ])
    classTest=classTe#np.concatenate([classTe,classTe_Rox ])
    SecondSvmModel=MachineLearning.TrainSVM_RBF_Features(featuresTrain, featuresTest, classTrain, classTest)#ClassFistLevel
    SaveClassifier("LeftSVM",SecondSvmModel)
    print tools.bcolors.HEADER + "Second SVM saved" + tools.bcolors.ENDC
    
    # features R
    
    featuresTr,featuresTe ,classTr, classTe =MachineLearning.Split_Features(featuresR.T,classR)
    #featuresTr_Rox,featuresTe_Rox ,classTr_Rox, classTe_Rox =MachineLearning.Split_Features(featuresR_Rox.T,classR_Rox)
	#featuresTr_Gab,featuresTe_Gab ,classTr_Gab, classTe_Gab =MachineLearning.Split_Features(featuresR_Gab.T,classR_Gab)
    featuresTrain=featuresTr#np.concatenate([featuresTr,featuresTr_Rox  ], axis=1)
    featuresTest=featuresTe#np.concatenate([featuresTe,featuresTe_Rox  ],axis=1)
    classTrain=classTr#np.concatenate([classTr,classTr_Rox  ])
    classTest=classTe#np.concatenate([classTe,classTe_Rox  ])
    ThirdSvmModel=MachineLearning.TrainSVM_RBF_Features(featuresTrain, featuresTest, classTrain, classTest)#ClassFistLevel
    SaveClassifier("RightSVM",ThirdSvmModel)
    print tools.bcolors.HEADER + "Third SVM saved" + tools.bcolors.ENDC
    
    
    FourthSvmModel=MachineLearning.TrainSVM_RBF_Features(features.T, CN)
    SaveClassifier("NumberSVM",FourthSvmModel)
    print tools.bcolors.HEADER + "Fourth SVM saved" + tools.bcolors.ENDC
    """
    print tools.bcolors.OKGREEN + "All classifiers have been saved" +tools.bcolors.ENDC
    
def train():
		[features_m, featuresL_m, featuresR_m, featuresC_m, classL_m, classR_m, classC,ClassFistLevel_m, CN]=ReadFeatureClass('mine')
		#[features_Rox, featuresL_Rox, featuresR_Rox, featuresC_Rox, classL_Rox, classR_Rox, classC_Rox,ClassFistLevel_Rox, CN_Rox]=ReadFeatureClass('Roxane')
		#[features_Gab, featuresL_Gab, featuresR_Gab, featuresC_Gab, classL_Gab, classR_Gab, classC_Gab,ClassFistLevel_Gab, CN_Gab]=ReadFeatureClass('Gabriel')
		
		if len(features_m)==0 :
				print tools.bcolors.FAIL + "In Classifier - Error : folders are empty"+tools.bcolors.ENDC
				return
		model=LoadClassifier("SVM")
		#modelL=LoadClassifier("LeftSVM")
		#modelR=LoadClassifier("RightSVM")
		#modelC=LoadClassifier("NumberSVM")
		params=dict(gamma=model.best_params_["gamma"], C=model.best_params_["C"])
		ClassFistLevel=ClassFistLevel_m#np.concatenate([ClassFistLevel_m,ClassFistLevel_Rox ])
		features=features_m#np.concatenate([features_m,features_Rox ], axis=1)
		SVM=MachineLearning.TrainBestParams(params,features.T,ClassFistLevel)#ClassFistLevel
		print tools.bcolors.HEADER + "First SVM Trained" + tools.bcolors.ENDC
		"""
		featuresL=featuresL_m#np.concatenate([featuresL_m,featuresL_Rox ], axis=1)
		paramsL=dict(gamma=modelL.best_params_["gamma"], C=modelL.best_params_["C"])
		classL=classL_m#np.concatenate([classL_m,classL_Rox ])
		
		SVML=MachineLearning.TrainBestParams(paramsL,featuresL.T,classL)
		print tools.bcolors.HEADER + "Left SVM Trained" + tools.bcolors.ENDC
		paramsR=dict(gamma=modelR.best_params_["gamma"], C=modelR.best_params_["C"])
		featuresR=featuresR_m#np.concatenate([featuresR_m,featuresR_Rox  ], axis=1)
		classR=classR_m#np.concatenate([classR_m,classR_Rox ])
		SVMR=MachineLearning.TrainBestParams(paramsR,featuresR.T,classR)
		print tools.bcolors.HEADER + "Right SVM Trained" + tools.bcolors.ENDC
		#paramsC=dict(gamma=modelC.best_params_["gamma"], C=modelR.best_params_["C"])
		
		#SVMC=MachineLearning.TrainBestParams(paramsC,features.T,CN)
		#print tools.bcolors.HEADER + "Center SVM Trained" + tools.bcolors.ENDC
	
		print tools.bcolors.OKGREEN + "All SVM Trained correctly -> next step save them" + tools.bcolors.ENDC
		SaveClassifier("SVM_Trained",SVM)
		print tools.bcolors.HEADER + " SVM Trained saved" + tools.bcolors.ENDC
		SaveClassifier("LeftSVM_Trained",SVML)
		print tools.bcolors.HEADER + " Left SVM Trained saved" + tools.bcolors.ENDC
		SaveClassifier("RightSVM_Trained",SVMR)
		print tools.bcolors.HEADER + " Right SVM Trained saved" + tools.bcolors.ENDC
		#SaveClassifier("NumberSVM_Trained",SVMC)
		#print tools.bcolors.HEADER + "Center SVM Trained saved" + tools.bcolors.ENDC
		"""
		#print tools.bcolors.OKGREEN + "All SVM Trained have been saved" + tools.bcolors.ENDC
 
 
def correl ():
	Normalization=Sphere2.Sphere_calibration()
	listdirectory = os.listdir(".")
	data=Normalization.ClassAndFeaturesSplit(np.loadtxt("coeff.out"),"test")
	for filename in listdirectory:
			if(FolderClassDictionnary(filename)==7):
				os.chdir(filename)
				data_TEST=Normalization.ClassAndFeaturesSplit((np.loadtxt(os.listdir(".")[0])).T,"train")
				os.chdir('../')
				mn=np.array([])#.reshape(5850,0)
				m=np.zeros((5850,data_TEST.shape[1]))
				for j in range(0,data_TEST.shape[1]):
					
					for i in range(0, data.size):
						m[i,j]=(data[i]-data_TEST[i,j]).T
				print m.shape ,"\n mean:", np.mean(m,axis=1), "\n std :" , np.std(m,axis=1)
				np.savetxt('Param.out',(np.mean(m,axis=1), np.std(m, axis=1)))
				#print "Class :" ,FolderClassDictionnary(filename) ,"\n", "moy : ", mn 
				
					#print(pearsonr(data,data_TEST[:,j]))
					#time.sleep(2)
            
            
if __name__=='__main__' :
   #FindWavFileAndStoreData()

 #correl()
	test()
	train()
   #model=LoadClassifier("SVM_Trained")
   #modelL=0 #LoadClassifier("LeftSVM_Trained")
  #modelR=0 #LoadClassifier("RightSVM_Trained")
 # print(model)
 #  features=np.loadtxt('coeff.out')
 # print len(features)
#   R1 , R2 , P1 , P2=MachineLearning.ClassifierWrapper(model, modelL, modelR,features)
 #  print R1, R2

   