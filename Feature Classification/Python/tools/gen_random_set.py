import numpy as np
from cPickle import dump
from os import chdir

chdir('/home/lucas/Desktop')
rng = np.random.RandomState(0)
X = rng.rand(1280, 10)
y = rng.randint(0,4,1280)

data={}

data['data']=X
data['labels']=y

dump(data,open('random','w'))


