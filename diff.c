#include "diff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_LINES 128
#define MAX_CHARS 128


int min(int a,int b,int c){
    if(a < b && a < c){
        return a;
    }
    if(b < a && b < c){
        return b;
    }
    return c;
}


//DIFF FUCNTION

void diff(char *file1,char *file2,char *arg){


    FILE *fptr1=fopen(file1,"r");
    FILE *fptr2=fopen(file2,"r");
    int l1=NoOfLines(file1);
    int l2=NoOfLines(file2);

    if(l1 < 0 || l2 < 0){
        return;
    }
    if(l1 > MAX_LINES || l2 > MAX_LINES){
        printf("File too Large\n");
        return;
    }
    
    //ARRAYS TO STORE LINES OF FILES

    char **arr1=(char**)malloc(sizeof(char*)*l1);
    char **arr2=(char**)malloc(sizeof(char*)*l2);


    //LINES TO ARRAY

    LinesToArray(arr1,arr2,l1,l2,fptr1,fptr2);


    //LCS MATRIX 

    int **LCS=(int**)malloc(sizeof(int*)*(l1+1));
    for(int i=0;i<=l1;i++){
        LCS[i]=(int*)malloc(sizeof(int)*(l2+1));
    }

    //BACK MATRIX FOR BACKTRACKING

    int **BACK=(int**)malloc(sizeof(int*)*(l1+1)); 
    for(int i=0;i<=l1;i++){
        BACK[i]=(int*)malloc(sizeof(int)*(l2+1));
    }
    
    //GENERATING LCS AND BACK MATRIX

    LCSmatrix(arr1,arr2,LCS,BACK,l1,l2);


    //printLCS(LCS,l1,l2);
    //printBACK(BACK,l1,l2);

    //BACKTRACKING THE BACK MATRIX AND PRINTING THE OUTPUT
    int x=l1,y=l2;

    if(arg==NULL){
        NormalDiff(arr1,arr2,LCS,BACK,x,y);
    }
    else if(strcmp(arg,"--normal")==0){
        NormalDiff(arr1,arr2,LCS,BACK,x,y);
    }
    else if(strcmp(arg,"--color")==0){
        ColorDiff(arr1,arr2,LCS,BACK,x,y);
    }
    else if(strcmp(arg,"-q")==0){
        if(LCS[l1][l2]!=0){
            printf("Files %s and %s differ\n",file1,file2);
        }
    }
    else if(strcmp(arg,"-s")==0){
        if(LCS[l1][l2]==0){
            printf("Files %s and %s are identical\n",file1,file2);
        }
        else {
            NormalDiff(arr1,arr2,LCS,BACK,x,y);
        }
    }
    else if(strcmp(arg,"-u")==0){
        UnionDiff(file1,file2,arr1,arr2,LCS,BACK,x,y);
    }
    else if(strcmp(arg,"-c")==0){
        ContextDiff(file1,file2,arr1,arr2,LCS,BACK,x,y);
    }
    free(arr1);
    free(arr2);
    free(LCS);
    free(BACK);
    fclose(fptr1);
    fclose(fptr2);
}


int checkFiles(char *file1,char *file2){
    FILE *fptr1,*fptr2;

    fptr1=fopen(file1,"r");
    fptr2=fopen(file2,"r");

    //FILES DOES NOT EXIST
    if(fptr1==NULL && fptr2==NULL){
        printf("diff: %s: No such file or directory",file1);
        printf("\ndiff: %s: No such file or directory\n",file2);
        return 1;
    }
    else if(fptr1==NULL){
        printf("diff: \'%s\': No such file or directory\n",file1);
        return 1;
    }
    else if(fptr2==NULL){
        printf("diff: '%s': No such file or directory\n",file2);
        return 1;
    }
    fclose(fptr1);
    fclose(fptr2);
    return 0;
}

int NoOfLines(char* file){
    FILE *fptr = fopen(file, "r");
    
    if (fptr==NULL) {
        return -1;
    }
    int ch;
    int count=0; //NO OF LINES
    while ((ch = fgetc(fptr)) != EOF) {
        if(!count){
            count = 1;
        }
        if ((char)ch == '\n'){
            ++count;
        } 
    }
    fclose(fptr);
    return count;
}

void printLCS(int **LCS,int l1,int l2){
    for(int x=0;x<=l1;x++){
        for(int y=0;y<=l2;y++){
            printf("%d ",LCS[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

void printBACK(int **BACK,int l1,int l2){
    for(int x=0;x<=l1;x++){
        for(int y=0;y<=l2;y++){
            printf("%d ",BACK[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

void LinesToArray(char **arr1,char **arr2,int l1,int l2,FILE *fptr1,FILE *fptr2){
    for(int i = 0; i < l1; i++) {
        arr1[i] = (char*)malloc(sizeof(char)*MAX_CHARS);
    }

    for(int i = 0; i < l2; i++) {
        arr2[i] = (char*)malloc(sizeof(char)*MAX_CHARS);
    }
    int i=0;
    while(fgets(arr1[i], MAX_CHARS, fptr1)) 
	{
        arr1[i][strlen(arr1[i]) - 1] = '\0';
        i++;
    }
    i=0;
    while(fgets(arr2[i], MAX_CHARS, fptr2)) 
	{
        arr2[i][strlen(arr2[i]) - 1] = '\0';
        i++;
    }
}

void LCSmatrix(char **arr1,char **arr2,int **LCS,int **BACK,int l1,int l2){
    int x=0,y=0;
    LCS[0][0]=0;
    BACK[0][0]=0;

    //FOR LCS MATRIX 

    for(x=1;x<=l1;x++){
        LCS[x][0]=LCS[x-1][0] + 1;
    }
    for(y=1;y<=l2;y++){
        LCS[0][y]=LCS[0][y-1] + 1;
    }

    ////FOR BACK MATRIX
    for(x=1;x<=l1;x++){
        BACK[x][0]=0;
    }
    for(y=1;y<=l2;y++){
        BACK[0][y]=0;
    }
    for(x=1;x<=l1;x++){
        for(y=1;y<=l2;y++){
            int sub=0;
            int del,add,change,minim;
            if(strcmp(arr1[x-1],arr2[y-1])==0){
                sub=0;
            }
            else {
                sub=1;
            }

            del=LCS[x-1][y] + 1;
            add=LCS[x][y-1] + 1;
            change=LCS[x-1][y-1] + sub;

            minim=min(del,add,change);
            LCS[x][y]=minim;

            
            if(minim==del){
                BACK[x][y]=1;
            }
            else if(minim==add){
                BACK[x][y]=2;
            }
            else if(minim==change){
                BACK[x][y]=3;
            }
        }
    }
}


//  FUNCTIONALITIES OF DIFF COMMANDS

void NormalDiff(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    if(x==0 && y==0){
        return;
    }
    if(BACK[x][y]==3){
        if(LCS[x][y]!=LCS[x-1][y-1]){
            NormalDiff(arr1,arr2,LCS,BACK,x-1,y-1);
            printf("%dc%d\n< %s\n---\n> %s\n",x,y,arr1[x-1],arr2[y-1]);
        }
        else {
            NormalDiff(arr1,arr2,LCS,BACK,x-1,y-1);
        }
    }
    else if(BACK[x][y]==2){
        if(LCS[x][y]!=LCS[x][y-1]){
            NormalDiff(arr1,arr2,LCS,BACK,x,y-1);
            printf("%da%d\n< %s\n",x,y,arr2[y-1]);
        }
        else {
            NormalDiff(arr1,arr2,LCS,BACK,x,y-1);
        }
    }
    else if(BACK[x][y]==1){
        if(LCS[x][y]!=LCS[x-1][y]){
            NormalDiff(arr1,arr2,LCS,BACK,x-1,y);
            printf("%dd%d\n< %s\n",x,y,arr1[x-1]);
        }
        else {
            NormalDiff(arr1,arr2,LCS,BACK,x-1,y);
        }
    }
    else {
        NormalDiff(arr1,arr2,LCS,BACK,x-1,y-1);
    }
}

void ColorDiff(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    if(x==0 && y==0){
        return;
    }
    if(BACK[x][y]==3){
        if(LCS[x][y]!=LCS[x-1][y-1]){
            ColorDiff(arr1,arr2,LCS,BACK,x-1,y-1);
            printf("\033[0;36m%dc%d \033[0;37m \n\033[0;31m< %s\n\033[0;37m---\n\033[0;32m> %s\n",x,y,arr1[x-1],arr2[y-1]);
        }
        else {
            ColorDiff(arr1,arr2,LCS,BACK,x-1,y-1);
        }
    }
    else if(BACK[x][y]==2){
        if(LCS[x][y]!=LCS[x][y-1]){
            ColorDiff(arr1,arr2,LCS,BACK,x,y-1);
            printf("\033[0;36m%da%d\n\033[0;32m< %s\n\033[0;37m",x,y,arr2[y-1]);
        }
        else {
            ColorDiff(arr1,arr2,LCS,BACK,x,y-1);
        }
    }
    else if(BACK[x][y]==1){
        if(LCS[x][y]!=LCS[x-1][y]){
            ColorDiff(arr1,arr2,LCS,BACK,x-1,y);
            printf("\033[0;36m%dd%d\n\033[0;31m< %s\n\033[0;37m",x,y,arr1[x-1]);
        }
        else {
            ColorDiff(arr1,arr2,LCS,BACK,x-1,y);
        }
    }
    else {
        ColorDiff(arr1,arr2,LCS,BACK,x-1,y-1);
    }
}

void UnionDiff1(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    if(x==0 && y==0){
        return;
    }
    if(BACK[x][y]==3){
        if(LCS[x][y]!=LCS[x-1][y-1]){
            UnionDiff1(arr1,arr2,LCS,BACK,x-1,y-1);
            printf("-%s\n+%s\n",arr1[x-1],arr2[y-1]);
        }
        else {
            UnionDiff1(arr1,arr2,LCS,BACK,x-1,y-1);
            printf(" %s\n",arr1[x-1]);
        }
    }
    else if(BACK[x][y]==2){
        if(LCS[x][y]!=LCS[x][y-1]){
            UnionDiff1(arr1,arr2,LCS,BACK,x,y-1);
            printf("+%s\n",arr2[y-1]);
        }
        else {
            UnionDiff1(arr1,arr2,LCS,BACK,x,y-1);
            printf(" %s\n",arr1[x-1]);
        }
    }
    else if(BACK[x][y]==1){
        if(LCS[x][y]!=LCS[x-1][y]){
            UnionDiff1(arr1,arr2,LCS,BACK,x-1,y);
            printf("-%s\n",arr1[x-1]);
        }
        else {
            UnionDiff1(arr1,arr2,LCS,BACK,x-1,y);
            printf(" %s\n",arr1[x-1]);
        }
    }
    else {
        UnionDiff1(arr1,arr2,LCS,BACK,x-1,y-1);
        printf(" %s\n",arr1[x-1]);
    }
}

void UnionDiff(char *file1,char *file2,char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    int l1=x;
    int l2=y;
    char t1[100] = "";
    char t2[100] = "";
    struct stat b1;
    struct stat b2;
    stat(file1,&b1);
    stat(file2,&b2);
    strftime(t1, 100, "%Y-%m-%d %H:%M:%s +0530\n", localtime( &b1.st_mtime));
    strftime(t2, 100, "%Y-%m-%d %H:%M:%s +0530\n", localtime( &b2.st_mtime));
    printf("--- %s      %s",file1,t1);
    printf("+++ %s      %s",file2,t2);
    printf("@@ -1,%d +1,%d @@\n",l1,l2);
    UnionDiff1(arr1,arr2,LCS,BACK,x,y);
    printf("\\ No newline at end of file\n");
}

void ContextDiff1(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    if(x==0 && y==0){
        return;
    }
    if(BACK[x][y]==3){
        if(LCS[x][y]!=LCS[x-1][y-1]){
            ContextDiff1(arr1,arr2,LCS,BACK,x-1,y-1);
            printf("! %s\n",arr1[x-1]);
        }
        else {
            ContextDiff1(arr1,arr2,LCS,BACK,x-1,y-1);
            printf("  %s\n",arr1[x-1]);
        }
    }
    else if(BACK[x][y]==2){
        if(LCS[x][y]!=LCS[x][y-1]){
            ContextDiff1(arr1,arr2,LCS,BACK,x,y-1);
        }
        else {
            ContextDiff1(arr1,arr2,LCS,BACK,x,y-1);
        }
    }
    else if(BACK[x][y]==1){
        if(LCS[x][y]!=LCS[x-1][y]){
            ContextDiff1(arr1,arr2,LCS,BACK,x-1,y);
            printf("- %s\n",arr1[x-1]);
        }
        else {
            ContextDiff1(arr1,arr2,LCS,BACK,x-1,y);
        }
    }
    else {
        ContextDiff1(arr1,arr2,LCS,BACK,x-1,y-1);
        printf(" %s\n",arr1[x-1]);
    }
}

void ContextDiff2(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    if(x==0 && y==0){
        return;
    }
    if(BACK[x][y]==3){
        if(LCS[x][y]!=LCS[x-1][y-1]){
            ContextDiff2(arr1,arr2,LCS,BACK,x-1,y-1);
            printf("! %s\n",arr2[y-1]);
        }
        else {
            ContextDiff2(arr1,arr2,LCS,BACK,x-1,y-1);
            printf("  %s\n",arr2[y-1]);
        }
    }
    else if(BACK[x][y]==2){
        if(LCS[x][y]!=LCS[x][y-1]){
            ContextDiff2(arr1,arr2,LCS,BACK,x,y-1);
            printf("+ %s\n",arr2[y-1]);
        }
        else {
            ContextDiff2(arr1,arr2,LCS,BACK,x-1,y-1);
        }
    }
    else if(BACK[x][y]==1){
        if(LCS[x][y]!=LCS[x-1][y]){
            ContextDiff2(arr1,arr2,LCS,BACK,x-1,y);
        }
        else {
            ContextDiff2(arr1,arr2,LCS,BACK,x-1,y);
        }
    }
    else {
        ContextDiff2(arr1,arr2,LCS,BACK,x-1,y-1);
        printf(" %s\n",arr2[y-1]);
    }
}

void ContextDiff(char *file1,char *file2,char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    char t1[100] = "";
    char t2[100] = "";
    int l1=x;
    int l2=y;
    struct stat b1;
    struct stat b2;
    stat(file1,&b1);
    stat(file2,&b2);
    strftime(t1, 100, "%Y-%m-%d %H:%M:%s +0530\n", localtime( &b1.st_mtime));
    strftime(t2, 100, "%Y-%m-%d %H:%M:%s +0530\n", localtime( &b2.st_mtime));
    printf("*** %s      %s",file1,t1);
    printf("--- %s      %s",file2,t2);
    printf("***************\n*** 1,%d ****\n",l1);
    ContextDiff1(arr1,arr2,LCS,BACK,x,y);
    printf("\\ No newline at end of file\n");
    printf("--- 1,%d ----\n",l2);
    ContextDiff2(arr1,arr2,LCS,BACK,x,y);
    printf("\\ No newline at end of file\n");
}



void printhelp(){
    FILE *fptr;
    fptr=fopen("help.txt","r");
    char ch;
    while((ch=fgetc(fptr))!=EOF){
        printf("%c",ch);
    }
    printf("\n");
}
