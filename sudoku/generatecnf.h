#ifndef GENERATECNF_H
#define GENERATECNF_H
int constraintOne(FILE* file,int m); 
int combine(FILE* file,int m, int row, int column, int columnOrRow);
int constraintTwo(FILE* file,int m); 
int constraintThree(FILE* file,int m);
int transfer(int* sudokuValue, int m, int seleceLitNum);
#endif