#include "tree.h"
//初始化“树”
decideTree *initTreeHead() {
  decideTree *tree = (decideTree *)malloc(sizeof(decideTree));
  tree->decideTreeHead = (decideTreeNode *)malloc(sizeof(decideTreeNode));
  tree->decideTreeHead->prev = NULL;
  tree->decideTreeHead->que = NULL;
  tree->decideTreeHead->next = NULL;
  tree->deicdeTreeTail = tree->decideTreeHead;
  return tree;
}
//新建节点
int newNode(decideTree *tree, int indexValue) {
  decideTreeNode *node = (decideTreeNode *)malloc(sizeof(decideTreeNode));
  node->next = NULL;
  node->que = (queue *)malloc(sizeof(queue));
  node->que->num = 0;
  node->left = 1;
  node->index = abs(indexValue);
  node->value = indexValue > 0 ? 1 : 0;
  tree->deicdeTreeTail->next = node;
  node->prev = tree->deicdeTreeTail;
  tree->deicdeTreeTail = node;
  return 0;
}
//默认释放的为链表的尾节点
int freeNode(decideTree *tree) { 
  decideTreeNode *node = tree->deicdeTreeTail;
  free(node->que);
  tree->deicdeTreeTail = node->prev;
  node->prev->next = NULL;
  free(node);
  return 0;
}

int emptyQueue(queue *queueA) {  //判断队列空
  if (queueA->num == 0) return 1;
  return 0;  //队列或栈为空时返回1，不为空返回1
}

int enQueue(queue *queueA, int index) {  //进队列
  if (!emptyQueue(queueA)) {
    queueA->tail->next = (queueNode *)malloc(sizeof(queueNode));
    queueA->tail = queueA->tail->next;
  } else {
    queueA->head = (queueNode *)malloc(sizeof(queueNode));
    queueA->tail = queueA->head;
  }
  queueA->tail->index = index;
  queueA->tail->next = NULL;
  queueA->num++;
  return 0;
}

int deQueue(queue *queueA) {  //出队列，返回出队列的值index
  if (!emptyQueue(queueA)) {
    int index = queueA->head->index;
    queueNode *queueB = queueA->head;
    queueA->head = queueA->head->next;
    free(queueB);
    queueA->num--;
    return index;
  }
  return -1;  //如果队列为空，返回-1
}