import adaboost
import classifier
import numpy as np
import random
import time
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
        print("Data: ", len(origin_data), " Train: ", np.shape(train[label]), " Valid: ", np.shape(valid[label]))
    print("====================End Loading====================")
    return train, valid, labels

def diffTopicTest(data,labels,model,target):
    num = len(data[target])
    error = 0
    count = 0
    right = 0
    print("====================Start Valid====================")
    print(f"target:{target}")
    for label in labels:
        if label !=target :continue
        for sample in data[label]:
            count = count + 1
            # if(count%100==0) :print(f"label: {label}count:{count } total:{num} error:{error}")
            result = model.predict(sample)
            predict_result = model.softmax(result)
            print(predict_result)
            if predict_result != target and target == label : error =error +1
            if predict_result == target and label != target :
                error = error +1
                num = num + 1
            if predict_result == target and label == target : right  = right +1

    error_rate = (error) /num
    print(error, num,len(data[target]))
    right_rate = right / len(data[target])
    print("校验集是错误率：", round(error_rate,2) )
    print("校验集正确率:", round(right_rate,2))
    print("====================End Valid====================")


def validTest(valid_data, demo):
    num = 0
    error = 0
    print("====================Start Valid====================")
    for label in demo.labels:
        for sample in valid_data[label]:
            num = num + 1
            result2 = demo.predict(sample)
            predict_result = demo.softmax(result2)
            if (predict_result != label): error = error + 1
    correct_rate = (num - error) / num
    print("校验集上正确率: ", round(correct_rate, 2))
    print("====================End Valid====================")

if __name__ == "__main__":
    start_time = time.time()
    # adaboost_test()
    # multiClassifier_test()
    # train_data_filename = "/home/lightning/Downloads/qq-files/1158423912/file_recv/tz/dataset"
    # train_data, valid_data, labels = loadFileData(train_data_filename, 0.9)
    # demo = classifier.MultiClassifier(labels, 400, 10)
    # result1 = demo.train(train_data)
    # print("训练集上错误率:", result1)
    # validTest(valid_data,labels,demo)
    # end_time = time.time()
    # print("Total cost time: ", end_time - start_time, 's')
    # demo.save("./model")


    train_data_filename = "/home/lightning/Downloads/qq-files/1158423912/file_recv/tz/dataset"
    train_data, valid_data, labels = loadFileData(train_data_filename, 0)
    # train_data_filename = "/home/lightning/Downloads/qq-files/1158423912/file_recv/xl_tz/dataset/经管/"
    # train_data, valid_data, labels = loadFileData(train_data_filename, 0)
    demo = classifier.MultiClassifier([])
    demo.loadFile("./model/adaboost.model","./model/adaboost.labels")
    # diffTopicTest(valid_data,labels,demo,labels[2])
    validTest(valid_data,demo)