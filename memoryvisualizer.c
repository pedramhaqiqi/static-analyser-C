#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 

#define MAX_LINE_LENGHT 100

/*function structs*/
typedef struct functions{

}function;

/*program error handler*/
void errors(int argc, char **argv){
    FILE *file = fopen(argv[1], "r");

    if (file == NULL){
        perror("fopen");
        exit(1);
    }
    fclose(file);
}

/*tokenizer functions*/

int isemptyline(char* line){
    //return 0 if line is not empty, 1 otherwise
    while(*line){
        if(!isspace(*line)){
            return 1;
        }
        line++;
    }
    return 0;
}

//strdup to duplicate string on heap var = strdup(string)
// strtok_r()

int isfunctionstart(char* line){
    //this function will return 1 if line contains a function declaration and 0 otherwise
}

char* fileoneline(FILE *fp){
    
    long size;
    char *oneline;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp); //visit the end of file and announce byte;
    rewind(fp); //return to beginning
    oneline = (char*)malloc(size + 1);
    fread(oneline, size, 1, fp); //mash all of file into one line
    oneline[size] = 0; //set end of string delimiter

    return oneline;
}


/*main function*/
int main(int argc, char**argv){
    
    FILE* fp =  fopen(argv[1], "r");
    char line[MAX_LINE_LENGHT+1];
    char* token;
    char* copyof;
    char* filestr;

    errors(argc, argv);

    filestr = fileoneline(fp);
    
    copyof = strdup(filestr);
    while(token  = strtok_r(copyof,"(", &copyof))
    {
        if (isemptyline(token) == 1){
             printf("token: %s\n", token);
        }
    } 
    fclose(fp);
    return 0;
}