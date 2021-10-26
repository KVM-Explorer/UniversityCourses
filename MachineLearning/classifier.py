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
                mapping[1] = self.labels[i]
                mapping[-1] =self.labels[j]
                # 构建训练数据集
                data1 = origin_data[self.labels[i]]
                label1 = ones((shape(data1)[0],1))

                data2 = origin_data[self.labels[j]]
                label2 = -1 *ones((shape(data2)[0],1))

                data = vstack((data1,data2))
                labels = vstack((label1,label2))
                # 打乱数据集
                new_index = [i for i in range(shape(data)[0])]
                random.shuffle(new_index)
                data = data[new_index]
                labels = labels[new_index]
                labels = mat(labels).T
                # 开始训练
                single_classifier = AdaBoost.Adaboost()

                single_classifier.train(data,labels)
                mapping['classifier'] = single_classifier.save()

                self.multiClassifierArray.append(mapping)

        # 检验训练结果

        # 构建测试集
        data = []
        labels = []
        for i in range(self.classes):
            temp_data = origin_data[self.labels[i]]
            temp_labels = i * ones(shape(temp_data)[0])

            if(data!=[]) :
                data = vstack((data,temp_data))
            else :
                data = temp_data
            if(labels!=[]) :
                labels = hstack((labels,temp_labels))
            else :
                labels = temp_labels

        # 计算错误率
        total_error = 0
        for i in range(shape(data)[0]) :
            result = self.predict(data[i,:])
            label = max(result,key=result.get)
            # print("=================================")
            # print(data[i])

            # print(labels[i] , label)
            id = int(labels[i])
            print("result: ",result, " labels: ",self.labels[id])
            if label != self.labels[id] : total_error = total_error + 1

        error_rate = total_error / shape(data)[0]

        return error_rate



    def predict(self, data):
        test_data = 0
        index = 0

        # 初始化分类结果
        all_result = {}
        for name in self.labels: all_result[name] = 0

        for i in range(self.classes):
            for j in range(i):
                classifier = AdaBoost.Adaboost()
                classifier.load(self.multiClassifierArray[index]['classifier'])
                result = classifier.predict(data)
                result = array(result)[0][0]
                result = self.multiClassifierArray[index][result]
                all_result[result] += 1
                index = index + 1
        for classes in all_result :
            all_result[classes] /=(self.classes*(self.classes-1)/2)
        return all_result

    def load(self,classifier,labels):
        self.multiClassifierArray = classifier
        self.labels = labels
        self.classes = len(labels)

    def save(self):
        return self.multiClassifierArray,self.labels