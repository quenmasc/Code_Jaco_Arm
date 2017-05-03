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

__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-04-30"
__version__="1.0-dev"


def ClassifierWrapper(classifier, Classifiertype, Vector):
    R=-1
    P=-1
    R=classifier.predict(Vector)
    P=classifier.predict_proba(vector)
    return [R,P]

def SVM_RBF(featuresClass)
    [features,Class]=listOfFeaturesClass(featuresClass)
    svm=sklearn.svm.SVC(C=1.0,kernel='rbf',probability=True)
    svm.fit(features,Class)
    return svm


def NormalizeFeatures(features):
        # features matrix need to be a matrix with n rows coefficient per k colunm data
        
    # do mean and standard deviation
    if features.shape[1]==0 :
        print ("mtrix is empty")
        break
    else :
        mean = np.mean(features,axis=1)
        std= np.mean(features,axis=1)
        
    # normalize
        featuresNormalize=features.copy()
        for coeff in range(features.shape[0]) :
            featuresNormalize[coeff,:]=(features[coeff,:]-mean)/std
    return (featuresNormalize, mean, std)

    

    
