from cPickle import load
from os import chdir

chdir('/home/lucas/Desktop/')
#gsr=load(open('random', 'rb'))
gsr=load(open('feature_test.pkl', 'rb'))
fout=open('SRTP_test','w')

for i in range(0,17):
    fout.write(str(gsr['labels'][i]))
    index=1

    for j in range(0,3):
        fout.write(' '+str(index)+':'+str(gsr['data'][i][j]))
        index+=1
    fout.write('\n')

fout.close()