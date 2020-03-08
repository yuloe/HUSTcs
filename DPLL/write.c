#include "basic.h"
//对每个算例的求解结果要求输出到一个与算例同名的文件（文件扩展名为.res），文件内容与格式要求如下：
// s求解结果 1表示满足，0表示不满足，-1未定
// v -1 2 -3 …
// 满足时，每个变元的赋值序列，-1表示第一个变元1取假，2表示第二个变元取真，用空格分开，此处为示例。
// t 17     以毫秒为单位的DPLL执行时间，可增加分支规则执行次数信息

int write(CNF *cnf, variable *literalIndex, int satisfied) {
  FILE *file = fopen("answer.res", "w+");
  int i = 0;
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
  return 0;
}