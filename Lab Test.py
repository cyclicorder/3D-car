import os
import random
import math


def readData(filename,targetFile):
    dataset=[]
    target = []
    
    with open(filename) as R:
        for line in R:
            line=line.split()
            for i in range(len(line)):
                line[i] = float(line[i].strip("',"))
            dataset.append(list(line[0:]))
    with open(targetFile) as R:
        for line in R:
            line = line.split()
            target.append(float(line[0])-1)
    return (dataset,target)

def matrixMulti(matA,matB):
    if len(matA[0])!=len(matB):
        print "Matrix Muktiplication Error"
        return 0
    ans = [[0 for col in range(len(matB[0]))] for row in range(len(matA))]
    for i in range(len(matA)):
        for j in range(len(matB[0])):
            for k in range(len(matA[0])):
                ans[i][j] = ans[i][j] + matA[i][k]*matB[k][j]
    return ans

def activationFunc(x):
    return 1.0/(1+math.exp(-x))

def dataSplit(dataset):
    trainnig = random.sample(dataset,int(len(dataset)*0.80))
    testing = random.sample(dataset,int(len(dataset)*0.20))
    return (trainnig,testing)

def weightInit():
    hLayer = 1
    neuron = 5 #raw_input("Enter no of neuron :")
    hLW1 = [[0 for col in range(neuron)] for row in range(len(dataset[0]))]
    oLW = [[0 for col in range(1)] for row in range(neuron)]
    for i in range(len(dataset[0])):
        for j in range(neuron):
            ran = random.uniform(0,1)
            hLW1[i][j] = ran
    for i in range(neuron):
        ran2 = random.uniform(0,1)
        oLW[i][0] = ran2
    #print oLW
    return (hLW1,oLW)

def forwardBias(x,hLW1,oLW):
    y = []
    y.append(x)
    #print len(y[0])
    hLayerZ = matrixMulti(y,hLW1)
    for i in range(len(hLayerZ[0])):
        hLayerZ[0][i] = activationFunc(hLayerZ[0][i])
    outZ = matrixMulti(hLayerZ,oLW)
    #print outZ[0][0]
    outZ[0][0] = activationFunc(outZ[0][0])
    return (hLayerZ,outZ)

def evaluation(x,hLW1,oLW):
    y = []
    y.append(x)
    #print len(y[0])
    hLayerZ = matrixMulti(y,hLW1)
    for i in range(len(hLayerZ[0])):
        hLayerZ[0][i] = activationFunc(hLayerZ[0][i])
    outZ = matrixMulti(hLayerZ,oLW)
    #print outZ[0][0]
    outZ[0][0] = activationFunc(outZ[0][0])
    
    return outZ[0][0]


def backPropagation(x,hLW1,oLW,hLayerZ,outZ,inp,outError):
    y = []
    y.append(x)
    outError += outZ[0][0]* (1-outZ[0][0]) * (target[inp]-outZ[0][0])
    for i in range(len(oLW)):
        for j in range(len(oLW[i])):
            oLW[i][j] = oLW[i][j] + outError*ita*hLayerZ[0][i]
    
    hLayerError = [0 for i in range(len(hLayerZ[0]))]
    for i in range(len(hLayerZ[0])):
        hLayerError[i] = hLayerZ[0][i]*(1-hLayerZ[0][i])*(outError*oLW[i][0])
    
    for i in range(len(hLW1)):
        for j in range(len(hLW1[i])):
            hLW1[i][j] = hLW1[i][j] + hLayerError[j]*ita*y[0][i]
            a = 9
    return (hLW1,oLW,hLayerZ,outZ,outError)

def appendingTarget(dataset,target):
    for i in range(len(dataset)):
        dataset[i].append(target[i])
        #print len(dataset[i])
    return (dataset)
    
def appendingTargetReverse(dataset2,dataset3):
    print len(dataset2),len(dataset3)
    target = []
    testTarget = []
    for i in range(len(dataset2)):
        target.append(dataset2[i][61])
    dataset2 = dataset2[:61]
    for i in range(len(dataset3)):
        testTarget.append(dataset3[i][61])
    dataset3 = dataset3[:61]
    return (dataset2,target,dataset3,testTarget)

ita = 1
outError = 0

dataset,target1 = readData("Dataset.txt","Target.txt")
#print len(dataset[0])

dataset = appendingTarget(dataset,target1)
#print len(dataset[0])
train,testing = dataSplit(dataset)
#print len(train[0]),len(testing[0])

train,target,testing,testTarget = appendingTargetReverse(train,testing)
# = appendingTargetReverse()

hLW1,oLW = weightInit()

for i in range(1):
    for input in range(len(train)):
        hLayerZ,outZ = forwardBias(train[input],hLW1,oLW)
        hLW1,oLW,hLayerZ,outZ,outError = backPropagation(train[input],hLW1,oLW,hLayerZ,outZ,input,outError)
        #print outZ,outError

correct = 0
incorrect = 0
#print len(testing),len(testTarget)
for i in range(len(testing)):
    out = evaluation(testing[i],hLW1,oLW)
    if(out<.5 and testTarget[i]==0) or (out>=.5 and testTarget[i]==1):
        correct += 1
    else:
        incorrect += 1
print "Correctly classified = ",correct,"incorrectly classified = ",incorrect
print "Accuracy = ", round(correct*100.0/len(testing),2)




















