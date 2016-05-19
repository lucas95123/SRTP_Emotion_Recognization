from sklearn.decomposition import PCA, KernelPCA
from os import chdir
from cPickle import load,dump

chdir('/home/lucas/Desktop/')
gsr=load(open('feature.pkl', 'rb'))

pca=PCA(15,True)
gsr_pca={}
gsr_pca['data']=pca.fit_transform(gsr['data'])
gsr_pca['labels']=gsr['labels']

dump(gsr_pca,open('feature_PCA.pkl','w'))