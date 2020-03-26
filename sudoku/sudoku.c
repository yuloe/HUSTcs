//此文件用于输出cnf文件，用于后面的dpll求解。
#include <stdio.h>
#include <stdlib.h>

//读取数独初盘,数独初盘以
//版本1，从文件中读取初盘
int readSudoku(int* sudokuValue) {
  if (freopen("initSudoku.txt", "r", stdin) == NULL) {
    fprintf(stderr, "file open error!\n");
  }
  int index, value;
  while (scanf("%d:%d", &index, &value) != -1) {
    sudokuValue[index - 1] = value;
  }
  return 0;
}
//数独棋盘
void drawboard(int rank, int* puzzle) {
  int carrier;
  int row = 1;
  int column = 0;
  printf("|");
  column += 1;
  while (column < rank * 2) {
    if (column % 2)
      printf("---");
    else
      printf("-");
    column += 1;
  }
  printf("|\n");

  while (row < 2 * rank) {
    column = 0;
    if (row % 2 == 0) {
      printf("|");
      column += 1;
      while (column < rank * 2) {
        if (column % 2)
          printf("---");
        else
          printf("|");
        column += 1;
      }
      printf("|\n");
    } else {
      while (column <= rank * 2) {
        if (column % 2 != 0)
          (carrier = *(puzzle + (row / 2) * rank + column / 2)) != -1
              ? printf(" %d ", carrier)
              : printf("   ");
        else
          printf("|");
        column += 1;
      }
      putchar('\n');
    }
    row += 1;
  }

  column = 0;
  printf("|");
  column += 1;
  while (column < rank * 2) {
    if (column % 2)
      printf("---");
    else
      printf("-");
    column += 1;
  }
  printf("|\n");
}
//自动生成数独，生成数独终盘，再挖洞。为生成终盘，可以先确定随机确定m行使用dpll解出一组可行的解
//再使用挖洞生成初盘，初盘中有m*m/4个已知量

//打印数独，完成之后的样子
int printSudoku(int m, int* sudokuValue) {
  int i, j;
  for (i = 0; i < m; i++) {
    for (j = 0; j < m; j++) {
      printf("%d ", sudokuValue[i * m + j]);
    }
    printf("\n");
  }
}

//不存在连续的三个0或者三个1
int constraintOne(int m) {
  // row
  int i, j;  // j表示行，i表示列
  for (j = 0; j <= 2 * m - 1; j++) {
    for (i = 1; i <= 2 * m - 2; i++) {
      printf("%d %d %d 0\n", i + j * 2 * m, i + j * 2 * m + 1,
             i + j * 2 * m + 2);
      printf("%d %d %d 0\n", -i - j * 2 * m, -i - j * 2 * m - 1,
             -i - j * 2 * m - 2);
    }
  }
  // column
  for (i = 1; i <= 2 * m; i++) {
    for (j = 0; j <= 2 * m - 3; j++) {
      printf("%d %d %d 0\n", i + j * 2 * m, i + (j + 1) * 2 * m,
             i + (j + 2) * 2 * m);
      printf("%d %d %d 0\n", -i - j * 2 * m, -i - (j + 1) * 2 * m,
             -i - (j + 2) * 2 * m);
    }
  }
  return 0;
}

//返回2*m中选m+1的组合数      columnOrRow==1表示此时输出行，0表示输出列
int combine(int m, int row, int column, int columnOrRow) {
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
          printf("%d ", order[i] + row * 2 * m + 1);
        }
        printf("0\n");
        for (i = 1; i <= m + 1; i++) {
          printf("-%d ", order[i] + row * 2 * m + 1);
        }
      } else {
        for (i = 1; i <= m + 1; i++) {
          printf("%d ", order[i] * 2 * m + column);
        }
        printf("0\n");
        for (i = 1; i <= m + 1; i++) {
          printf("-%d ", order[i] * 2 * m + column);
        }
      }
      printf("0\n");
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
int constraintTwo(int m) {
  //输出行
  int i, j;  //分别代表列和行
  int q, p;
  for (i = 1; i <= 2 * m; i++) {
    combine(m, 0, i, 0);
  }
  for (j = 0; j < 2 * m; j++) {
    combine(m, j, 0, 1);
  }
  //输出列
}

//不存在相同的两行或两列
int constraintThree(int m) {
  int i, j;
  int column, row;
  int a[2 * m], b[2 * m], c[2 * m], d;
  int count;
  // column
  int variable = m * m + 1;
  for (i = 1; i <= 2 * m; i++) {
    for (j = i + 1; j <= 2 * m; j++) {
      for (count = 0; count < 2 * m; count++) {
        a[count] = variable + count;
        b[count] = variable + 2 * m + count;
        c[count] = variable + 4 * m + count;
      }
      d = variable + 6 * m;
      for (row = 0; row <= 7; row++) {
        printf("-%d %d %d 0\n", a[row], 2 * m * row + i, 2 * m * row + j);
        printf("%d -%d 0\n", a[row], 2 * m * row + i);
        printf("%d -%d 0\n", a[row], 2 * m * row + j);
        printf("-%d -%d -%d 0\n", b[row], 2 * m * row + i, 2 * m * row + j);
        printf("%d %d 0\n", b[row], 2 * m * row + i);
        printf("%d %d 0\n", b[row], 2 * m * row + j);
      }
      printf("%d ", -d);
      for (count = 0; count < 2 * m; count++) {
        printf("%d ", c[count]);
      }
      printf("0\n");
      for (count = 0; count < 2 * m; count++) {
        printf("%d %d 0\n", c[count], d);
      }
      variable += 6 * m + 1;
    }
  }
  // row
  for (j = 0; j < 2 * m; j++) {
    for (i = j + 1; i <= 2 * m; i++) {
      for (count = 0; count < 2 * m; count++) {
        a[count] = variable + count;
        b[count] = variable + 2 * m + count;
        c[count] = variable + 4 * m + count;
      }
      d = variable + 6 * m;
      for (column = 1; column <= 2 * m; column++) {
        printf("-%d %d %d 0\n", a[column - 1], 2 * m * i + column,
               2 * m * j + column);
        printf("%d -%d 0\n", a[column - 1], 2 * m * column + i);
        printf("%d -%d 0\n", a[column - 1], 2 * m * column + j);
        printf("-%d -%d -%d 0\n", b[column - 1], 2 * m * i + column,
               2 * m * j + column);
        printf("%d %d 0\n", b[column - 1], 2 * m * i + column);
        printf("%d %d 0\n", b[column - 1], 2 * m * j + column);
      }
      printf("%d ", -d);
      for (count = 0; count < 2 * m; count++) {
        printf("%d ", c[count]);
      }
      printf("0\n");
      for (count = 0; count < 2 * m; count++) {
        printf("%d %d 0\n", c[count], d);
      }
      variable += 6 * m + 1;
    }
  }
}

//打印cnf文件，将数独转化为cnf文件
int transfer(int* sudokuValue, int m) {
  int i;
  for (i = 0; i < 2 * 2 * m * m; i++) {
    if (sudokuValue[i] != -1) {
      if (sudokuValue[i] == 1) {
        printf("%d 0\n", i + 1);
      } else {
        printf("-%d 0\n", i + 1);
      }
    }
  }
  constraintOne(m);
  constraintTwo(m);
  constraintThree(m);
}

int mainSudoku(int m) {
  FILE* fileName = freopen("sudoku.cnf", "w+", stdout);
  int sudokuValue[2 * 2 * m * m];
  transfer(sudokuValue, m);
  fclose(stdout);
  freopen("CON", "w", stdout);
  return 0;
}

int main() {
  int m = 4;
  int sudokuValue[m*m*4];
  for(int i = 0; i<m*m*4; i++){
    sudokuValue[i] = 0;
  }
  drawboard(2*m,sudokuValue);
  return 0;
}