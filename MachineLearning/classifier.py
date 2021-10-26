import random

import AdaBoost
from numpy import *


class MultiClassifier:

    def __init__(self,labels):
        self.multiClassifierArray = []
        self.labels = labels
        self.classes = len(self.labels)
    # 按类别输入数据
    def train(self, origin_data):

        for i in range(self.classes):
            for j in range(i):
                # 建立映射关系
                mapping = {}
                mapping[self.labels[i]] = 1
                mapping[self.labels[j]] = -1

                # 构建训练数据集
                data1 = origin_data[self.labels[i]]
                label1 = ones((shape(data1)[0],1))

                data2 = origin_data[self.labels[j]]
                label2 = -1 *ones((shape(data2)[0],1))

                data = vstack(data1,data2)
                labels = vstack(label1,label2)
                # 打乱数据集
                new_index = [i for i in range(shape(data)[0])]
                random.shuffle(new_index)
                data = data[new_index]
                labels = labels[new_index]
                # 开始训练
                classifier = AdaBoost.Adaboost
                classifier.train(data, labels)
                mapping['classifier'] = classifier.save()

                self.multiClassifierArray.append(mapping)

        # 检验训练结果

        # 构建测试集

        # 作出预测


    def predict(self, data):
        test_data = 0
        index = 0

        # 初始化分类结果
        result_dictionary = {}
        for name in self.labels: result_dictionary[name] = 0

        for i in range(self.classes):
            for j in range(i):
                classifier = AdaBoost.Adaboost
                classifier.load(self.multiClassifierArray[index])
                result = classifier.predict(data)
                result = self.multiClassifierArray[index][result]
                result_dictionary[result] += 1
                index = index + 1
        result_dictionary = result_dictionary / (self.classes*(self.classes-1)/2)

        return result_dictionary