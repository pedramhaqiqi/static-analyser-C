#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 

#define MAX_LINE_LENGHT 100
#define MAX_TYPE_LENGHT 30
#define MAX_NAME_LENGHT 30
#define MAX_VAR_LENGHT 30

/*structs and linked lists*/

typedef struct variable{
    char size;
    char name[MAX_VAR_LENGHT];
    char type[MAX_TYPE_LENGHT]; 
    struct variable *next;
}VAR;

typedef struct functions{
    int totallines;
    int totalvar;
    char name[MAX_NAME_LENGHT];
    VAR *head;    //local variables
}FUN;

void LLV_inserthead(LL_VAR **head, )

VAR* createnewvar(char* size, char* name, char* type){
    VAR *variable = malloc(sizeof(VAR));
    strcpy(variable->name, name);
    strcpy(variable->type, type);
    strcpy(variable->size, size);
    return variable;
}

VAR* append(VAR *head, char* size, char* name, char* type){
    // Returns new head after inserting newvar
    VAR* newvar = createnewvar(size, name, type);
    newvar->next = head;
    return newvar;
}
/*string manipulators*/


void remove_newline(char* string) {
    int i = 0;
    while (string[i] != 0){
        if (string[i] == '\n'){
            string[i] = ' ';
        }
        i++;
    }
}

void remove_space(char* string){
    int i = 0;
    while (string[i] != 0){
        if (string[i] == ' '){
            string[i] = ;
        }
        i++;
    }
}
/*validators*/
int var_assignment_validator(char* line){
    int t = 0;
    char eql = '=';
    if (strchr(line,eql)){
        t = 1;
    }
    return t;
}

int alloc_validator(char* line){
    int t = 0;
    char mall[] = "malloc(";
    char call[] = "calloc(";
    if (strstr(line, mall)){
        t = 1;
    }
    else if(strstr(line, call)){
        t = 1;
    }
    return t;

}
int ifloops_validator(char* line){
    int t = 0;
    char ifword[] = "if (";
    char forword[] = "for(";
    char whileword[] = "while(";

    if (strstr(line, ifword)){
        t = 1;
    }
    else if(strstr(line, forword)){
        t = 1;
    }
    else if(strstr(line, whileword)){
        t = 1;
    }
    return t;
}

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
VAR* var_parse_append(VAR* head, char* line){

    char size[MAX_NAME_LENGHT];
    char name[MAX_NAME_LENGHT];
    char type[MAX_TYPE_LENGHT];

    if(strstr(line, "int")){

    }else if(strstr(line, "char")){

    }else if(strstr(line, "float")){

    }else if(strstr(line, "int*")){

    }else if(strstr(line,"char*")){

    }else if(strstr(line,"float*")){

    }else if(strstr(line,"int[")){

    }else if(strstr(line,"char[")){

    }else if(strstr(line,"float[")){

    }


    return append(head, size, name, type);
}


/*
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
*/
/*main function*/
int main(int argc, char**argv){
    FILE* fp =  fopen(argv[1], "r");

    LL_VAR *list_var_head = NULL;



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