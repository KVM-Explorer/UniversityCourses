import cupy as cp


# ======================================
# cupy 使用注意事项不要cpu和gpu混写尽量采用官方 API
# https://www.jianshu.com/p/b5a6ee8564df
# ======================================

def img2col(x, kh, stride=1):
    '''
    卷积展开为矩阵运算
    :param x: 数据输入 w h channels
    :param kh:
    :param stride: 步长
    :return:
    '''
    height, width, channels = x.shape
    kw = width
    feature_height = (height - kh) + 1
    feature_width = 1
    img_col = cp.zeros((feature_width * feature_height, kw * kh * channels))
    num = 0

    for i in range(feature_height):
        for j in range(feature_width):
            img_col[num] = x[i * stride:i * stride + kh, j * stride:j * stride + kw, :].reshape(-1)
            num = num + 1

    return img_col


class Conv:
    def __init__(self, kernal, stride=1):
        '''
        初始化卷积层
        :param kernal: cupy创建的卷积核(W,H,C,num)
        '''
        width, height, in_channels, out_channels = kernal
        self.k = cp.random.standard_normal(kernal)
        self.b = cp.random.standard_normal(out_channels)
        self.k_gradient = cp.zeros(kernal)
        self.b_gradiant = cp.zeros(out_channels)
        self.stride = stride

    def forward(self, x):

        '''
        输入 batches w h channels
        输出 batches 1 feature_h kernal_outchannels
        :param x:    输入参数（batches,W,H,C） H 文本分词后最大个数 W 特征向量维度 C 为通道数
        :param kernal:  卷积核 （W,H,C,num)
        :param b:       偏置，列向量存储
        :param paading: 填充
        :param strides: 卷积移动步长
        :return:
        '''
        self.x = x
        batch_size, xh, xw, xc = self.x.shape
        kh, kw, kc, knum = self.k.shape
        feature_h = (xh - kh) + 1
        feature = cp.zeros((batch_size, feature_h, 1, knum))

        self.image_col = []
        kernal = self.k.reshape(-1, knum)

        for i in range(batch_size):
            image_col = img2col(self.x[i], kh, self.stride)
            feature[i] = (cp.dot(image_col, kernal) + self.b).reshape(feature_h, 1, knum)
            self.image_col.append(image_col)
        return feature

    def backward(self, delta, learning_rate):
        batch_size, xh, xw, xc = self.x.shape
        kh, kw, kc, knum = self.k.shape
        delta_batches, dh, dw, dc = delta.shape

        # 计算梯度
        delta_col = delta.reshape(delta_batches, -1, dc)
        for i in range(delta_batches):
            self.k_gradient += cp.dot(self.image_col[i].T, delta_col[i]).reshape(self.k.shape)

        self.k_gradient /= batch_size
        self.b_gradiant += cp.sum(delta_col, axis=(0, 1))
        self.b_gradiant /= batch_size

        # 忽略继续往前传播部分

        # 反向传播
        self.k -= self.k_gradient * learning_rate
        self.b -= self.b_gradiant * learning_rate


class Relu:
    # 输入 batch k通道列向量
    def forward(self, x):
        self.x = x
        return cp.maximum(x, 0)

    def backward(self, delta):
        delta[self.x < 0] = 0
        return delta


class Pool:
    def __init__(self, ksize):
        '''
        初始化池化范围
        :param ksize:
        :return:
        '''
        self.ksize = ksize

    def forward(self, x):
        batch_size, height, width, channels = x.shape
        feature_width = 1
        feature_height = height // self.ksize
        feature = cp.zeros((batch_size, feature_height, 1, channels))
        # 记录最大池化位置
        self.feature_mask = cp.zeros((batch_size, height, width, channels))

        for batch in range(batch_size):
            for c in range(channels):
                for row in range(feature_height):
                    # print(f"up:{row*self.ksize} down:{row*self.ksize+self.ksize} length:{feature_height} height:{height}")
                    # print(f"x:{x.shape}")
                    feature[batch, row, 0, c] = cp.max(x[batch, row * self.ksize:row * self.ksize + self.ksize, 0:1, c])
                    index = cp.argmax(x[batch, row * self.ksize:row * self.ksize + self.ksize, 0, c])
                    self.feature_mask[batch, row * self.ksize + index // self.ksize, 0, c] = 1
        return feature

    def backward(self, delta):
        return cp.repeat(delta, self.ksize, axis=2) * self.feature_mask


class Flatten:
    '''

    '''

    # def __init__(self,width,height,channels):
    #     # self.width = width
    #     # self.height = height
    #     # self.channels = channels

    def forward(self, x):
        batch_size, xh, xw, xc = x.shape
        reuslt = cp.zeros((batch_size, xc * xh, 1))
        self.height = xh
        self.width = xw
        self.channels = xc

        result = x.reshape(batch_size, -1)

        return reuslt

    def backward(self, delta):
        batch_size, dw, dh = delta.shape
        backward_flatten = cp.zeros((batch_size, dw, self.height, self.channels))

        for batch in range(batch_size):
            backward_flatten[batch] = delta[batch, dh, 1].reshape(self.height, 1, self.channels)
        return backward_flatten


class Concat:
    '''
     合并两个个分支的结果
    '''

    # def __init__(self):

    def forward(self, x, y, z):
        xbatch_size, xh, xw = x.shape
        ybatch_size, yh, yw = y.shape
        zbatch_size, zh, zw = z.shape
        self.firstHeight = xh
        self.secondHeight = yh
        self.thirdHeight = zh
        result = cp.zeros((xbatch_size, 1, yh + xh + zh))
        print(f"result = {result.shape}")
        for i in range(ybatch_size):
            # 此处为行向量方便后续的矩阵乘法计算全连接层
            result[i] = cp.concatenate((x[i], y[i], z[i]), axis=0).reshape(1, -1)
            print(result[i].shape)
        return result

    def backward(self, delta):
        delta_batches, dw, dh, dc = delta.shape
        x = cp.zeros((delta_batches, self.firstHeight))
        y = cp.zeros((delta_batches, self.secondHeight))
        z = cp.zeros((delta_batches, self.thirdHeight))

        for i in range(delta_batches):
            x[i] = delta[i][0:self.firstHeight]
            y[i] = delta[i][self.firstHeight, self.firstHeight + self.secondHeight]
            z[i] = delta[i][
                self.firstHeight + self.secondHeight, self.firstHeight + self.secondHeight + self.thirdHeight]
        return x, y, z


class Fc:
    def __init__(self, inchannels, outchannels):
        # todo scale 含义不明
        scale = cp.sqrt(inchannels / 2)
        self.k = cp.random.standard_normal((inchannels, outchannels)) / scale
        self.b = cp.random.standard_normal(outchannels) / scale
        self.k_gradient = cp.zeros((inchannels, outchannels))
        self.b_gradient = cp.zeros(outchannels)

    def forward(self, x):
        self.x = x
        batch_size = x.shape[0]
        # 小技巧2 维乘 1维 会根据后部分判断是否符合相乘条件（batch,x) * (k)
        result = cp.dot(self.x, self.k) + self.b
        return result.reshape(batch_size, -1)

    def backward(self, delta, learning_rate):
        batch_size = self.x.shape[0]
        self.k_gradient = cp.dot(self.x.T, delta) / batch_size
        self.b_gradient = cp.sum(delta, axis=0) / batch_size
        result = cp.dot(delta, self.k.T)
        print(f"result:{result.shape}")
        print(self.k_gradient.shape)
        print(self.b_gradient.shape)
        print(self.b)
        leanring_k_matrix = cp.repeat(cp.repeat(cp.array([[[learning_rate]]]), self.k_gradient.shape[2], axis=2), self.k_gradient.shape[0], axis=0)
        leanring_b_matrix = cp.repeat(cp.array([learning_rate]),self.b_gradient.shape[0])
        print(leanring_b_matrix.shape)
        print(leanring_k_matrix.shape,self.k_gradient.shape)
        print(self.k.shape)
        self.k -= cp.multiply(self.k_gradient,leanring_k_matrix)
        self.b -= self.b_gradient * leanring_b_matrix

        return result


class Softmax:

    def calcuteLoss(self, x, label):
        batch_size, result = x.shape
        self.predict(x)
        loss = 0
        delta = cp.zeros(x.shape)
        for i in range(batch_size):
            delta[i] = self.softmax[i] - label[i]
            # 计算交叉熵
            loss -= cp.sum(cp.log(self.softmax[i]) * label[i])
        loss /= batch_size
        return loss, delta

    def predict(self, x):
        batch_size, result = x.shape
        self.softmax = cp.zeros(x.shape)
        for i in range(batch_size):
            # 做减法 防止指数幂爆炸
            predict_tmp = x[i] - cp.max(x[i])
            predict_tmp = cp.exp(predict_tmp)
            self.softmax[i] = predict_tmp / cp.sum(predict_tmp)
        return self.softmax
