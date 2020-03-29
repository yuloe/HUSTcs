#include "generateCnf.h"
#include <stdio.h>
//不存在连续的三个0或者三个1
int constraintOne(FILE*file,int m) {
  // row
  int i, j;  // j表示行，i表示列
  for (j = 0; j <= 2 * m - 1; j++) {
    for (i = 1; i <= 2 * m - 2; i++) {
      fprintf(file,"%d %d %d 0\n", i + j * 2 * m, i + j * 2 * m + 1,
             i + j * 2 * m + 2);
      fprintf(file,"%d %d %d 0\n", -i - j * 2 * m, -i - j * 2 * m - 1,
             -i - j * 2 * m - 2);
    }
  }
  // column
  for (i = 1; i <= 2 * m; i++) {
    for (j = 0; j <= 2 * m - 3; j++) {
      fprintf(file,"%d %d %d 0\n", i + j * 2 * m, i + (j + 1) * 2 * m,
             i + (j + 2) * 2 * m);
      fprintf(file,"%d %d %d 0\n", -i - j * 2 * m, -i - (j + 1) * 2 * m,
             -i - (j + 2) * 2 * m);
    }
  }
  return 0;
}

//返回2*m中选m+1的组合数      columnOrRow==1表示此时输出行，0表示输出列
int combine(FILE* file,int m, int row, int column, int columnOrRow) {
  int* order = (int*)malloc(sizeof(int) * (m + 1));
  int i;
  for (i = 0; i <= m + 1; i++)
    order[i] = i - 1;  // 注意这里order[0]=-1用来作为循环判断标识

  int count = 0;
  int k = m + 1;
  int flag = 1;  // 标志找到一个有效组合
  while (order[0] == -1) {
    if (flag)  // 输出符合要求的组合
    {
      if (columnOrRow == 1) {
        for (i = 1; i <= m + 1; i++) {
          fprintf(file,"%d ", order[i] + row * 2 * m + 1);
        }
        fprintf(file,"0\n");
        for (i = 1; i <= m + 1; i++) {
          fprintf(file,"-%d ", order[i] + row * 2 * m + 1);
        }
      } else {
        for (i = 1; i <= m + 1; i++) {
          fprintf(file,"%d ", order[i] * 2 * m + column);
        }
        fprintf(file,"0\n");
        for (i = 1; i <= m + 1; i++) {
          fprintf(file,"-%d ", order[i] * 2 * m + column);
        }
      }
      fprintf(file,"0\n");
      count++;
      flag = 0;
    }

    order[k]++;             // 在当前位置选择新的数字
    if (order[k] == 2 * m)  // 当前位置已无数字可选，回溯
    {
      order[k--] = 0;
      continue;
    }

    if (k < m + 1)  // 更新当前位置的下一位置的数字
    {
      order[++k] = order[k - 1];
      continue;
    }

    if (k == m + 1) flag = 1;
  }
  return count;
}

//每行每列的0与1的个数相同
int constraintTwo(FILE* file,int m) {
  //输出行
  int i, j;  //分别代表列和行
  int q, p;
  for (i = 1; i <= 2 * m; i++) {
    combine(file,m, 0, i, 0);
  }
  for (j = 0; j < 2 * m; j++) {
    combine(file,m, j, 0, 1);
  }
  //输出列
}

//不存在相同的两行或两列
int constraintThree(FILE* file,int m) {
  int i, j;
  int column, row;
  int a[2 * m], b[2 * m], c[2 * m], d;
  int count;
  // column
  int variable = 4 * m * m + 1;
  for (i = 1; i <= 2 * m; i++) {
    for (j = i + 1; j <= 2 * m; j++) {
      for (count = 0; count < 2 * m; count++) {
        a[count] = variable + count;
        b[count] = variable + 2 * m + count;
        c[count] = variable + 4 * m + count;
      }
      d = variable + 6 * m;
      for (row = 0; row <= 2 * m - 1; row++) {
        fprintf(file,"%d -%d -%d 0\n", a[row], 2 * m * row + i, 2 * m * row + j);
        fprintf(file,"-%d %d 0\n", a[row], 2 * m * row + i);
        fprintf(file,"-%d %d 0\n", a[row], 2 * m * row + j);
        fprintf(file,"%d %d %d 0\n", b[row], 2 * m * row + i, 2 * m * row + j);
        fprintf(file,"-%d -%d 0\n", b[row], 2 * m * row + i);
        fprintf(file,"-%d -%d 0\n", b[row], 2 * m * row + j);
        fprintf(file,"-%d %d %d 0\n", c[row],a[row], b[row]);
        fprintf(file,"%d -%d 0\n", c[row], a[row]);
        fprintf(file,"%d -%d 0\n", c[row], b[row]);
      }
      fprintf(file,"%d ", -d);
      for (count = 0; count < 2 * m; count++) {
        fprintf(file,"-%d ", c[count]);
      }
      fprintf(file,"0\n");
      for (count = 0; count < 2 * m; count++) {
        fprintf(file,"%d %d 0\n", c[count], d);
      }
      fprintf(file,"%d 0\n",d);
      variable += 6 * m + 1;
    }
  }
  // row
  for (j = 0; j <= 2 * m - 1; j++) {
    for (i = j + 1; i <= 2 * m - 1; i++) {
      for (count = 0; count < 2 * m; count++) {
        a[count] = variable + count;
        b[count] = variable + 2 * m + count;
        c[count] = variable + 4 * m + count;
      }
      d = variable + 6 * m;
      for (column = 1; column <= 2 * m; column++) {
        fprintf(file,"%d -%d -%d 0\n", a[column - 1], 2 * m * i + column,
               2 * m * j + column);
        fprintf(file,"-%d %d 0\n", a[column - 1], 2 * m * i + column);
        fprintf(file,"-%d %d 0\n", a[column - 1], 2 * m * j + column);
        fprintf(file,"%d %d %d 0\n", b[column - 1], 2 * m * i + column,
               2 * m * j + column);
        fprintf(file,"-%d -%d 0\n", b[column - 1], 2 * m * i + column);
        fprintf(file,"-%d -%d 0\n", b[column - 1], 2 * m * j + column);
        fprintf(file,"-%d %d %d 0\n", c[column - 1],a[column - 1], b[column - 1]);
        fprintf(file,"%d -%d 0\n", c[column - 1], a[column - 1]);
        fprintf(file,"%d -%d 0\n", c[column - 1], b[column - 1]);
      }
      fprintf(file,"%d ", -d);
      for (count = 0; count < 2 * m; count++) {
        fprintf(file,"-%d ", c[count]);
      }
      fprintf(file,"0\n");
      for (count = 0; count < 2 * m; count++) {
        fprintf(file,"%d %d 0\n", c[count], d);
      }
      fprintf(file,"%d 0\n",d);
      variable += 6 * m + 1;
    }
  }
}

//打印cnf文件，将数独转化为cnf文件
int transfer(int* sudokuValue, int m, int seleceLitNum) {
  FILE *file = fopen("sudoku.cnf", "w");
  if (m == 2) {
    fprintf(file,"p cnf %d %d\n", 172, seleceLitNum + 600);
  } else {
    fprintf(file,"p cnf %d %d\n", 606, 2316 + seleceLitNum);
  }
  int i;
  for (i = 0; i < 2 * 2 * m * m; i++) {
    if (sudokuValue[i] != -1) {
      if (sudokuValue[i] == 1) {
        fprintf(file,"%d 0\n", i + 1);
      } else {
        fprintf(file,"-%d 0\n", i + 1);
      }
    }
  }
  constraintOne(file,m);
  constraintTwo(file,m);
  constraintThree(file,m);
  fclose(file);
  return 0;
}