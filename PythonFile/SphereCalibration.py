import numpy as np
import numpy.matlib
import tools
import math
import struct
import array
import time
import cPickle
import tools
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from multiprocessing import Pool
from sklearn.cluster import KMeans
import os
import AudioIO
import MachineLearning
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import accuracy_score
from sklearn.preprocessing import Normalizer
import Sphere


__author__="Quentin MASCRET <quentin.mascret.1@ulaval.ca>"
__date__="2017-06-16"
__version__="1.0-dev"



def Angle2Coordinated(Theta,Phi):
				x=math.sin(math.radians(Phi))*math.cos(math.radians(Theta))
				y=math.sin(math.radians(Phi))*math.sin(math.radians(Theta))
				z=math.cos(math.radians(Phi))
				return [x,y,z]
				
class Sphere_Calibrator(object):
		def __init__(self,nWord,CandidateName):
				self.__accuracyResult=np.zeros(nWord)
				self.__nClusters=24
				self.__initCentroid=np.array([[0,0,0],
											  [1,0,0],
											  [0,1,0],
											  [-1,0,0],
											  [0,-1,0],
											  [0,0,1],
											  [0,0,-1],
											  Angle2Coordinated(45,45),
											  Angle2Coordinated(45,135),
											  Angle2Coordinated(45,-135),
											  Angle2Coordinated(45,-45),
											  Angle2Coordinated(135,45),
											  Angle2Coordinated(135,135),
											  Angle2Coordinated(135,-135),
											  Angle2Coordinated(135,-45)
											  ], np.float64)
				self.__rayon=1.0
				self.__center=np.array([0,0,0])
				self.__Candidate=CandidateName
				self.__best_score=np.zeros((12),'f')
				self.__last=0.0
				self.__best_score_init=np.zeros((12),'f')
				self.__T=0.0
				self.__P=0.0
				
				
		def SphereFeatures(self,features,RotationMatrix):
				self.__DataLength=int(features.size/features.shape[0])
				self.__prof=int((features.size/39))
				self.__feature=np.zeros((39,self.__DataLength*self.__prof))
				self.__feature=np.reshape(features,(39,self.__prof), order='F')
				
				# reshape all matrix in 3 vector -> MFCC, VMFCC, VVMFCC
				x=self.__feature[(0+np.arange(13)),:].reshape(self.__feature[(0+np.arange(13)),:].size, order='F')
				y=self.__feature[(13+np.arange(13)),:].reshape(self.__feature[(13+np.arange(13)),:].size, order='F')
				z=self.__feature[(26+np.arange(13)),:].reshape(self.__feature[(26+np.arange(13)),:].size, order='F')
	
				meanx=np.mean(x)
				meany=np.mean(y)
				meanz=np.mean(z) 
				stdx=np.std(x)
				stdy=np.std(y)
				stdz=np.std(z)
		
				# 0 mean and one std
				## allocation of memory
				new_x=np.zeros(self.__DataLength*self.__prof*13)
				new_y=np.zeros(self.__DataLength*self.__prof*13)
				new_z=np.zeros(self.__DataLength*self.__prof*13)
				## operation
				for i in range(0,x.size):
						if x[i] != 0:
								new_x[i]=((x[i]-meanx)/stdx)-self.__center[0]
								new_y[i]=((y[i]-meany)/stdy)-self.__center[1]
								new_z[i]=((z[i]-meanz)/stdz)-self.__center[2]
						else :
								new_x[i]=0
								new_y[i]=0
								new_z[i]=0
				
				# Paramter to create sphere with dataset
				## allocation of memory
				P_abs=np.zeros(self.__DataLength*self.__prof*13)
				## operation
				P=(np.vstack((new_x,new_y,new_z))).T
				for i in range(0, x.size):
						P_abs[i]=math.sqrt(new_x[i] **2 +new_y[i]**2 + new_z[i]**2)
				Q=np.zeros(P_abs.size)
				for i in range (0, P_abs.size):
						if P_abs[i] !=0 :
								Q[i]=(self.__rayon/P_abs[i]).T
						else :
								Q[i]=0
				# Sphere data
				## allocation of memory
				SphereData=np.zeros((self.__DataLength*self.__prof*13,3))
				for i in range(0,3):
						SphereData[:,i]=Q*P[:,i]
				for i in range (0,3):
						for j in range(0,self.__DataLength*self.__prof*13):
								if math.isnan(SphereData[j,i])==True :
										SphereData[j,i]=0
				SphereData=SphereData.dot(RotationMatrix)
				feat_struct=np.concatenate((SphereData[:,0].reshape(13,self.__prof*self.__DataLength,order='F'),SphereData[:,1].reshape(13,self.__prof*self.__DataLength,order='F'),SphereData[:,2].reshape(13,self.__prof*self.__DataLength,order='F')),axis=0)
				features=np.zeros((39*self.__prof,self.__DataLength))
				for i in range (0,self.__DataLength):
						intermediaire=feat_struct[:,(i*self.__prof+np.arange(self.__prof))]
						features[:,i]=intermediaire.reshape(intermediaire.size, order='F')
				#print tools.bcolors.OKGREEN + "In Sphere - Calibration has been done ..." + tools.bcolors.ENDC	
				return features
		
		def Sphere(self,features,Label,nClass, types):
				print tools.bcolors.OKBLUE + "Sphere Calibration" + tools.bcolors.ENDC
				tools.printProgressBar(0,3,prefix='Progress',suffix='Complete',decimals=1, length=50)
				self.__DataLength=int(features.size/features.shape[0])
				if features.shape[1]==1:
						self.__prof=int((features.size/39))
				else :
						self.__prof=int((features.size/features.shape[1])/39)
				# new features matrix 
				self.__feature=np.zeros((39,self.__DataLength*self.__prof))
				self.__Label=np.zeros((self.__DataLength*self.__prof*13))
				for i in range(0,self.__DataLength):
						idx=(i*self.__prof+np.arange(self.__prof))
						idx_2=(i*self.__prof*13+np.arange(self.__prof*13))
						self.__feature[:,idx]=np.reshape(features[:,i],(39,self.__prof), order='F')
						self.__Label[idx_2]=numpy.matlib.repmat(Label[i],1,self.__prof*13)		
				# reshape all matrix in 3 vector -> MFCC, VMFCC, VVMFCC
				x=self.__feature[(0+np.arange(13)),:].reshape(self.__feature[(0+np.arange(13)),:].size, order='F')
				y=self.__feature[(13+np.arange(13)),:].reshape(self.__feature[(13+np.arange(13)),:].size, order='F')
				z=self.__feature[(26+np.arange(13)),:].reshape(self.__feature[(26+np.arange(13)),:].size, order='F')	
				
				if (types == "train"):
		
				#mean and std of x, y and z
					meanx=np.mean(x)
					meany=np.mean(y)
					meanz=np.mean(z) 
					stdx=np.std(x)
					stdy=np.std(y)
					stdz=np.std(z)
					fo=open("Properties_file/Mean_and_Std","wb")
					cPickle.dump(meanx,fo,protocol=cPickle.HIGHEST_PROTOCOL)
					cPickle.dump(meany,fo,protocol=cPickle.HIGHEST_PROTOCOL)
					cPickle.dump(meanz,fo,protocol=cPickle.HIGHEST_PROTOCOL)
					cPickle.dump(stdx,fo,protocol=cPickle.HIGHEST_PROTOCOL)
					cPickle.dump(stdy,fo,protocol=cPickle.HIGHEST_PROTOCOL)
					cPickle.dump(stdz,fo,protocol=cPickle.HIGHEST_PROTOCOL)
					fo.close()
					
				elif (types == "test" ):	
					try :
						fl=open("Properties_file/Mean_and_Std","rb")
					except IOError :
						print tools.bcolors.FAIL + "In Sphere - unable to open Mean and Std file" + tools.bcolors.ENDC
						return
					try :
						meanx=cPickle.load(fl)
						meany=cPickle.load(fl)
						meanz=cPickle.load(fl)
						stdx=cPickle.load(fl)
						stdy=cPickle.load(fl)
						stdz=cPickle.load(fl)
					except :
						fl.close()
					fl.close()
				"""
				# mean and std of x, y and z
				meanx=np.mean(x)
				meany=np.mean(y)
				meanz=np.mean(z) 
				stdx=np.std(x)
				stdy=np.std(y)
				stdz=np.std(z)
				"""
				
				tools.printProgressBar(1,3,prefix='Progress',suffix='Complete',decimals=1, length=50)
				## allocation of memory
				new_x=np.zeros(self.__DataLength*self.__prof*13)
				new_y=np.zeros(self.__DataLength*self.__prof*13)
				new_z=np.zeros(self.__DataLength*self.__prof*13)
				P_abs=np.zeros(self.__DataLength*self.__prof*13)
				Q=np.zeros(x.size)
				## operation
				for i in range(0,x.size):
						if x[i] != 0:
								new_x[i]=((x[i]-meanx)/stdx)-self.__center[0]
								new_y[i]=((y[i]-meany)/stdy)-self.__center[1]
								new_z[i]=((z[i]-meanz)/stdz)-self.__center[2]
								P_abs[i]=math.sqrt(new_x[i] **2 +new_y[i]**2 + new_z[i]**2)
								Q[i]=(self.__rayon/P_abs[i]).T
						else :
								new_x[i]=0
								new_y[i]=0
								new_z[i]=0
								P_abs[i]=0
								Q[i]=0
				# Paramter to create sphere with dataset
				P=(np.vstack((new_x,new_y,new_z))).T
				tools.printProgressBar(2,3,prefix='Progress',suffix='Complete',decimals=1, length=50)
				# Sphere data
				## allocation of memory
				SphereData=np.zeros((self.__DataLength*self.__prof*13,3))
				for i in range(0,3):
						SphereData[:,i]=Q*P[:,i]
						for j in range(0,self.__DataLength*self.__prof*13):
								if math.isnan(SphereData[j,i])==True :
										SphereData[j,i]=0
			
				tools.printProgressBar(3,3,prefix='Progress',suffix='Complete',decimals=1, length=50)
				# KMEANS below
				mu=np.zeros((self.__nClusters,nClass*3),'f')
				Theta=np.zeros((nClass,self.__nClusters),'f')
				Phi=np.zeros((nClass,self.__nClusters),'f')
				print tools.bcolors.OKBLUE + "Kmeans calculation in progress" + tools.bcolors.ENDC
				tools.printProgressBar(0,nClass,prefix='Progress',suffix='Complete',decimals=1, length=50)
				for i in range(1,nClass+1):
					km=KMeans(n_clusters=self.__nClusters,n_jobs=20,init='k-means++',n_init=10).fit(SphereData[self.__Label.T==i,:])
					mu[:,(i-1)*3+np.arange(3)]=km.cluster_centers_
					Angle=self.SphericalAngle(km.cluster_centers_.T)
					Theta[i-1,:]=Angle[0,:]
					Phi[i-1,:]=Angle[1,:]
					tools.printProgressBar(i,nClass,prefix='Progress',suffix='Complete',decimals=1, length=50)
				params=np.zeros((2),'i')
				params[0]=self.__DataLength
				params[1]=self.__prof
				return Theta.T , Phi.T , SphereData,  params
				
		def SphericalAngle(self,mu):
				n=mu.shape[1]
				X=np.zeros((2,n))
				for i in range(n):
							X[:,i]=self.ReturnAngle(mu[0,i],mu[1,i],mu[2,i])
				return X
		
		def ReturnAngle(self,x,y,z):
				if y==0 and x==0 :
						Theta=0
				else :
						Theta=math.degrees(math.atan2(y,x))
				Phi=math.degrees(math.acos(z))
				X=[Theta,Phi]
				return X
					
		def ImportData(self,path):
				# go in DataBase folder 
				os.chdir(path)
				
				# Check folder in this directory
				listdirectory = os.listdir(".")
				# memory allocation
				Features=np.array([]).reshape(4680,0)
				ClassNormal=np.array([]).reshape(1,0)
				print tools.bcolors.OKBLUE +"Reading Data"+ tools.bcolors.ENDC
				# read loop
				i=0
				for filename in listdirectory :
						tools.printProgressBar(i,len(listdirectory),prefix='Progress',suffix='Complete',decimals=1, length=50)
						if AudioIO.FolderClassDictionnary2(filename)>0 : 
								os.chdir(filename)
								listoffile=os.listdir(".")
								
								for allfile in listoffile :
											data=(np.loadtxt(allfile)).reshape(4680,1)
											Features=np.hstack([Features,data])
								classNormal=np.matlib.repmat(AudioIO.FolderClassDictionnary2(filename),1,len(listoffile))
								ClassNormal=np.hstack([ClassNormal,classNormal])
								os.chdir('../')
						i+=1
						tools.printProgressBar(i,len(listdirectory),prefix='Progress',suffix='Complete',decimals=1, length=50)
				os.chdir('../')
				# return 
				np.savetxt('b.out', ClassNormal[0])
				return Features, ClassNormal[0],12
		
		def CalculateDiffAngle(self,AngleData, Angle):
				ThetaData=AngleData[0]
				PhiData=AngleData[1]
				Theta=Angle[0]
				Phi=Angle[1]
				# get size of matrix -> size of Phi and Theta must be the same
				row, column=ThetaData.shape
				PhiDiff=np.zeros((row,column),'f')
				ThetaDiff=np.zeros((row,column),'f')
				for i in range(row):
						for j in range (column):
								PhiDiff[i,j]=PhiData[i,j]-Phi[i,j]
								ThetaDiff[i,j]=ThetaData[i,j]-Theta[i,j]
				
				return ThetaDiff, PhiDiff
				
		def SphereData2FeaturesMAtrix(self,SphereData,params):
				feat_struct=np.concatenate((SphereData[:,0].reshape(13,params[1]*params[0],order='F'),SphereData[:,1].reshape(13,params[1]*params[0],order='F'),SphereData[:,2].reshape(13,params[1]*params[0],order='F')),axis=0)

				## allocation of memory
				features=np.zeros((39*params[1],params[0]))
				
				for i in range (0,params[0]):
						intermediaire=feat_struct[:,(i*params[1]+np.arange(params[1]))]
						features[:,i]=intermediaire.reshape(intermediaire.size, order='F')
				return features
				
		def Angle2RotationMatrix(self,Theta,Phi):
			
				#define new vector director
				u=np.zeros((3),'f')
				u[0]=-math.sin(math.radians(Theta))
				u[1]=math.cos(math.radians(Theta))
				
				# some angles
				cosPhi=math.cos(math.radians(Phi))
				sinPhi=math.sin(math.radians(Phi))
				
				# Phi Rotation
				RotationPhi=np.zeros((3,3),'f')
				RotationPhi[0,0]=(u[0]**2)*(1-cosPhi)+cosPhi#u[0]**2+(1-u[0]**2)*cosPhi
				RotationPhi[0,1]=u[0]*u[1]*(1-cosPhi)
				RotationPhi[0,2]=-u[1]*sinPhi
				RotationPhi[1,0]=u[0]*u[1]*(1-cosPhi)
				RotationPhi[1,1]=(u[1]**2)*(1-cosPhi)+cosPhi
				RotationPhi[1,2]=u[0]*sinPhi
				RotationPhi[2,0]=u[1]*sinPhi
				RotationPhi[2,1]=-u[0]*sinPhi
				RotationPhi[2,2]=cosPhi
				np.savetxt('matr.out',RotationPhi)
				# Theta Rotation
				RotationTheta=np.zeros((3,3),'f')
				RotationTheta[0,0]=u[1]
				RotationTheta[0,1]=-u[0]
				RotationTheta[0,2]=0
				RotationTheta[1,0]=-u[0]
				RotationTheta[1,1]=u[1]
				RotationTheta[1,2]=0
				RotationTheta[2,0]=0
				RotationTheta[2,1]=0
				RotationTheta[2,2]=1
				
				RotationMatrix=RotationTheta.dot(RotationPhi)
				np.savetxt('mat.out',RotationMatrix)
				return RotationMatrix
		
		def CompileSVMChecking(self,svm,svmL,svmR,features, Class):
				best_score=np.zeros((12),'f')
				test_score=np.zeros((12),'i')
				ligne,column=features.shape
				for i in range(column):
						classL=int( MachineLearning.ClassifierWrapper(svm, svmL, svmR,features[:,i].reshape(1,-1))[1][0])
						Label=AudioIO.InvertClass(classL)
						if Label == Class[i]:
								best_score[Class[i]-1]+=1
						test_score[Class[i]-1]+=1
				if np.sum(test_score)!=len(Class):
						print "Error"
				for i in range(12):
						best_score[i]=best_score[i]/test_score[i]
				return best_score
				
		def Score(self,best_score,T,P):
				new_in=0
				last=0
				local_score=1.0
				for l in range(12):
						local_score*=best_score[l]
				if local_score > self.__last:
						self.__best_score=best_score
						self.__T=T
						self.__P=P
						self.__last=local_score
						print tools.bcolors.OKBLUE + "Best score saved" + tools.bcolors.ENDC
				
	
				
		def main(self):
				svm=AudioIO.LoadClassifier("SVM_Trained")
				svmL=AudioIO.LoadClassifier("LeftSVM_Trained")
				svmR=AudioIO.LoadClassifier("RightSVM_Trained")
				DataSetPath="DataBase/"
				NewSetPath="Guillaume/"
				print tools.bcolors.OKGREEN + "DataBase Treatment" + tools.bcolors.ENDC
				F,C,n=self.ImportData(DataSetPath)
				ThetaData, PhiData, SphereData, params =self.Sphere(F,C,n,"train")
				print tools.bcolors.OKGREEN + "Treatment complete" + tools.bcolors.ENDC
				print tools.bcolors.OKGREEN + "New DataBase Treatment" + tools.bcolors.ENDC
				Fn,Cn,n=self.ImportData(NewSetPath)
				Theta, Phi, SphereDataN, paramsN =self.Sphere(Fn,Cn,n,"test")
				print tools.bcolors.OKGREEN + "Treatment complete" + tools.bcolors.ENDC
				
				## put here newdata
				AngleData=[ThetaData,PhiData]
				Angle=[Theta,Phi]
				T,P=self.CalculateDiffAngle(AngleData, Angle)
				featuresSphere=np.array([]).reshape(4680,0)
				# concatenate features using sphere for each one of them
				RotationMatrix=self.Angle2RotationMatrix(0,0)
				for l in range (Fn.shape[1]):
							featuresSphere=np.hstack([featuresSphere,self.SphereFeatures(Fn[:,l],RotationMatrix).reshape(4680,1)])
				best_score_init=self.CompileSVMChecking(svm,svmL,svmR,featuresSphere, Cn)				
				
				k=0
				tools.printProgressBar(k,(self.__nClusters-1)*12,prefix='Progress',suffix='Complete',decimals=1, length=50)
				# check last angle point (define  z rotation throughout matrix)
				for i in range (1,self.__nClusters):
						for j in range (12):#self.__nClusters):
								RotationMatrix=self.Angle2RotationMatrix(T[i,j],P[i,j])
								featuresSphere=np.array([]).reshape(4680,0)#np.array([Fn.shape[0],Fn.shape[1]],'f')
								#print featuresSphere.shape
								for l in range (Fn.shape[1]):
											featuresSphere=np.hstack([featuresSphere,self.SphereFeatures(Fn[:,l],RotationMatrix).reshape(4680,1)])
								#print featuresSphere.shape
								best_score=self.CompileSVMChecking(svm,svmL,svmR,featuresSphere, Cn)
								#self.ScoreInit(best_score_init)
								self.Score(best_score,T[i,j],P[i,j])
								print best_score_init ,'\n', tools.bcolors.OKGREEN + "Best :", self.__best_score ,'\n' , "Angle ", T[i,j],P[i,j], "" + tools.bcolors.ENDC
								
								k+=1
								tools.printProgressBar(k,(self.__nClusters-1)*12,prefix='Progress',suffix='Complete',decimals=1, length=50)
				print best_score_init,'\n', self.__best_score ,'\n',self.__P,'\n', self.__T
				"""
				self.__first_score=self.__best_score
				self.__best_score=np.zeros((12),'f')
				self.__last=0
				self.__best_score_init=np.zeros((12),'f')
				self.__last_init=0
				self.__T=0.0
				self.__P=0.0
				
				PP=np.arange(15.0,16.0,0.1)
				TT=np.arange(52.0,53.0,0.1)
				k=0
				tools.printProgressBar(k,len(TT)*len(PP),prefix='Progress',suffix='Complete',decimals=1, length=50)
				
				for i in range(len(TT)):
						for j in range(len(PP)):
								RotationMatrix=self.Angle2RotationMatrix(TT[i],PP[j])
								featuresSphere=np.array([]).reshape(4680,0)#np.array([Fn.shape[0],Fn.shape[1]],'f')
								#print featuresSphere.shape
								for l in range (Fn.shape[1]):
											featuresSphere=np.hstack([featuresSphere,self.SphereFeatures(Fn[:,l],RotationMatrix).reshape(4680,1)])
								#print featuresSphere.shape
								best_score=self.CompileSVMChecking(svm,svmL,svmR,featuresSphere, Cn)
						
								self.Score(best_score,TT[i],PP[j])
		
								print best_score ,'\n', tools.bcolors.OKGREEN + "Best :", self.__best_score,'\n' , "Angle ", TT[i],PP[j],"" + tools.bcolors.ENDC
								k+=1
								tools.printProgressBar(k,len(TT)*len(PP),prefix='Progress',suffix='Complete',decimals=1, length=50)
				print best_score_init,'\n', self.__best_score ,'\n',self.__first_score,'\n', self.__P, self.__T
				"""
if __name__=='__main__' :
		 Calibration=Sphere_Calibrator(7,"Guilaume")
		 Calibration.main()

