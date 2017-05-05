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
from sklearn.grid_search import GridSearchCV
from sklearn.cross_validation import train_test_split

__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-04-30"
__version__="1.0-dev"


def ClassifierWrapper(classifier, Classifiertype, Vector):
    R=-1
    P=-1
    R=classifier.predict(Vector)
    P=classifier.predict_proba(vector)
    return [R,P]

def SVM_RBF(featuresClass, Cparam):
    [features,Class]=listOfFeaturesClass(featuresClass)
    svm=sklearn.svm.SVC(C=Cparam,kernel='rbf',probability=True)
    svm.fit(features,Class)
    return svm


def NormalizeFeatures(features):
        # features matrix need to be a matrix with n rows coefficient per k colunm data
        
    # do mean and standard deviation
    if features.shape[1]==0 :
        print ("mtrix is empty")
    else :
        mean = np.mean(features,axis=1)
        std= np.mean(features,axis=1)
        
    # normalize
        featuresNormalize=features.copy()
        for coeff in range(features.shape[0]) :
            featuresNormalize[coeff,:]=(features[coeff,:]-mean)/std
    return (featuresNormalize, mean, std)

def TrainFeatures():

    [features, classLabel]=AudioIO.ReadFeatureClass()
    if len(features)==0 :
        print "In Classifier - Error : folders are empty"
        return
    

def Splitfeatures(features, classLabel, pourcentOf):
    featuresTrain, featuresTest, ClassTrain, ClassTest = train_test_split(features,classLabel,test_size=pourcentOf,random_state=45)
    print "In MachineLearning - Splitfeatures : featuresTrain and featuresTest have been generated with suceed"
    return featuresTrain, featuresTest, ClassTrain ,ClassTest
