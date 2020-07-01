import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
import numpy as np


def knn():
    mnist = input_data.read_data_sets("./data/mnist/", one_hot=True)
    #取出训练集中的数据
    train_x, train_y = mnist.train.next_batch(60000)
    test_x, test_y = mnist.test.next_batch(10000)

    # 占位符
    train_x_p = tf.placeholder(tf.float32, [None, 784])
    test_x_p = tf.placeholder(tf.float32, [784])

    # 计算曼哈顿距离distance = sum(|X1-X2|)
    distance = tf.reduce_sum(tf.abs(train_x_p + tf.negative(test_x_p)), reduction_indices=1)

    #计算欧式距离distance = sqrt(sum(|X1-X2|^2))
    #distance = tf.sqrt(tf.reduce_sum(
        #tf.square(tf.abs(train_x_p + tf.negative(test_x_p))), reduction_indices=1))

    prediction = tf.arg_min(distance, 0)        # 预测测试集中的手写体图片的值
    accuracy = 0.                               # 用于计算最终的准确率
    init = tf.initialize_all_variables()
    with tf.Session() as sess:
        sess.run(init)

        for i in range(len(test_x)):
            # 开始预测测试集图片的值
            index = sess.run(prediction, feed_dict={train_x_p: train_x, test_x_p: test_x[i, :]})
            print("NO.%d image, true value is %d, prediction is %d" %
                  (i, np.argmax(test_y[i]), np.argmax(train_y[index])))
            # 预测正确时，将准确率提高
            if np.argmax(test_y[i]) == np.argmax(train_y[index]):   
                accuracy += 1. / len(test_x)

        print("the accuracy%f " % accuracy)

    return None


if __name__ == '__main__':
    knn()
