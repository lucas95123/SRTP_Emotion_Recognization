from os import chdir
from cPickle import load

chdir('/home/lucas/Desktop/')
dat=load(open('feature.pkl','rb'))
print 'jde'