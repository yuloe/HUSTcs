#ifndef BASIC_H
#define BASIC_H
#include <stdio.h>
#include <stdlib.h>
typedef struct literal {
  int noLit;//是否带有否运算，1表示带有，0表示不带有
  int index;//表示文字/变量的序号
  struct literal *next;
  struct literal *prev;
}literal;//文字

typedef struct clause {
  int satisfied;//子句是否满足，0表示子句已被满足，大于0表示未被满足且不是单子句
  literal *tailb;//在读取子句完成之后，tailb为链表尾
  literal *headb;//headb为链表真正的头
  literal *head;//以带头指针的链表的形式存储
  literal *tail;
  int literalLearned;//该子句是否为学习子句，为第多少个学习子句
  struct clause *next;
}clause;//子句

typedef struct linkedClauseNode{
  struct clause *clausePointer;
  struct linkedClauseNode *next;
}linkedClauseNode;

typedef struct variable{
  int value;
  linkedClauseNode *clauseHead;//带头指针的链表存储       两个指针指向同一个带头指针的链表
  linkedClauseNode *clauseTail;//保存该链表的尾           其中一个是指向头，另一个指向尾，
}variable;//变元

typedef struct variableState{
  int positiveVariable;
  int negativeVariable;//统计正负文字各有多少
}variableState;//用来统计，用以VSIDS

typedef struct CNF{
  int literalNum;
  int clauseNum;
  clause *clauseHead;//以带头指针链表的形式存储子句
  clause *clauseLearnedHead;//带头指针的链表存学习子句
  clause *clauseLearnedTail;
  variableState *variableInfo;
}CNF;//cnf范式

typedef struct graphNode{
  int *literalLinked;
  int decideNode;//是否为决策点，1表示是，0表示否
  int linkedNum;//和它相关联的点的数目
}graphNode;

typedef struct graph{
  graphNode *graphNode;
  int literalNum;
}graph;
#endif