#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "generatecnf.c"
#define RANDOM rand() % 2
#define COLUMNRAN rand() % rank
int *createoriginsudoku(int rank) {
  int m = rank / 2;
  int *puzzle = malloc(rank * rank * sizeof(int));
  srand(clock());
  int randomx;  //利用rand()的mod2或mod rank余数生成随机的1/0或额外信息列数

  int mark[rank][rank];

  for (int a = 0; a < rank; a++)
    for (int b = 0; b < rank; b++) mark[a][b] = 0;

  for (int a = 0; a < rank; a++)
    for (int b = 0; b < rank; b++) *(puzzle + a * rank + b) = -1;

  for (int a = 0, b = 0; a < rank && b < rank; a++, b++) {
    if (a == m || a == m - 1) {
      break;
    }
    randomx = RANDOM;
    *(puzzle + a * rank + b) = randomx % 2;
    mark[a][b] = 1;
  }
  for (int a = rank - 1, b = 0; a >= 0 && b < rank; a--, b++) {
    randomx = RANDOM;
    *(puzzle + a * rank + b) = randomx % 2;  //生成数独对角线的变量
    mark[a][b] = 1;
  }
  for (int a = 0; a < rank; a++)
    for (int b = 0; b < rank; b++)
      if (mark[a][b] != 1)
        *(puzzle + a * rank + b) = -1;  //将没有赋值的地方挖空
  return puzzle;
}
//统计有多少赋值，以便输出cnf
int count(int m, int *sudokuValue) {
  int count = 0;
  for (int i = 0; i < m * m * 4; i++) {
    if (sudokuValue[i] != -1) {
      count++;
    }
  }
  return count;
}
//挖洞直到不为唯一解，假定已有完整的盘局
//检查是否可解，1表示可解，0表示不行,直接利用SAT求解器求解
int check(int *literalValue, int m) {
  int seleceLitNum = count(m,literalValue);
  char c;
  int flag;
  transfer(literalValue, m, seleceLitNum);
  system("appS.exe");
  FILE* file = fopen("answer.res","r");
  fscanf(file,"%c", &c);
  fscanf(file,"%d",&flag);
  fclose(file);
  return flag;
}

//返回的数组中，-1代表空
int *generateSudoku(int m,int *literalValue) {
  int n = m * m * 4;
  int i, tempValue, flag;
  for (i = 0; i < n; i++) {
    tempValue = literalValue[i];
    if (tempValue == 1) {
      literalValue[i] = 0;
    } else {
      literalValue[i] = 1;
    }
    flag = check(literalValue, m);
    // check函数判断是否能挖去
    if (flag == 1) {  
      literalValue[i] = tempValue;
    } else {
      literalValue[i] = -1;
    }
  }
}

void drawboard(int rank, int *puzzle) {
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

int main() {
  int m, i, indexValue;
  printf("choose the scale of the binary puzzle:(4/6)");
  scanf("%d", &m);
  m = m / 2;
  int *sudokuValue = createoriginsudoku(m * 2);
  int *firstSudoku = (int *)malloc(sizeof(int) * m * m * 4);
  int seleceLitNum = count(m, sudokuValue);
  transfer(sudokuValue, m, seleceLitNum);
  system("appS.exe");
  freopen("answer.res", "r", stdin);
  char *c = (char *)malloc(sizeof(char) * 10);
  char b;
  gets(c);
  if ((b = getchar()) == 'v') {
    for (i = 0; i < 4 * m * m; i++) {
      scanf("%d", &indexValue);
      if (indexValue > 0) {
        sudokuValue[i] = 1;
        firstSudoku[i] = 1;
      } else {
        firstSudoku[i] = 0;
        sudokuValue[i] = 0;
      }
    }
  }
  fclose(stdin);
  freopen("CON", "r", stdin);  //到这一步就有了完成的数独盘,接下来给数独挖洞

  generateSudoku(m,sudokuValue);
  drawboard(2 * m, sudokuValue);
  printf("\n");
  printf("need help?(y/n)");
  scanf("%c",&b);
  if(b == 'y'){
    drawboard(2 * m, firstSudoku);
  }
  getchar();
  return 0;
}