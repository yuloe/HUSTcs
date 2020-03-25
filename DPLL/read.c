#include "basic.h"
#include <math.h>

int addliteral(int num, clause *clause, variableState *variableInfo, variable *literalValue) {
  literal *newNode = (literal *)malloc(sizeof(literal));
  newNode->index = abs(num);
  newNode->noLit = num < 0 ? 1 : 0;
  num > 0 ? variableInfo[newNode->index - 1].positiveVariable++ : variableInfo[newNode->index - 1].negativeVariable++;
  newNode->next = NULL;
  newNode->prev = clause->tail;
  clause->tail->next = newNode;
  clause->tail = clause->tail->next;
  linkedClauseNode *Nodeofclause = (linkedClauseNode *)malloc(sizeof(linkedClauseNode));
  Nodeofclause->clausePointer = clause;
  Nodeofclause->next = NULL;
  literalValue[newNode->index - 1].clauseTail->next = Nodeofclause;
  literalValue[newNode->index - 1].clauseTail = literalValue[newNode->index - 1].clauseTail->next;
  return 0;
}

int addclause(clause *clauseHead, variableState *variableInfo, variable *literalValue) {
  int num, flag;
  clause *clauseNode = (clause *)malloc(sizeof(clause));
  clauseNode->next = NULL;
  clauseNode->head = (literal *)malloc(sizeof(literal));
  clauseNode->head->prev = NULL;  //将双向链表的头的prev指针指向NULL
  clauseNode->tail = clauseNode->head;
  clauseHead->next = clauseNode;  //将子句添加到链表
  while (scanf("%d", &num) >= 0) {
    if (num == 0) {
      clauseNode->satisfied = 1;
      clauseNode->headb = clauseNode->head->next;
      clauseNode->tailb = clauseNode->tail;
      return 0;
    } else {
      addliteral(num, clauseNode, variableInfo, literalValue);
    }
  }
  clauseHead->next = NULL;
  return -1;
}

int addCNF(CNF *cnf, variable **literalValue) {
  clause *tempClause;
  //char* fileName = (char *)malloc(sizeof(int) * 20);
  char b,*a;
  a = (char*)malloc(sizeof(char)*10);
  scanf("%s", a/* fileName */);
  if (freopen(a/* fileName */, "r", stdin) == NULL) {
    fprintf(stderr, "file reopen error\n");
    return -1;
  } else {
    while ((b = getchar()) == 'c') {
      gets(a);
    }
    fgets(a, 5, stdin);
    scanf("%d %d", &cnf->literalNum, &cnf->clauseNum);
    *literalValue = (variable *)malloc(sizeof(variable) * cnf->literalNum);
    for (int i = 0; i < cnf->literalNum; i++) {
      (*literalValue)[i].value = -1;
      (*literalValue)[i].clauseHead = (linkedClauseNode *)malloc(sizeof(linkedClauseNode));
      (*literalValue)[i].clauseTail = (*literalValue)[i].clauseHead;
    }
    cnf->variableInfo = (variableState *)malloc(sizeof(variableState) * cnf->literalNum);
    for(int i = 0; i < cnf->literalNum;i++){
      cnf->variableInfo[i].negativeVariable = 0;
      cnf->variableInfo[i].positiveVariable = 0;    
    }
    if (cnf->variableInfo == NULL) exit(0);  //未能分配足够空间时，直接退出程序
    cnf->clauseHead = (clause *)malloc(sizeof(clause));
    cnf->clauseHead->next = NULL;
    tempClause = cnf->clauseHead;
    while (addclause(tempClause, cnf->variableInfo, *literalValue) != -1) {
      tempClause = tempClause->next;
    }
    cnf->clauseLearnedHead = tempClause;
    cnf->clauseLearnedTail = tempClause;
  }
  fclose(stdin);
  freopen("CON", "r", stdin);
  return 0;
}
//*****以上是从cnf文件中读取范式*****