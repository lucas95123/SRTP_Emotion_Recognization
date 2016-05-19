from cPickle import load
from os import chdir
from sklearn import svm

chdir('/home/lucas/Desktop/')
emg = load(open('feature_PCA.pkl', 'rb'))

reg = svm.SVC(kernel='rbf')

reg.fit(emg['data'], emg['labels'])
for i in range(0, 70):
    print reg.predict(emg['data'][i])