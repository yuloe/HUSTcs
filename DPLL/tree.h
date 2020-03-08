#ifndef TREE_H
#define TREE_H
//队列的定义，二者的存储不带头指针也不带尾指针
typedef struct queueNode{
  int index;
  struct queueNode *next;
}queueNode;

typedef struct queue{
  queueNode *head;
  queueNode *tail;
  int num;
}queue;

//决策树的定义
typedef struct decideTreeNode{
  int index;
  int value;
  int left;
  queue *que;
  struct decideTreeNode *prev;
  struct decideTreeNode *next;
}decideTreeNode;

typedef struct decideTree{
  decideTreeNode* decideTreeHead;//需要带头指针
  decideTreeNode* deicdeTreeTail;
}decideTree;

#endif