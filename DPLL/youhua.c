#include "basic.h"
#include "tree.c"
graph *initGraph(int literalNum){
  graph *graphA;
  graphA = (graph*)malloc(sizeof(graph));
  graphNode *graph = (graphNode*)malloc(sizeof(graphNode)*literalNum);
  int i;
  for(i = 0; i < literalNum; i++){
    graph[i].literalLinked = NULL;
    graph[i].decideNode = 0;
    graph[i].linkedNum = 0;
  }
  graphA->graphNode = graph;
  graphA->literalNum = literalNum;
  return graphA;
}
//生成以逆邻接矩阵方式存储的图，产生冲突时，从冲突节点开始遍历，将得到的最终顶点，可形成学习的子句
int updateGraph(graph *graph,int literalIndex,clause *clauseNode){
  literal *lit = clauseNode->head->next;
  graphNode exactGraphNode = graph->graphNode[literalIndex - 1];
  exactGraphNode.decideNode = 1;
  int i;
  while(lit){
    if(exactGraphNode.linkedNum==0){
      exactGraphNode.literalLinked = (int*)malloc(sizeof(int));
      exactGraphNode.linkedNum++;
      exactGraphNode.literalLinked[0] = lit->index;
    }else{
      i = exactGraphNode.linkedNum++;
      exactGraphNode.literalLinked = (int*)realloc(exactGraphNode.literalLinked,i+1);
      exactGraphNode.literalLinked[i] = lit->index;
    }
    lit = lit->next;
  }
  return 0;
}
//生成学习子句
int clauseLearn(graph *graphA,int index ,queue *clauseQue){
  queue que;//用来保存需要遍历的点
  que.num = 0;
  int literalNum = graphA->literalNum;
  int i,j,indexA,indexB,k;
  graphNode *impliedGraph = graphA->graphNode;
  int visited[literalNum];
  for(i = 0; i < literalNum; i++){
    visited[i] = 0;
  }//将该节点是否已被拜访过
  enQueue(&que,index);
  visited[index - 1] = 1;
  while (!emptyQueue(&que)){
    indexA = deQueue(&que);
    for(j = 0; j < impliedGraph[indexA - 1].linkedNum;j++){
      for(k = 0; k < impliedGraph[indexA - 1].linkedNum;k++){
        indexB = impliedGraph[indexA - 1].literalLinked[k];
        if(!visited[indexB - 1]){
          enQueue(&que,indexB);
          if(impliedGraph[indexB - 1].decideNode){
            enQueue(clauseQue,indexB);
          }
        }
      }
    }
  }
}

//添加学习子句，保证子句学习上限为200，当学习到的子句数目大于200时，删除部分子句
int addClauseLearned(CNF *cnf,queue *que,variable* literalValue){
  static int literalLearnedNum = 0;
  int num, flag;
  clause *clauseNode = (clause *)malloc(sizeof(clause));
  clauseNode->next = NULL;
  clauseNode->head = (literal *)malloc(sizeof(literal));
  clauseNode->head->prev = NULL;  //将双向链表的头的prev指针指向NULL
  clauseNode->tail = clauseNode->head;
  clauseNode->literalLearned = 0;
  cnf->clauseLearnedTail->next = clauseNode;  //将子句添加到链表
  while (num = deQueue(que)) {
    if (num == 0) {
      clauseNode->satisfied = 1;
      clauseNode->headb = clauseNode->head->next;
      clauseNode->tailb = clauseNode->tail;
      return 0;
    } else {
      addliteral(num, clauseNode, cnf->variableInfo, literalValue);
    }
  }
}

//非时序回溯，当当前节点指向right时，判断应回溯多少层
int smartBackTrack(){

}

