import os
import cPickle
import scipy.io as sio

os.chdir('/home/lucas/Desktop/')
dat=sio.loadmat('pca_3d_test.mat')
mat = {}
mat['data'] = []
mat['labels'] = []

for row in dat['unnamed']:
    if row[0]==0:
        break
    tmp=[]
    for col in range(0,3):
        tmp.append(row[col])
    mat['labels'].append(row[3])
    mat['data'].append(tmp)



print 'All done'
cPickle.dump(mat,open('feature_test.pkl','w'))