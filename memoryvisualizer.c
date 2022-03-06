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
char* isfunctionstart(char* start, char* line){
    //return the pointer to the first function {
    
    start = strchr(line, '{');
    return start;
}

void remove_newline(char* string) {
    int i = 0;
    while (string[i] != 0){
        if (string[i] == '\n'){
            string[i] = ' ';
        }
        i++;
    }
}

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

void tokenizer_functions(char* filestr){
    
    char line[MAX_LINE_LENGHT+1];
    char* token;
    char* copyof;
    
    copyof = strdup(filestr);
    while((token = strtok_r(copyof,"{}", &copyof)))
    {
        
        printf("token: %s\n", token);
    } 
    
}

void tokenizer_global(char *line){
    n

}
//strdup to duplicate string on heap var = strdup(string)
// strtok_r()
char* fileoneline(FILE *fp){
    
    long size;
    char *onestring;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp); //visit the end of file and announce byte;
    rewind(fp); //return to beginning
    onestring = (char*)malloc(size + 1);
    fread(onestring, size, 1, fp); //mash all of file into one string
    onestring[size] = 0; //set end of string delimiter

    return onestring;
}

/*main function*/
int main(int argc, char**argv){
    FILE* fp =  fopen(argv[1], "r");
    char* filestr;



    /*
    filestr = fileoneline(fp); //put file in one string
    
    char *file_line_1 = malloc(sizeof(char) * (strlen(filestr) + 1));
    strncpy(file_line_1, filestr, strlen(filestr)+ 1);

    remove_newline(file_line_1);
    errors(argc, argv);
    // until here we read the file content into one string
    // and de-newlined the string

    char *start = malloc(sizeof(char)); //free me
    //tokenizer_functions(file_line_1);
    

    fclose(fp);
    free(file_line_1);
    */

    return 0;
}