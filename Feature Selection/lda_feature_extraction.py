import numpy as np
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.lda import LDA
import sys
from os import chdir
import scipy.io as sio
from cPickle import load
import matplotlib.pyplot as plt
from sklearn import svm
label1=5
label2=6

def loaddatafrommat(filename, matname):
    global numofsamples
    global numoffeatures
    global numoftest
    dat = sio.loadmat(filename)
    if(matname=='train'):
        numofsamples=dat[matname].shape[0]
        numoffeatures=dat[matname].shape[1]-1
    else:
        numoftest=dat[matname].shape[0]
    mat = {}
    mat['data'] = []
    mat['labels'] = []

    for row in dat[matname]:
        if row[0]==0:
            break
        tmp=[]
        for col in range(0,numoffeatures):
            tmp.append(row[col])
        mat['labels'].append(row[numoffeatures])
        mat['data'].append(tmp)
    return mat


chdir('/Users/dingshilun/Documents/Junior/SRTP/srtp')

emg = loaddatafrommat('train_'+str(label1)+'_'+str(label2)+'_pca.mat', 'train')
testS = loaddatafrommat('test_'+str(label1)+'_'+str(label2)+'_pca.mat', 'test')#test samples

X = np.array(emg['data'])
y = np.array(emg['labels'])
# clf = LDA()
# #lda = LinearDiscriminantAnalysis(n_components=2)
# LDA(n_components=2, priors=None, shrinkage=None, solver='svd',
#   store_covariance=False, tol=0.0001)
# X_r2 = clf.fit(X, y).transform(X)
# print(testS['labels'])
# print(clf.predict(testS['data']))
# print(X_r2)
reg = svm.SVC(kernel='rbf')
reg.fit(emg['data'], emg['labels'])
print(reg.predict(testS['data']))
print(testS['labels'])

def find(label, index):
    idx=[]
    count=0
    for row in label:
        if row==index:
            idx.append(count)
        count=count+1
    return idx
t=np.array(testS['data'])
plt.figure()
f2 = plt.figure(4)
idx_1 = find(y,label1)
p1 = plt.scatter(X[idx_1,1], X[idx_1,0], marker = 'x', color = 'm', label=str(label1), s = 30)
idx_2 = find(y,label2)
p2 = plt.scatter(X[idx_2,1], X[idx_2,0], marker = '+', color = 'c', label=str(label2), s = 30)
idx_3=find(testS['labels'],label1)
p3=plt.scatter(t[idx_3][1],t[idx_3][0],marker='x',color='m',label=str(label1),s=70)
idx_4=find(testS['labels'],label2)
p4=plt.scatter(t[idx_4][1],t[idx_4][0],marker='+',color='c',label=str(label2),s=70)
plt.legend(loc = 'upper right')
plt.title('LDA of IRIS dataset')

plt.show()
