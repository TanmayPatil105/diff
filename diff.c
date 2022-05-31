#include "diff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 128
#define MAX_CHARS 128


#define max a>b?a:b

int min(int a,int b,int c){
    if(a < b && a < c){
        return a;
    }
    if(b < a && b < c){
        return b;
    }
    return c;
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

void BackTrackR(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    if(x==0 && y==0){
        return;
    }
    if(BACK[x][y]==3){
        if(LCS[x][y]!=LCS[x-1][y-1]){
            BackTrackR(arr1,arr2,LCS,BACK,x-1,y-1);
            printf("%dc%d\n< %s\n---\n> %s\n",x,y,arr1[x-1],arr2[y-1]);
        }
        else {
            BackTrackR(arr1,arr2,LCS,BACK,x-1,y-1);
        }
    }
    else if(BACK[x][y]==2){
        if(LCS[x][y]!=LCS[x][y-1]){
            BackTrackR(arr1,arr2,LCS,BACK,x,y-1);
            printf("%da%d\n< %s\n",x,y,arr2[y-1]);
        }
        else {
            BackTrackR(arr1,arr2,LCS,BACK,x-1,y-1);
        }
    }
    else if(BACK[x][y]==1){
        if(LCS[x][y]!=LCS[x-1][y]){
            BackTrackR(arr1,arr2,LCS,BACK,x-1,y);
            printf("%dd%d\n< %s\n",x,y,arr1[x-1]);
        }
        else {
            BackTrackR(arr1,arr2,LCS,BACK,x-1,y);
        }
    }
    else {
        BackTrackR(arr1,arr2,LCS,BACK,x-1,y-1);
    }
}

void BackTrack(char **arr1,char **arr2,int **LCS,int **BACK,int x,int y){
    while(x!=0 && y!=0){
        if(BACK[x][y]==3){
            if(LCS[x][y]!=LCS[x-1][y-1]){
                printf("%dc%d\n< %s\n---\n> %s\n",x,y,arr1[x-1],arr2[y-1]);
                x=x-1;
                y=y-1;
            }
            else {
                x=x-1;
                y=y-1;
            }
        }
        else if(BACK[x][y]==2){
            if(LCS[x][y]!=LCS[x][y-1]){
                printf("%da%d\n< %s\n",x,y,arr2[y-1]);
                x=x;
                y=y-1;
            }
            else {
                x=x;
                y=y-1;
            }
        }
        else if(BACK[x][y]==1){
            if(LCS[x][y]!=LCS[x-1][y]){
                printf("%dd%d\n< %s\n",x,y,arr1[x-1]);
                x=x-1;
                y=y;
            }
            else {
                x=x-1;
                y=y;
            }
        }
        else {
            x=x-1;
            y=y-1;
        }
    }
}

void diff(char *file1,char *file2){


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
    BackTrackR(arr1,arr2,LCS,BACK,x,y);

    free(arr1);
    free(arr2);
    free(LCS);
    free(BACK);
    fclose(fptr1);
    fclose(fptr2);
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
