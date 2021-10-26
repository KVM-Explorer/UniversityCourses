import adaboost
import classifier
import numpy as np


def loadSimpData():
    datMat = np.matrix([[1., 2.1],
                        [2., 1.1],
                        [1.3, 1.],
                        [1., 1.],
                        [2., 1.]])
    classLabels = [1.0, 1.0, -1.0, -1.0, 1.0]
    return datMat, classLabels


def loadMuitiData():
    data1 = np.matrix([
        [1.1, 0.9, 1.5],
        [0.8, 1.1, 1.7],
        [1.2, 0.5, 1.9],
        [1.0, 0.2, 1.7],
    ])
    data2 = np.matrix([
        [1.4, 0.5, 0.8],
        [1.6, 0.8, 1.0],
        [1.7, 0.3, 1.3],
        [1.8, 1.1, 0.7],
    ])
    data3 = np.matrix([
        [0.2,1.6,0.3],
        [0.4,1.8,1.2],
        [1.3,1.9,0.6],
        [1.0,1.5,0.8],
    ])
    labels = ['医疗', '民生', '法律']

    data = {labels[0]: data1, labels[1]: data2, labels[2]: data3}
    return data, labels


def adaboost_test():
    demo = adaboost.Adaboost()
    data, labels = loadSimpData()
    print(demo.train(data, labels))
    print(demo.weakClassifierArray)
    print(demo.predict(data))


def multiClassifier_test():
    data, labels = loadMuitiData()
    demo = classifier.MultiClassifier(labels)
    result = demo.train(data)
    print("Error Rate: ", result)


if __name__ == "__main__":
    adaboost_test()
    multiClassifier_test()
