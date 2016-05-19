from cPickle import load
from os import chdir
from sklearn.naive_bayes import GaussianNB
from sklearn.naive_bayes import MultinomialNB
from sklearn.naive_bayes import BernoulliNB

chdir('/home/lucas/Desktop/DEAP_GSR')
gsr=load(open('feature.pkl', 'rb'))

def getlabellist(matlabel):
    result = []
    for row in matlabel:
        result.append(getlabel(row))
    return result

def getlabel(row):
    if row[0] > 5 and row[1] > 5:
            return 1
    elif row[0] < 5 and row[1] > 5:
            return 2
    elif row[0] < 5 and row[1] < 5:
            return 3
    else:
            return 4

def getlabellist2d(matlabel):
    result = []
    for row in matlabel:
        result.append(getlabel2d(row))
    return result

def getlabel2d(row):
    if row[0] >= 5:
            return 1
    elif row[0] < 5:
            return 2

reg = GaussianNB()
# reg = BernoulliNB()
# reg = MultinomialNB()

reg.fit(gsr['data'][100:-5], getlabellist(gsr['labels'][100:-5]))

count = 0
correct = 0
for i in range(-5,100):
    print reg.predict(gsr['data'][i])
    print getlabel(gsr['labels'][i])
    if reg.predict(gsr['data'][i]) == getlabel(gsr['labels'][i]):
        correct+=1
    count+=1
print float(correct)/count
