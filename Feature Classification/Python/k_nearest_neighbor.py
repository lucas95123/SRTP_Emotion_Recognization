import scipy.io as sio
from os import chdir
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn import neighbors

chdir('/home/lucas/Desktop/')
n_neighbors = 15

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

label1=3
label2=6

emg = loaddatafrommat('train_'+str(label1)+'_'+str(label2)+'_pca.mat', 'train')
testS = loaddatafrommat('test_'+str(label1)+'_'+str(label2)+'_pca.mat', 'test')#test samples

X=np.array(emg['data'])
y=np.array(emg['labels'])

X=X[:, 1:3]

def find(label, index):
    idx=[]
    count=0
    for row in label:
        if index==row:
            idx.append(count)
        count=count+1
    return idx

f2 = plt.figure(2)
idx_1 = find(y,label1)
p1 = plt.scatter(X[idx_1,1], X[idx_1,0], marker = 'x', color = 'm', label=str(label1), s = 30)
idx_2 = find(y,label2)
p2 = plt.scatter(X[idx_2,1], X[idx_2,0], marker = '+', color = 'c', label=str(label2), s = 50)
plt.legend(loc = 'upper right')
plt.show()

