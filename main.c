#include "diff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
    FILE *fptr1,*fptr2; 
    //getFileCreationTime(argv[1]);
    if(argc==1){
        printf("diff: missing operand after 'diff'\ndiff: Try './diff --help' for more information.\n");
        return 1;
    }
    if(strcmp(argv[1], "--help") == 0) {
	printhelp();
        return 1;
	}
    else if(argc==3 || argc==4){
        if(checkFiles(argv[1],argv[2])){
            return 1;
        }
        diff(argv[1],argv[2],argv[3]);
    }
    //printf("%d ",NoOfLines(argv[1]));
    //printf("%d ",NoOfLines(argv[2]));
}
