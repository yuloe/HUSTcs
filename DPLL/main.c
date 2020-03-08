#include "dpll.c"
#include "read.c"
#include "write.c"
int check(CNF *cnf, variable *literalValue) {
  int flag = 1;
  clause *tempClause;
  literal *tempLiteral;
  tempClause = cnf->clauseHead->next;
  while (tempClause) {
    if (flag != 1) {
      break;
    }
    flag = -1;
    tempLiteral = tempClause->head->next;
    while (tempLiteral) {
      if (!(tempLiteral->noLit == literalValue[tempLiteral->index - 1].value)) {
        flag = 1;
        break;
      };
      tempLiteral = tempLiteral->next;
    }
    tempClause = tempClause->next;
  }
  printf("%d", flag);
}
int output1(CNF *cnf) {
  clause *tempClause;
  literal *tempLiteral;
  tempClause = cnf->clauseHead->next;
  while (tempClause) {
    tempLiteral = tempClause->headb;
    while (tempLiteral) {
      if (tempLiteral->noLit > 0) {
        printf("%d ", -tempLiteral->index);
      } else {
        printf("%d ", tempLiteral->index);
      }
      tempLiteral = tempLiteral->next;
    };
    printf("0\n");
    tempClause = tempClause->next;
  }
  return 0;
}

int main() {
  CNF theCnf;  //定义cnf范式;
  variable *literalValue;
  char *fileName;
  queue que;
  que.num = 0;
  addCNF(&theCnf, &literalValue);
  // outputCNF1(&theCnf);
  DPLL(&theCnf, literalValue);
  write(&theCnf, literalValue, 1);
  check(&theCnf, literalValue);
  getchar();
  return 0;
}
