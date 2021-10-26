from numpy import *

class Adaboost:

    def __init__(self, iterations = 40, step_number = 10):
        self.weakClassifierArray = []
        self.iterations = iterations
        self.stepNumber = step_number
    def classify(self,data,feature,threshold,position):
        result = ones((shape(data)[0],1))
        if position == 'upper' :
            result[ data[:,feature] <= threshold ] = -1.0
        else:
            result[data[:,feature] > threshold] =-1.0
        return result

    def train(self,data,labels):
        sample_number = shape(data)[0]
        sample_weight = mat(ones((sample_number,1))/sample_number)
        classifier_result = mat(zeros((sample_number,1)))

        for i in range(self.iterations) :
            # 训练弱分类器
            simple_classifier,error_rate,simple_result = \
                self.buildClassifier(data,labels,sample_weight)

            # 计算弱分类器权重系数
            alpha = float( 0.5 * log((1.0-error_rate)/max(error_rate,1e-16)))
            simple_classifier['alpha'] = alpha
            self.weakClassifierArray.append(simple_classifier)

            # 更新样本权重系数
            tmp = multiply(-1 * alpha * mat(labels).T,simple_result)
            sample_weight = multiply(sample_weight , exp(tmp))
            sample_weight = sample_weight / sample_weight.sum()

            # 更新强分类器结果
            classifier_result  += alpha * simple_result

            # 当前强分类结果的错误率
            classifier_errors = multiply(sign(classifier_result) != mat(labels).T,ones((sample_number,1)))
            classifier_errors_rate = classifier_errors.sum() / sample_number
            print("Total Error:", classifier_errors_rate)
            if(classifier_errors_rate == 0.0) : break
        return classifier_result

    def buildClassifier(self,data,labels,sample_weight):
        data_matrix = mat(data)
        label_matrix = mat(labels).T
        sample_number , sample_feature = shape(data_matrix)
        steps = self.stepNumber
        simple_classifier = {}
        simple_result = mat(zeros((sample_number,1)))
        min_error = inf

        for i in range(sample_feature):
            lower = data_matrix[:,i].min()
            upper = data_matrix[:,i].max()
            step_size = (upper-lower)/steps
            for j in range(-1,(int)(steps)+1) :
                for postion  in ['upper','lower']:
                    threshold = (lower + float(j)*step_size)
                    predict_result = self.classify(data_matrix,i,threshold,postion)

                    error_array = mat(ones((sample_number,1)))
                    error_array[predict_result==label_matrix] = 0
                    total_error = sample_weight.T *error_array

                    if total_error < min_error :
                        min_error = total_error
                        simple_result = predict_result.copy()
                        simple_classifier['feature'] = i
                        simple_classifier['threshold'] = threshold
                        simple_classifier['postion'] = postion
        return simple_classifier,min_error,simple_result


    def predict(self,data):
        data_matrix = mat(data)
        sample_number = shape(data_matrix)[0]
        classifier_result = mat(zeros(sample_number,1))

        for i in range(len(self.weakClassifierArray)):
            predict_result = self.classify(data_matrix,
                                           self.weakClassifierArray[i]['feature'],
                                           self.weakClassifierArray[i]['threshold'],
                                           self.weakClassifierArray[i]['postion'])
            classifier_result += self.weakClassifierArray[i]['alpha'] * predict_result
        return sign(classifier_result)

    def load(self,model_parameter):
        self.weakClassifierArray = model_parameter
    def save(self):
        return self.weakClassifierArray

