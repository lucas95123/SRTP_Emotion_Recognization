#    This file is part of DEAP.
#
#    DEAP is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as
#    published by the Free Software Foundation, either version 3 of
#    the License, or (at your option) any later version.
#
#    DEAP is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with DEAP. If not, see <http://www.gnu.org/licenses/>.


#    example which maximizes the sum of a list of integers
#    each of which can be 0 or 1
import warnings
warnings.filterwarnings("ignore")

import random
import copy
import sys
from deap import base
from deap import creator
from deap import tools
from os import chdir
from sklearn import svm
import scipy.io as sio
#you can change the two parameters below to modify the number of samples and features
global numofsamples
global numoffeatures
global numoftest
numgeneration=2000
numpopulation=1000
#read the features of signals
chdir('/Users/dingshilun/Documents/Junior/SRTP/srtp')

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

global emg
global testS

emg = loaddatafrommat('train_1_3_pca.mat', 'train')
testS = loaddatafrommat('test_1_3_pca.mat', 'test')#test samples

creator.create("FitnessMax", base.Fitness, weights=(1.0,))
creator.create("Individual", list, fitness=creator.FitnessMax)

toolbox = base.Toolbox()

# Attribute generator
#                      define 'attr_bool' to be an attribute ('gene')
#                      which corresponds to integers sampled uniformly
#                      from the range [0,1] (i.e. 0 or 1 with equal
#                      probability)
toolbox.register("attr_bool", random.randint, 0, 1)

# Structure initializers
#                         define 'individual' to be an individual
#                         consisting of 100 'attr_bool' elements ('genes')
toolbox.register("individual", tools.initRepeat, creator.Individual,
    toolbox.attr_bool, numoffeatures)

# define the population to be a list of individuals
toolbox.register("population", tools.initRepeat, list, toolbox.individual)

# the goal ('fitness') function to be maximized
def evalOneMax(individual):
    ans=0;
    feat=copy.deepcopy(emg)
    test=copy.deepcopy(testS)
    itemi=0
    for itemi in range(0,numofsamples):
        feat['data'][itemi]=[]
        for it in range(0,numoffeatures):
            if (individual[it]):
                #print(it)
                #print("labels",emg['labels'][it])
                #print(itemi)
                temp=emg['data'][itemi][it]
                #print(temp)
                feat['data'][itemi].append(temp)
        #feat['data'][itemi].reshape(-1,1);
        #feat['labels'].append(emg['labels'][itemi])
    #print(feat['data'][0])
    #print(emg['data'][0][0])
#    print(feat['data'])
    for itemi in range(0,numoftest):
        test['data'][itemi]=[]
        for it in range(0,numoffeatures):
            if (individual[it]):
                #print(it)
                #print("labels",emg['labels'][it])
                #print(itemi)
                temp=testS['data'][itemi][it]
                #print(temp)
                test['data'][itemi].append(temp)

    reg = svm.SVC(kernel='rbf')

    reg.fit(feat['data'], feat['labels'])
    count=0
    for i in range(0, numoftest):
        #print reg.predict(feat['data'][i])
        if (reg.predict(test['data'][i])==test['labels'][i]):
            count+=1

        #print(feat['labels'][1])
            #calculate the fitness to decide if the individual is suitable for the next generation
    return count,

#----------
# Operator registration
#----------
# register the goal / fitness function
toolbox.register("evaluate", evalOneMax)

# register the crossover operator
toolbox.register("mate", tools.cxTwoPoint)

# register a mutation operator with a probability to
# flip each attribute/gene of 0.05
toolbox.register("mutate", tools.mutFlipBit, indpb=0.05)

# operator for selecting individuals for breeding the next
# generation: each individual of the current generation
# is replaced by the 'fittest' (best) of three individuals
# drawn randomly from the current generation.
toolbox.register("select", tools.selTournament, tournsize=3)

#----------

def main(argv):
    global emg
    global testS
    emg = loaddatafrommat(argv[1], 'train')
    testS = loaddatafrommat(argv[2], 'test')  # test samples
    random.seed(64)

    # create an initial population of 300 individuals (where
    # each individual is a list of integers)
    pop = toolbox.population(n=numpopulation)

    # CXPB  is the probability with which two individuals
    #       are crossed
    #
    # MUTPB is the probability for mutating an individual
    #
    # NGEN  is the number of generations for which the
    #       evolution runs
    CXPB, MUTPB, NGEN = 0.5, 0.2, numgeneration

    print("Start of evolution")

    # Evaluate the entire population
    fitnesses = list(map(toolbox.evaluate, pop))
    for ind, fit in zip(pop, fitnesses):
        ind.fitness.values = fit

    print("  Evaluated %i individuals" % len(pop))

    # Begin the evolution
    for g in range(NGEN):
        print("-- Generation %i --" % g)

        # Select the next generation individuals
        offspring = toolbox.select(pop, len(pop))
        # Clone the selected individuals
        offspring = list(map(toolbox.clone, offspring))

        # Apply crossover and mutation on the offspring
        for child1, child2 in zip(offspring[::2], offspring[1::2]):

            # cross two individuals with probability CXPB
            if random.random() < CXPB:
                toolbox.mate(child1, child2)

                # fitness values of the children
                # must be recalculated later
                del child1.fitness.values
                del child2.fitness.values

        for mutant in offspring:

            # mutate an individual with probability MUTPB
            if random.random() < MUTPB:
                toolbox.mutate(mutant)
                del mutant.fitness.values

        # Evaluate the individuals with an invalid fitness
        invalid_ind = [ind for ind in offspring if not ind.fitness.valid]
        fitnesses = map(toolbox.evaluate, invalid_ind)
        for ind, fit in zip(invalid_ind, fitnesses):
            ind.fitness.values = fit

        print("  Evaluated %i individuals" % len(invalid_ind))

        # The population is entirely replaced by the offspring
        pop[:] = offspring

        # Gather all the fitnesses in one list and print the stats
        fits = [ind.fitness.values[0] for ind in pop]

        length = len(pop)
        mean = sum(fits) / length
        sum2 = sum(x*x for x in fits)
        std = abs(sum2 / length - mean**2)**0.5

        print("  Min %s" % min(fits))
        print("  Max %s" % max(fits))
        print("  Avg %s" % mean)
        print("  Std %s" % std)

    print("-- End of (successful) evolution --")

    best_ind = tools.selBest(pop, 1)[0]
    print("Best individual is %s, %s" % (best_ind, best_ind.fitness.values))

if __name__ == "__main__":
    main(sys.argv)
