#include "basic.h"
#include "optimization.c"
#include "tree.c"
/* 传入index,将cnf化简，并不负责判断是否出现冲突
 * 消去的子句中设置变量值表示已经消去忽略*/

int singleClauseRule(CNF *cnf, int indexValue, variable *literalValue,
                     decideTree *tree) {
  int index = abs(indexValue);
  int value = indexValue > 0 ? 1 : 0;
  linkedClauseNode *tempClausePointer =
      literalValue[index - 1].clauseHead->next;
  clause *tempClause;
  literal *litA;
  while (tempClausePointer) {  //对每一个子句
    tempClause = tempClausePointer->clausePointer;
    if (tempClause->satisfied != 0) {  //当前子句未被满足
      if (tempClause->headb->index == index) {
        //如果headb的指针指向的文字是函数传入的文字
        if (!(tempClause->headb->noLit == value)) {  //该文字为真
          tempClause->satisfied = 0;                 //子句满足
        } else {                                     //该文字为假
          litA = tempClause->headb->next;
          while (literalValue[litA->index - 1].value != -1 &&
                 litA != tempClause->tailb) {
            if (!(literalValue[litA->index - 1].value == litA->noLit)) {
              tempClause->satisfied = 0;
            }
            litA = litA->next;
          }
          tempClause->headb = litA;
        }
      } else if (tempClause->tailb->index == index) {
        if (!(tempClause->tailb->noLit == value)) {  //该文字为真
          tempClause->satisfied = 0;                 //子句满足
        } else {                                     //该文字为假
          litA = tempClause->tailb->prev;
          while (literalValue[litA->index - 1].value != -1 &&
                 litA != tempClause->headb) {
            if (!(literalValue[litA->index - 1].value == litA->noLit)) {
              tempClause->satisfied = 0;
            }
            litA = litA->prev;
          }
          tempClause->tailb = litA;
        }
      }
    }
    tempClausePointer = tempClausePointer->next;
  }
  return 0;
}
/* 当有冲突产生时，返回-1，无事发生返回0，用队列存储蕴含得到的赋值 */
int determineConflict(CNF *cnf, variable *literalValue, int indexValue,
                      queue *containValue, decideTreeNode *treeTail,
                      graph *gra) {
  int index = abs(indexValue);
  linkedClauseNode *linkClause = literalValue[index - 1].clauseHead->next;
  clause *cla;
  cla = cnf->clauseHead->next;
  while (cla) {
    if (cla->satisfied) {              //只有子句为不满足，才会判断
      if (cla->headb == cla->tailb) {  //判断当前子句为单子句
        if (literalValue[cla->headb->index - 1].value == -1) {
          //表示当前文字并未赋值，可蕴含得到结果
          updateGraph(gra, cla->headb->index, cla);
          printGrapf(gra);
          literalValue[cla->headb->index - 1].value =
              cla->headb->noLit == 1 ? 0 : 1;
          enQueue(containValue, cla->headb->index);
          enQueue(treeTail->que, cla->headb->index);
          cla->satisfied = 0;
        } else {
          if (literalValue[cla->headb->index - 1].value == cla->headb->noLit) {
            return -1;
          }
        }
      }
    }
    cla = cla->next;
  }
  return 0;
}

//选择决策变量，选择出值最大但是未被赋值的文字，返回带符号的序号值
int selectLiteral(CNF *cnf, variable *literalValue) {
  int flag = 0, indexValue;
  variableState *variableInfo = cnf->variableInfo;
  for (int i = 0; i < cnf->literalNum; i++) {
    if (literalValue[i].value == -1) {
      if (flag < variableInfo[i].negativeVariable ||
          flag < variableInfo[i].positiveVariable) {
        if (variableInfo[i].negativeVariable >
            variableInfo[i].positiveVariable) {
          flag = variableInfo[i].negativeVariable;
          indexValue = -i - 1;
        } else {
          flag = variableInfo[i].positiveVariable;
          indexValue = i + 1;
        }
      }
    }
  }
  if (indexValue > 0) {
    literalValue[indexValue - 1].value = 1;
  } else {
    literalValue[-indexValue - 1].value = 0;
  }
  return indexValue;
}

/* 回溯，传入回溯节点，只对单个节点进行回溯， */
int backTrack(CNF *cnf, decideTreeNode *tree, variable *literalValue) {
  int restoreLiteral[tree->que->num + 1], num = tree->que->num, i;
  int flag = 0;
  restoreLiteral[0] = tree->index;
  linkedClauseNode *claNode;
  clause *cla;
  literal *litc;
  for (i = 1; !emptyQueue(tree->que); i++) {
    restoreLiteral[i] = deQueue(tree->que);
  }
  for (i = 0; i < num + 1; i++) {  //将所有关联的变量设置为未赋值
    literalValue[restoreLiteral[i] - 1].value = -1;
  }
  for (i = 0; i < num + 1; i++) {  //对这些还原的文字
    claNode = literalValue[restoreLiteral[i] - 1].clauseHead->next;
    while (claNode) {  //对文字相关联的子句
      cla = claNode->clausePointer;
      if (cla->satisfied == 2 || cla->satisfied == -1) {
        claNode = claNode->next;
        continue;
      }
      litc = cla->head->next;
      while (litc) {
        if (literalValue[litc->index - 1].value == -1) {
          if (flag == 0) {
            cla->headb = litc;
            flag = 1;
          }
          cla->tailb = litc;
        } else if (!(literalValue[litc->index - 1].value == litc->noLit)) {
          cla->satisfied = -1;
          break;
        }
        litc = litc->next;
      }
      if (litc == NULL) {
        cla->satisfied = 2;
      }
      flag = 0;
      claNode = claNode->next;
    }
  }
  for (i = 0; i < num + 1; i++) {
    claNode = literalValue[restoreLiteral[i] - 1].clauseHead->next;
    while (claNode) {
      cla = claNode->clausePointer;
      if (cla->satisfied == -1) {
        cla->satisfied = 0;
      } else if (cla->satisfied == 2) {
        cla->satisfied = 1;
      }
      claNode = claNode->next;
    }
  }
  return 0;
}

/* 非时序回溯 */
int smartBackTrack(CNF *cnf, decideTree *tree, variable *literalValue,
                   graph *gra) {
  int index = tree->deicdeTreeTail->index;
  queue clauseQue;
  clauseQue.num = 0;
  clauseLearn(gra, index, &clauseQue, literalValue);
  decideTreeNode *treeNode = tree->decideTreeHead->next;
  decideTreeNode *tempTreeNode;
  queueNode *queNode = clauseQue.head;
  while (queNode) {
    tempTreeNode = treeNode;
    while (tempTreeNode) {
      if (tempTreeNode->index == queNode->index) {
        treeNode = tempTreeNode;
        break;
      }
      tempTreeNode = tempTreeNode->next;
    }
    queNode = queNode->next;
  }
  index = treeNode->index;  //找到回溯到哪一层
  addClauseLearned(cnf, &clauseQue, literalValue);
  treeNode = tree->deicdeTreeTail;
  while (treeNode->index != index) {
    backTrack(cnf, treeNode, literalValue);
    freeNode(tree);
    treeNode = tree->deicdeTreeTail;
  }
}

/* 如果返回值为0，表示该cnf范式可满足 */
int satisfactory(CNF *cnf) {
  clause *cla = cnf->clauseHead->next;
  int flag = 1;
  while (cla) {
    flag += cla->satisfied;
    cla = cla->next;
  }
  return flag;
}

int unsatisfactory(decideTree *tree) {
  int flag = 0;
  decideTreeNode *node = tree->decideTreeHead->next;
  while (node != tree->deicdeTreeTail) {
    if (node->left == 1) {
      flag--;
      break;
    }
    node = node->next;
  }
  return flag;
}

int BCP(CNF *cnf, variable *literalValue, decideTree *tree, int indexValue,
        graph *gra) {
  int flag = 0, litIndex = abs(indexValue), tof = indexValue > 0 ? 1 : 0;
  queue que;
  que.num = 0;
  singleClauseRule(cnf, indexValue, literalValue, tree);
  flag = determineConflict(cnf, literalValue, indexValue, &que,
                           tree->deicdeTreeTail, gra);
  while (!emptyQueue(&que) && flag == 0) {
    litIndex = deQueue(&que);
    litIndex = literalValue[litIndex - 1].value > 0 ? litIndex : -litIndex;
    singleClauseRule(cnf, litIndex, literalValue, tree);
    flag = determineConflict(cnf, literalValue, litIndex, &que,
                             tree->deicdeTreeTail, gra);
  }
  return flag;
}

int DPLL(CNF *cnf, variable *literalValue, graph *gra) {
  int whetherSatisfied = 0;
  decideTree *tree = initTreeHead();
  decideTreeNode *treeNode;
  int flag = 2, indexValue;
  queue que;
  que.num = 0;

  indexValue = selectLiteral(cnf, literalValue);
  updateGraph(gra, abs(indexValue), NULL);
  newNode(tree, indexValue);
  while (flag != 1 && flag != 0) {
    flag = BCP(cnf, literalValue, tree, indexValue, gra);
    if (flag == 0) {
      flag = satisfactory(cnf);
    } else {
      flag = unsatisfactory(tree);
    }
    if (flag == 1) {
      return 1;  //表示可满足
    } else if (flag == 0) {
      return 0;  //表示不可满足
    } else if (flag == -1) {
      smartBackTrack(cnf, tree, literalValue, gra);
      treeNode = tree->deicdeTreeTail;
      while (treeNode->left != 1) {
        backTrack(cnf, treeNode, literalValue);
        freeNode(tree);
        treeNode = tree->deicdeTreeTail;
      }
      backTrack(cnf, treeNode, literalValue);
      treeNode->value = treeNode->value > 0 ? 0 : 1;
      if (treeNode->value > 0) {
        literalValue[treeNode->index - 1].value = 1;
        indexValue = treeNode->index;
      } else {
        literalValue[treeNode->index - 1].value = 0;
        indexValue = -treeNode->index;
      }
      treeNode->left = 0;
      continue;
    } else {
      indexValue = selectLiteral(cnf, literalValue);
      updateGraph(gra, abs(indexValue), NULL);
      printGrapf(gra);
      newNode(tree, indexValue);
      continue;
    }
  }
}