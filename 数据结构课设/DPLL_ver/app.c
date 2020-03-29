#include "dpll.c"
#include "read.c"
#include "write.c"
#include <time.h>

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
      if(literalValue[tempLiteral->index - 1].value == -1 || !(tempLiteral->noLit == literalValue[tempLiteral->index - 1].value)){
        flag = 1;
        break;
      }
      tempLiteral = tempLiteral->next;
    }
    tempClause = tempClause->next;
  }
  printf("%d", flag);
}

int main() {
  char *fileName;
  time_t start_t,end_t;
  CNF theCnf;  //定义cnf范式;
  variable *literalValue;//变元存储单元
  queue que;
  que.num = 0;
  addCNF(&theCnf, &literalValue,&fileName);
  start_t = clock();
  int flag = DPLL(&theCnf, literalValue);
  end_t = clock();
  double total = (double)(end_t - start_t)/CLOCKS_PER_SEC*1000;
  write(&theCnf, literalValue, flag,total,fileName);
  return 0;
}
