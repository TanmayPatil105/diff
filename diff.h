#include <stdio.h>

int checkFiles(char *file1,char *file2);

int NoOfLines(char* file);

void getFileCreationTime(char *path);

void diff(char *file1,char *file2,char *arg);

void LinesToArray(char **arr1,char **arr2,int l1,int l2,FILE *fptr1,FILE *fptr2);

void LCSmatrix(char **arr1,char **arr2,int **LCS,int **BACK,int l1,int l2);

void NormalDiff(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y);

void UnionDiff(char *file1,char *file2,char **arr1,char **arr2,int **LCS,int **BACK,int x,int y);

void ColorDiff(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y);

void ContextDiff(char *file1,char *file2,char **arr1,char **arr2,int **LCS,int **BACK,int x,int y);

void printhelp();