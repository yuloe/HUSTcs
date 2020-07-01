import pandas as pd
from sklearn.feature_extraction.text import CountVectorizer, TfidfTransformer
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB, MultinomialNB
from sklearn.metrics import accuracy_score
import jieba
import re
# 读入数据，并对数据进行处理
'''创建停用词表'''
def createStopWordList(filename):
    stopWordList = []
    with open(filename, 'r', encoding='utf-8') as ft:
        for line in ft.readlines():
            stopWordList.append(''.join(line.splitlines()))
    return stopWordList
            
'''读入数据，删除停用词，删除数字，删除英文内容'''
def data_preprocessing(filename, stopWordList):
    Y = []
    X = []
    with open(filename, 'r', encoding='utf-8') as ft:
        for line in ft.readlines():
            Y.append(line.split('	')[0])
            spamText = jieba.lcut(line.split('	')[1])
            #删除停用词
            for stopword in stopWordList:
                while spamText.count(stopword) > 0 :
                    spamText.remove(stopword)
            #删除数字和字母部分
            length = len(spamText)
            index = 0
            while index < length:
                if not all(map(lambda c:'\u4e00' <= c <= '\u9fa5',spamText[index])):
                    spamText.pop(index)
                    length -= 1
                    index -= 1
                index += 1
            X.append(' '.join(spamText))
    return X, Y

if __name__ == '__main__':
    stopWordList = createStopWordList('stopwords.txt')
    X, Y = data_preprocessing('labeled_text.txt', stopWordList)
    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.1, random_state=2)
    count_vector = CountVectorizer(min_df=0, token_pattern='\w')
    X_train_termcounts = count_vector.fit_transform(X_train)
    X_test_termcounts = count_vector.transform(X_test)
    #
    tfidf_transformer = TfidfTransformer()
    X_train_tfidf = tfidf_transformer.fit_transform(X_train_termcounts)
    X_test_tfidf = tfidf_transformer.transform(X_test_termcounts)
    #训练分类器
    naive_bayes = MultinomialNB()
    naive_bayes.fit(X_train_tfidf, Y_train)
    predicted_catcommentOneories = naive_bayes.predict(X_test_tfidf)
    accuracy = accuracy_score(Y_test, predicted_catcommentOneories)
    print("accuracy:", accuracy)
    '''with open("result_bayes.txt", "wt") as f:
        for index in range(len(X_test)):
            print("true value:",
                  Y_test[index], "predict value:", predicted_catcommentOneories[index], file=f)'''