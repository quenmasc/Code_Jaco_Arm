# all import here
import numpy as np
import math
import struct
import array
import time
import DSP
import wave
import os
import sys
import sklearn.svm
import sklearn.decomposition
import sklearn.ensemble
from sklearn.svm import SVC
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import train_test_split
from sklearn.model_selection import StratifiedShuffleSplit
from sklearn.model_selection import StratifiedKFold
from sklearn.multiclass import OneVsRestClassifier, OutputCodeClassifier
from sklearn.metrics import accuracy_score
import tools
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import Normalize
from sklearn.metrics import confusion_matrix
import pprint as pp
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import RobustScaler
from sklearn.metrics import classification_report
__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-04-30"
__version__="1.0-dev"

class MidpointNormalize(Normalize):

    def __init__(self, vmin=None, vmax=None, midpoint=None, clip=False):
        self.midpoint = midpoint
        Normalize.__init__(self, vmin, vmax, clip)

    def __call__(self, value, clip=None):
        x, y = [self.vmin, self.midpoint, self.vmax], [0, 0.5, 1]
        return np.ma.masked_array(np.interp(value, x, y))
        
def ClassifierWrapper(classifier,classifierL,classifierR, Vector):
		R1=-1
		R2=-1
		R1=classifier.predict(Vector)
    #P1=classifier.predict_proba(Vector)
		if R1 == 1 :
			R2=classifierL.predict(Vector)
       # P2=classifierL.predict_proba(Vector)
		elif R1 == 2 :
			Vector =np.array(Vector)
			R2=classifierR.predict((Vector[0][(0+np.arange(3900))]).reshape(1,-1))
		else :
			R2=[8.]
		#P2=classifierR.predict_proba((Vector[0][(0+np.arange(3120))]).reshape(1,-1))
		return R1 , R2

def TrainBestParams(params,features,classLabel):
		svm=(SVC(probability=True, kernel="rbf",**params))
		svm.fit(features,classLabel)
		return svm

def NormalizeFeatures(features):
        # features matrix need to be a matrix with n rows coefficient per k colunm data
     print "normalization"

def Split_Features(features, classLabel):
	return Splitfeatures(features,classLabel,0.2)
	

def TrainSVM_RBF_Features(featuresTrain, featuresTest,classTrain,classTest):

    # splt features into class test and train
	#featuresTrain,featuresTest ,classTrain, ClassTest = Splitfeatures(features,classLabel,0.2)

    # C and Gamma range
	#C= np.logspace(-3,6,20) #20
	#Gamma=np.logspace(-9,3,20)#20
	scaler = StandardScaler()
	featuresTrain = scaler.fit_transform(featuresTrain)
	featuresTest = scaler.fit_transform(featuresTest)
	print featuresTrain.shape, featuresTest.shape , classTest.shape; classTrain.shape
	C_range =np.logspace(-1,3,30) #40
	gamma_range = np.logspace(-10,-3,30)
	params = dict(C=C_range, gamma=gamma_range)

	
    # cv
	cv = StratifiedShuffleSplit(n_splits=10, test_size=0.2, train_size=0.8,random_state=4)
	#cv.split(featuresTrain,classTrain) 
	
	print tools.bcolors.OKBLUE + "Running ..." + tools.bcolors.ENDC
	grid =GridSearchCV((SVC(kernel="rbf")),param_grid=params,cv=cv,verbose=20,n_jobs=10) # k=17 StratifiedKFold(classLabel,k=17)
	grid.fit(featuresTrain.T,classTrain)
	y_true, y_pred=classTest,grid.predict(featuresTest.T)
	print (grid.best_score_)
	print accuracy_score(classTest, y_pred)
	print("The best parameters are %s with a score of %0.2f"% (grid.best_params_, grid.best_score_))
	npv=classification_report(y_true, y_pred)
	print("Detailed classification report:")
	print("")
	print("The model is trained on the full development set.")
	print("The scores are computed on the full evaluation set.")
	print("")
	print npv
	print("")
	
	scores = [x[1] for x in grid.grid_scores_]
	scores=[entry.mean_validation_score for entry in grid.grid_scores_]
	#print scores
	scores = np.array(scores).reshape(len(C_range), len(gamma_range))
	#scores = grid.grid_scores_.mean_validation_score#['mean_validation_score']#.reshape(len(C),len(Gamma))
	
	plt.figure(figsize=(8, 6))
	plt.subplots_adjust(left=.2, right=0.95, bottom=0.15, top=0.95)
	plt.imshow(scores, interpolation='nearest', cmap=plt.cm.hot, norm=MidpointNormalize(vmin=0.2, midpoint=0.92))
	plt.xlabel('gamma')
	plt.ylabel('C')
	plt.colorbar()
	plt.xticks(np.arange(len(gamma_range)), gamma_range, rotation=45)
	plt.yticks(np.arange(len(C_range)), C_range)
	plt.title('Validation accuracy')
	plt.show()
	return grid
    
def Splitfeatures(features, classLabel, pourcentOf):
    featuresTrain, featuresTest, ClassTrain, ClassTest = train_test_split(features,classLabel,test_size=pourcentOf,random_state=45)
    print tools.bcolors.OKGREEN + "In MachineLearning - Splitfeatures : featuresTrain and featuresTest have been generated with suceed" +tools.bcolors.ENDC
    print featuresTrain.T.shape , ClassTrain.shape
    return featuresTrain.T, featuresTest.T, ClassTrain ,ClassTest


