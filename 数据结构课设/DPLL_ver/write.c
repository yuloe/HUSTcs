#include "basic.h"
#include <string.h>
//对每个算例的求解结果要求输出到一个与算例同名的文件（文件扩展名为.res）
// s求解结果 1表示满足，0表示不满足
// v -1 2 -3 …每个变元的赋值序列，-1表示第一个变元1取假，2表示第二个变元取真，用空格分开，前有？表示0/1取值皆可
// t 17     以毫秒为单位的DPLL执行时间

int write(CNF *cnf, variable *literalIndex, int satisfied,double total_t,char *fileName) {
  int i = 0;
  char a = 'a';
  while(a!='.'){
    a = fileName[i];
    i++;
  }
  fileName[i] = 'r';
  fileName[i+1] = 'e';
  fileName[i+2] = 's';
  FILE *file = fopen(fileName, "w+");
  if (satisfied) {
    fprintf(file,"s 1\nv ");
    i = 0;
    for (i = 0; i < cnf->literalNum; i++) {
      if (literalIndex[i].value != -1) {
        if (literalIndex[i].value == 1) {
          fprintf(file, "%d   ", i + 1);
        } else {
          fprintf(file, "%d   ", -i - 1);
        }
      } else {
        fprintf(file, "?%d   ", i + 1);
      }
    }
  }else{
    fprintf(file,"s 0");
  }
  fprintf(file, "\nt %lfms", total_t);
  return 0;
}