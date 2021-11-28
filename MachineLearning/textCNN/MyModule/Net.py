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
        :param kernal: cupy创建的卷积核(H,W,C,num)
        '''
        height, width, in_channels, out_channels = kernal
        self.kernal = cp.random.standard_normal(kernal)
        self.b = cp.random.standard_normal(out_channels)
        self.k_gradient = cp.zeros(kernal)
        self.b_gradient = cp.zeros(out_channels)
        self.stride = stride

    def forward(self, x):

        '''
        输入 batches w h channels
        输出 batches 1 feature_h kernal_outchannels
        :param x:    输入参数（batches,W,H,C） H 文本分词后最大个数 W 特征向量维度 C 为通道数
        :return:
        '''
        self.x = x
        batch_size, xh, xw, xc = self.x.shape
        kh, kw, kc, knum = self.kernal.shape
        feature_h = (xh - kh) + 1
        feature = cp.zeros((batch_size, feature_h, 1, knum))

        self.image_col = []
        kernal = self.kernal.reshape(-1, knum)

        for i in range(batch_size):
            image_col = img2col(self.x[i], kh, self.stride)
            feature[i] = (cp.dot(image_col, kernal) + self.b).reshape(feature_h, 1, knum)
            self.image_col.append(image_col)
        return feature

    def backward(self, delta, learning_rate):
        batch_size, xh, xw, xc = self.x.shape
        kh, kw, kc, knum = self.kernal.shape
        delta_batches, dh, dw, dc = delta.shape

        # 计算权重梯度
        delta_col = delta.reshape(delta_batches, -1, dc)
        # 重置梯度
        self.k_gradient = self.k_gradient * 0
        for i in range(delta_batches):
            self.k_gradient += cp.dot(self.image_col[i].T, delta_col[i]).reshape(self.kernal.shape)

        self.k_gradient /= batch_size
        # 重置偏置梯度
        self.b_gradient = cp.sum(delta_col, axis=(0, 1))
        self.b_gradient /= batch_size

        self.k_gradient += 1e-9
        self.b_gradient += 1e-9

        # 忽略继续往前传播部分

        # 反向传播
        self.kernal -= self.k_gradient * learning_rate
        self.b -= self.b_gradient * learning_rate


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
        self.x = x
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
                    feature[batch, row, 0, c] = cp.max(x[batch, row * self.ksize:row * self.ksize + self.ksize, 0, c])
                    index = cp.argmax(x[batch, row * self.ksize:row * self.ksize + self.ksize, 0, c])
                    self.feature_mask[batch, row * self.ksize + index, 0, c] = 1
        return feature

    def backward(self, delta):
        # print(self.feature_mask.shape)
        # print(delta.shape)
        # print(cp.repeat(delta, self.ksize, axis=1).shape)
        origin_height = self.feature_mask.shape[1]
        if (origin_height % 2 == 1 and origin_height-self.ksize+1 !=1):

            result = cp.repeat(delta, self.ksize, axis=1)
            result = cp.pad(result, ((0, 0), (0, 1), (0, 0), (0, 0)), 'constant')
            # print(result.shape)
            result = result * self.feature_mask
            return result
        else:
            return cp.repeat(delta, self.ksize, axis=1) * self.feature_mask


class Flatten:
    '''

    '''

    # def __init__(self,width,height,channels):
    #     # self.width = width
    #     # self.height = height
    #     # self.channels = channels

    def forward(self, x):
        self.x = x
        batch_size, xh, xw, xc = x.shape
        # reuslt = cp.zeros((batch_size, xc * xh, 1))
        self.height = xh
        self.width = xw
        self.channels = xc

        result = x.reshape(batch_size, xc*xh,1)

        return result

    def backward(self, delta):
        batch_size, dh = delta.shape
        backward_flatten = cp.zeros((batch_size, self.height, 1, self.channels))
        # print(f"origin_height:{self.height} delta_height:{dh}")
        for batch in range(batch_size):
            backward_flatten[batch] = delta[batch].reshape(self.height, 1, self.channels)
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
        # print(f"result = {result.shape}")
        for i in range(xbatch_size):
            # 此处为行向量方便后续的矩阵乘法计算全连接层
            result[i] = cp.concatenate((x[i], y[i], z[i]), axis=0).reshape(1, -1)
            # print(result[i].shape)
        return result

    def backward(self, delta):
        # print(delta.shape)
        delta_batches, total_height = delta.shape
        x = cp.zeros((delta_batches, self.firstHeight))
        y = cp.zeros((delta_batches, self.secondHeight))
        z = cp.zeros((delta_batches, self.thirdHeight))

        for i in range(delta_batches):
            x[i] = delta[i][0:self.firstHeight]
            y[i] = delta[i][self.firstHeight:self.firstHeight + self.secondHeight]
            z[i] = delta[i][self.firstHeight + self.secondHeight:self.firstHeight + self.secondHeight + self.thirdHeight]
        return x, y, z


class Fc:
    def __init__(self, inchannels, outchannels):
        # # todo scale 含义不明
        # scale = cp.sqrt(inchannels / 2)
        # self.k = cp.random.standard_normal((inchannels, outchannels)) / scale
        # self.b = cp.random.standard_normal(outchannels) / scale
        self.inchannels = inchannels
        self.outchannels = outchannels
        self.k = cp.random.standard_normal((inchannels, outchannels))
        self.b = cp.random.standard_normal(outchannels)
        self.k_gradient = cp.zeros((inchannels, outchannels))
        self.b_gradient = cp.zeros(outchannels)

    def forward(self, x):
        self.x = x
        batch_size = x.shape[0]
        # 小技巧 2维*1维 会根据后部分判断是否符合相乘条件（batch,x) * (k)
        result = cp.dot(self.x, self.k) + self.b
        return result.reshape(batch_size, -1)

    def backward(self, delta, learning_rate):
        batch_size = self.x.shape[0]

        self.k_gradient = cp.dot(self.x.T, delta).reshape(self.inchannels, self.outchannels) / batch_size
        self.b_gradient = cp.sum(delta, axis=0) / batch_size
        result = cp.dot(delta, self.k.T)
        # print(f"K:{self.k.shape} k_gradient:{self.k_gradient.shape}")
        # print(f"b:{self.b.shape} b_gradient:{self.b_gradient.shape}")

        self.k -= self.k_gradient * learning_rate
        self.b -= self.b_gradient * learning_rate

        return result


class Softmax:

    def calcuteLoss(self, x, label):
        batch_size, result = x.shape
        self.predict(x)
        loss = 0
        delta = cp.zeros(x.shape)
        for i in range(batch_size):
            delta[i] = self.softmax[i] - label[i]            # print("softmax: ",self.softmax[i])
            #print(f"delta[i]={delta[i]} softmax[i]={self.softmax[i]} label[i]={label[i]} ")
            # 计算交叉熵
            loss -= cp.sum(cp.log(self.softmax[i]+1e-5) * label[i])
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
