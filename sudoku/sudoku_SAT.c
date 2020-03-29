//单独的数独游戏
#include <stdio.h>
#include <stdlib.h>

int main() {
  int op = 1;
  while(op){
    system("cls");
    printf("1.binary puzzle game\n2.SAT solver\n3.exit\n");
    printf("please choose function:(1/2/3)");
    scanf("%d", &op);
    if(op == 1){
      system("generateSudoku.c");
    }else if(op == 2){
      system("app.exe");
      printf("please check the result in file 'answer.res'\n");
    }else{
      op = 0;
    }
  }
  return 0;
}