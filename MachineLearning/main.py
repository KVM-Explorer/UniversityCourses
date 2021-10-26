import AdaBoost
import numpy as np
def loadSimpData():
    datMat = np.matrix([[1., 2.1],
                     [2., 1.1],
                     [1.3, 1.],
                     [1., 1.],
                     [2., 1.]])
    classLabels = [1.0, 1.0, -1.0, -1.0, 1.0]
    return datMat, classLabels

if __name__=="__main__" :
    p = AdaBoost.Adaboost()
    data,labels = loadSimpData()
    print(p.train(data,labels),p.weakClassifierArray)
    import Adaboost
    print(Adaboost.adaBoostTrainDS(data,labels,40))
