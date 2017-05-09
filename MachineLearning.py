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
from sklearn.grid_search import GridSearchCV
from sklearn.cross_validation import train_test_split
from sklearn.cross_validation import StratifiedShuffleSplit
from sklearn.cross_validation import StratifiedKFold


__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-04-30"
__version__="1.0-dev"


def ClassifierWrapper(classifier, Vector):
    R=-1
    P=-1
    R=classifier.predict(Vector)
   # P=classifier.predict_proba(Vector)
    return R

def SVM_RBF(featuresClass, Cparam):
    [features,Class]=listOfFeaturesClass(featuresClass)
    svm=sklearn.svm.SVC(C=Cparam,kernel='rbf',probability=True)
    svm.fit(features,Class)
    return svm


def NormalizeFeatures(features):
        # features matrix need to be a matrix with n rows coefficient per k colunm data
     print "normalization"

def TrainSVM_RBF_Features(features,classLabel):

    # splt features into class test and train
   # featuresTrain,featuresTest ,classTrain, ClassTest = MachineLearning.Splitfeatures(features.T,classL.T,0.9)

    # C and Gamma range
    C= np.logspace(-6,3,20)
    Gamma=np.logspace(-9,3,20)

    # paramgrid
    param_grid=dict(gamma=Gamma, C=C)

    # cv
   # cv = StratifiedShuffleSplit(n_splits=50 , test_size=0.1, random_state=42)

    # grid
    print("Running ...")
    grid =GridSearchCV(SVC(probability=True),param_grid=param_grid,cv=StratifiedKFold(classLabel,k=17),verbose=50,n_jobs=3)
    grid.fit(features,classLabel)
    print("the best classifier is :" , grid.best_estimator_)
    return grid
    
def Splitfeatures(features, classLabel, pourcentOf):
    featuresTrain, featuresTest, ClassTrain, ClassTest = train_test_split(features,classLabel,test_size=pourcentOf,random_state=45)
    print "In MachineLearning - Splitfeatures : featuresTrain and featuresTest have been generated with suceed"
    return featuresTrain, featuresTest, ClassTrain ,ClassTest


