import minpy.numpy

import adaboost
import classifier
import numpy as np
import random
import os


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
        [0.2, 1.6, 0.3],
        [0.4, 1.8, 1.2],
        [1.3, 1.9, 0.6],
        [1.0, 1.5, 0.8],
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


def loadFileData(path, partition=0.8):
    """
    加载当前文件夹下的所有txt数据集，每个txt名称认为是类别，内容为对应数据
    :param partition: 训练集和测试集划分参数
    :param path: 文件夹地址
    :return: 训练集和验证集的数据存储的字典和标签列表
    """
    print("====================Loading Data====================")
    labels = []
    train = {}
    valid = {}
    names = os.listdir(path)
    print()
    for name in names:
        filename = os.path.join(path, name)
        # 生成标签列表
        label = name.strip(".txt")
        labels.append(label)
        # 生成训练集和校验集
        train[label] = []
        valid[label] = []
        with open(filename, "r") as file:
            origin_data = file.readlines()
        for sample in origin_data:
            seed = random.random()
            vector = list(map(float, sample.split(" ")))
            if (seed < partition):
                train[label].append(vector)
            else:
                valid[label].append(vector)
        print("Data: ",len(origin_data)," Train: ",numpy.shape(train[label])," Valid: ",numpy.shape(valid[label]))
    print("====================End Loading====================")
    return train, valid, labels


if __name__ == "__main__":
    # adaboost_test()
    # multiClassifier_test()
    train_data_filename = "/home/lightning/Downloads/qq-files/1158423912/file_recv/tz/dataset"
    train_data, valid_data, labels = loadFileData(train_data_filename,0.8)
    demo = classifier.MultiClassifier(labels,80,100)
    result1 = demo.train(train_data)
    print("训练集上错误率:" ,result1)
    num = 0
    error =0
    print("====================Start Valid====================")
    for label in labels:
        for sample in valid_data[label] :
            num = num +1
            result2 = demo.predict(sample)
            predict_result = demo.softmax(result2)
            if(predict_result != label) : error = error + 1
    correct_rate = (num-error)/num
    print("校验集上正确率: ",round(correct_rate,2),"%")
    print("====================End Valid====================")
