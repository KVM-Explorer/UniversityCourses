import random

import adaboost
from numpy import *


class MultiClassifier:

    def __init__(self,labels,num = 40,step =10):
        '''
        构造类，初始化成员
        :param labels: 数据标签需保持和训练集和测试集对应
        :param num: 单个Adaboost弱分类器个数
        :param step: 单个Adaboost弱分类器精细程度
        '''
        self.multiClassifierArray = []
        self.labels = labels
        self.classes = len(self.labels)
        self.num = num
        self.step = step

    # 按类别输入数据
    def train(self, origin_data):
        '''
        针对输入数据按照标签进行采用adaboost进行多分类训练
        :param origin_data: 以字典的形式进行存储键值分别对应类别和该类别的数据集，数据集包含多个样本每个样本以特征分量进行存储构建一个矩阵
        :return: 返回训练后对于训练数据的错误率
        '''
        print("====================Start Training====================")
        index= 0
        for i in range(self.classes):
            for j in range(i):
                index = index + 1
                print(f"iterators: {index}/{(self.classes*(self.classes-1))/2}")
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
                single_classifier = adaboost.Adaboost(self.num,self.step)

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
            # print("result: ",result, " labels: ",self.labels[id])
            if label != self.labels[id] : total_error = total_error + 1

        error_rate = total_error / shape(data)[0]
        print("====================End Traing====================")
        return error_rate



    def predict(self, data):
        '''
        对输入数据进行预测
        :param data: 一个样本的各特征分量
        :return: 各个类别的概率
        '''
        test_data = 0
        index = 0

        # 初始化分类结果
        all_result = {}
        for name in self.labels: all_result[name] = 0

        for i in range(self.classes):
            for j in range(i):
                classifier = adaboost.Adaboost()
                classifier.load(self.multiClassifierArray[index]['classifier'])
                result = classifier.predict(data)
                result = array(result)[0][0]
                result = self.multiClassifierArray[index][result]
                all_result[result] += 1
                index = index + 1
        for classes in all_result :
            all_result[classes] /=(self.classes*(self.classes-1)/2)
        return all_result

    def softmax(self,result):
        return max(result,key=result.get)

    def load(self,classifier,labels):
        '''
        加载模型参数
        :param classifier: 分类器参数
        :param labels: 标签参数
        '''
        self.multiClassifierArray = classifier
        self.labels = labels
        self.classes = len(labels)
    def loadFile(self,modelname,labelsname):
        '''
        读取文件中保存的模型
        :param filename:
        :return:
        '''
        with open(modelname) as file:
            content = file.readlines()
            classifier = eval(content[0])
        with open(labelsname) as file:
            content = file.readlines()
            labels = eval(content[0])
        # self.load(clas)
        self.load(classifier,labels)

        return self
    def save(self,filename):
        '''
        保存当前模型的参数
        :return: 输出模型参数和标签列别
        '''

        # model
        with  open(filename+"/./adaboost.model","w+") as file:
            print(self.multiClassifierArray,file= file)
        # label
        with open(filename+"/./adaboost.labels","w+") as file :
            print(self.labels,file=file)
        return self.multiClassifierArray,self.labels