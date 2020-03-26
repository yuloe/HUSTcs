#include "basic.h"
#include "tree.h"
graph *initGraph(int literalNum) {
  graph *graphA;
  graphA = (graph *)malloc(sizeof(graph));
  graphNode *graph = (graphNode *)malloc(sizeof(graphNode) * literalNum);
  int i;
  for (i = 0; i < literalNum; i++) {
    graph[i].litLinkedHead = NULL;
    graph[i].decideNode = 0;
  }
  graphA->graphNode = graph;
  graphA->literalNum = literalNum;
  return graphA;
}
//生成以逆邻接矩阵方式存储的图，产生冲突时，从冲突节点开始遍历，将得到的最终顶点，可形成学习的子句
//更新图的时候每有一个蕴含的节点，就需要更新图
int updateGraph(graph *graph, int literalIndex, clause *clauseNode) {
  literal *lit;
  graphNode *exactGraphNode = &graph->graphNode[literalIndex - 1];
  if (clauseNode == NULL) {
    exactGraphNode->decideNode = 1;
    lit = NULL;
  } else {
    lit = clauseNode->head->next;
  }
  int i;
  while (lit) {
    if (lit->index != literalIndex) {
      if (exactGraphNode->litLinkedHead == NULL) {
        exactGraphNode->litLinkedHead =
            (literalLinked *)malloc(sizeof(literalLinked));
        exactGraphNode->litLinkedHead->index = lit->index;
        exactGraphNode->litLinkedHead->next = NULL;
        exactGraphNode->litLinkedTail = exactGraphNode->litLinkedHead;
      } else {
        literalLinked *graNode = (literalLinked *)malloc(sizeof(literalLinked));
        graNode->index = lit->index;
        graNode->next = NULL;
        exactGraphNode->litLinkedTail->next = graNode;
        exactGraphNode->litLinkedTail = exactGraphNode->litLinkedTail->next;
      }
    }
    lit = lit->next;
  }
  return 0;
}

//生成学习子句
int clauseLearn(graph *graphA, int index, queue *clauseQue,
                variable *literalValue) {
  queue que;  //用来保存需要遍历的点
  que.num = 0;
  int literalNum = graphA->literalNum;
  int i, j, indexA, indexB, k;
  graphNode *impliedGraph = graphA->graphNode;
  int visited[literalNum];
  for (i = 0; i < literalNum; i++) {
    visited[i] = 0;
  }  //将该节点置为未被拜访过
  enQueue(&que, index);
  visited[index - 1] = 1;
  while (!emptyQueue(&que)) {
    indexA = deQueue(&que);
    literalLinked *temp = impliedGraph[indexA - 1].litLinkedHead;
    while (temp) {
      indexB = temp->index;
      if (!visited[indexB - 1]) {
        visited[indexB - 1] = 1;
        enQueue(&que, indexB);
        if (impliedGraph[indexB - 1].decideNode) {
          if (literalValue[indexB - 1].value == 1) {
            enQueue(clauseQue, -indexB);
          } else {
            enQueue(clauseQue, indexB);
          }
        }
      }
      temp = temp->next;
    }
  }
  enQueue(clauseQue, 0);
  return 0;
}

//添加学习子句，保证子句学习上限为200，当学习到的子句数目大于200时，删除部分子句
int addClauseLearned(CNF *cnf, queue *que, variable *literalValue) {
  static int literalLearnedNum = 0;
  if (literalLearnedNum <= 200) {
    literalLearnedNum++;
  } else {
    clause *tempClause = cnf->clauseLearnedHead->next;
    cnf->clauseLearnedHead->next = cnf->clauseLearnedHead->next->next;
    literal *tempLit = tempClause->head, *tempLitA;
    while (tempLit) {  //释放子句中每个文字的空间
      tempLitA = tempLit;
      tempLit = tempLit->next;
      free(tempLitA);
    }
    free(tempClause);
  }
  int num = 1, flag;
  clause *clauseNode = (clause *)malloc(sizeof(clause));
  clauseNode->next = NULL;
  clauseNode->head = (literal *)malloc(sizeof(literal));
  clauseNode->head->prev = NULL;  //将双向链表的头的prev指针指向NULL
  clauseNode->tail = clauseNode->head;
  clauseNode->satisfied = 1;
  cnf->clauseLearnedTail->next = clauseNode;  //将子句添加到链表
  cnf->clauseLearnedTail = clauseNode;
  while (num) {
    num = deQueue(que);
    if (num == 0) {
      cnf->clauseLearnedTail->headb = cnf->clauseLearnedTail->head->next;
      cnf->clauseLearnedTail->tailb = cnf->clauseLearnedTail->tail;
      return 0;
    } else {
      addliteral(num, clauseNode, cnf->variableInfo, literalValue);
    }
  }
}

//当决策树被回溯时，图也需要回溯
int backTrackGraph(graph *gra, decideTreeNode *treeNode, decideTree *tree) {
  int i = 0;
  int index = treeNode->index;
  gra->graphNode[index - 1].decideNode = 0;  //释放图的决策点
  queueNode *tempQueNode = treeNode->que->num == 0? NULL:treeNode->que->head;
  while (tempQueNode) {
    index = tempQueNode->index;
    literalLinked *tempLitLinked =
        gra->graphNode[index - 1].litLinkedHead;  //删除掉联系关系
    literalLinked *tofree;
    while (tempLitLinked) {
      tofree = tempLitLinked;
      tempLitLinked = tempLitLinked->next;
      free(tofree);
    }  //释放完成
    gra->graphNode[index - 1].litLinkedHead = NULL;
    for (i = 0; i < gra->literalNum; i++) {
      tempLitLinked = gra->graphNode[i].litLinkedHead;
      if(tempLitLinked == NULL||i == index - 1)
        continue;
      if (tempLitLinked->index == index) {
        gra->graphNode[i].litLinkedHead = tempLitLinked->next;
        free(tempLitLinked);
      } else {
        while (tempLitLinked->next) {
          if (tempLitLinked->next->index == index) {
            tofree = tempLitLinked->next;
            tempLitLinked->next = tempLitLinked->next->next;
            free(tofree);
            break;
          }
          tempLitLinked = tempLitLinked->next;
        }
      }
    }
    tempQueNode = tempQueNode->next;
  }
}

int printGrapf(graph *gra) {
  int i, j;
  for (i = 0; i < gra->literalNum; i++) {
    if (gra->graphNode[i].decideNode == 1) {
      printf("%d:decideLiteral\n", i + 1);
      continue;
    }
    if (gra->graphNode[i].litLinkedHead == NULL) {
      continue;
    } else {
      printf("%d:  ", i + 1);
      literalLinked *temp = gra->graphNode[i].litLinkedHead;
      while (temp) {
        printf("%d ", temp->index);
        temp = temp->next;
      }
      printf("\n");
    }
  }
  printf("\n");
}