import numpy as np
import os
import random
import adaboost

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

def doubleDividedData(data,labels):
    result_data = []
    result_labels = []
    num = 0
    for i in range(len(labels)) :
        data1 = data[labels[i]]
        if i%2==0 : label1 = np.ones(np.shape(data1)[0])
        else : label1 = -1 * np.ones(np.shape(data1)[0])
        num = num + len(data1)
        if i==0 :
            result_data = data1
            result_labels = label1
        else :
            result_data = np.vstack((result_data,data1))
            result_labels = np.hstack((result_labels,label1))
    print(result_labels.shape)
    new_index = [i for i in range(num)]
    random.shuffle(new_index)
    result_data = result_data[new_index]
    result_labels = result_labels[new_index]
    result_labels = result_labels.T
    return result_data,result_labels

def save(filename,demo):
    path = filename +"/./1.model"
    with open(path,"w") as f:
        print(demo.save(),file=f)


def load(filename,demo):
    path = filename + "/./1.model"
    with open(path,"r") as f:
        content = f.readlines()
    content = eval(content[0])
    demo.weakClassifierArray = content
    return demo

if __name__=="__main__":

    path = "/home/lightning/dataset/二分类"
    train,valid,labels= loadFileData(path,0.7)
    #
    demo = adaboost.Adaboost(400,2)
    # train_data, train_labels = doubleDividedData(train, labels)
    #
    # demo.train(train_data, train_labels)
    # save("DoubleDivided",demo)

    # 二分类性能测试
    valid_data,valid_labels = doubleDividedData(valid,labels)
    error = 0
    demo = load("DoubleDivided",demo)
    print(len(valid_labels))
    for i in range(len(valid_labels)):
        result = demo.predict(valid_data[i])
        result = np.array(result)[0][0]
        if result != valid_labels[i] : error = error +1

    right_rate = (len(valid_labels)-error)/len(valid_labels)
    print("BinaryClassification Right Rate:",right_rate)
    print("finish")