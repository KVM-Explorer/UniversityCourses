# Ababoost 分类器

## 简介
以python手动实现adaboost算法,并封装成类
## 使用需求

- numpy

## 扩展——多分类


## 使用

### 测试

main.py 中预留了单元测试函数，可用于简单测试adaboost二元分类和adaboost多元分类

### 二元分类

```python
import adaboost
```
1. 实例化adaboost对象，可通过输入额外的参数对ababoost构造进行自定义
2. 创建训练集和标签集,训练集以二维矩阵save,rows is sample and the column is feature
3. use the object's function **train** and  input the data and label to train the classifier
4. after train,you can use **predict** to predict your personal test data

### 多元分类

1. same as above
2. build a data set fellow the instrction 
3. **train** to train your model
4. same as above


