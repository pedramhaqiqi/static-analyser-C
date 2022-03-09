#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 

#define MAX_LINE_LENGTH 100
#define MAX_TYPE_LENGTH 30
#define MAX_NAME_LENGTH 30
#define MAX_VAR_LENGTH 30

/*structs and linked lists*/

typedef struct variable{
    char size[MAX_NAME_LENGTH];
    char name[MAX_VAR_LENGTH];
    char type[MAX_TYPE_LENGTH]; 
    struct variable *next;
}VAR;

typedef struct functions{
    int totallines;
    int totalvar;
    char name[MAX_NAME_LENGTH];
    VAR *head;    //local variables
}FUN;

typedef struct global{
    VAR *head;
}GLB;

typedef struct heap{
    VAR *head;
}HEAP;

typedef struct rodata{
    VAR *head;
} ROD;

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

char* remove_space(char* string){
    char* new= malloc(sizeof(char) * (strlen(string)+1)) ;
    char* copyof;
    char* token;
    copyof = strdup(string);

    while((token = strtok_r(copyof," ", &copyof))){
        strncat(new, token, strlen(token));
    }
    new[strlen(new)] = 0;
    return new;
}

char *remove_semicolon(char* string){
    char* new= malloc(sizeof(char) * (strlen(string)+1));
    char* copyof;
    char* token;
    copyof = strdup(string);

    while((token = strtok_r(copyof,";", &copyof))){
        strncat(new, token, strlen(token));
    }
    new[strlen(new)] = 0;
    return new;
}
char *remove_semicolon_space(char* string){
    char* new= malloc(sizeof(char) * (strlen(string)+1)) ;
    char* copyof;
    char* token;
    copyof = strdup(string);

    while((token = strtok_r(copyof,";", &copyof))){
        strncat(new, token, strlen(token));
    }
    new[strlen(new)] = 0;
    return remove_space(new);
}
/*validators*/
int has_eql(char* line){
    int t = 0;
    char eql = '=';
    if (strchr(line,eql)){
        t = 1;
    }
    return t;
}

int alloc_validator(char* line){
    //return 0 for not *alloc, 1 for malloc, 2 for calloc
    int t = 0;
    char mall[] = "malloc(";
    char call[] = "calloc(";
    if (strstr(line, mall)){
        t = 1;
    }
    else if(strstr(line, call)){
        t = 2;
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

int comma_validator(char* line){
    int t = 0;
    char comma = ',';
    if (strchr(line,comma)){
        t = 1;
    }
    return t;
}

int has_colon(char* line){
    int t = 0;
    if(strchr(line, ';')){
        t = 1;
    }
    return t;
}
int checks_next_line(FILE* fp){
    //this function checks the next line of {\n without compromising
    //the file handler, return 1 if found and 0 otherwise
    char returnval[256];

    fgets(returnval, 256, fp);
    fseek(fp, -1 * strlen(returnval), SEEK_CUR);

    if (strcmp(returnval, "{\n") == 0){
        return 1;
    }
    
    return 0;
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

int has_type(char* line){
    // Retrun 1 if line has valid type declaration
    char *token;
    char type[MAX_NAME_LENGTH];
    char* copyof = strdup(line);
    token = strtok_r(copyof, " ", &copyof);
    char* types_native[] = {"void","int","float", "char"};
    char* types_ext[] = {"*", ""};
    for (int i=0; i<4; i++) {
        for (int j=0; j<2; j++) {
            strcpy(type, types_native[i]);
            strcat(type, types_ext[j]);
            if(strcmp(token, type) == 0){
                return 1;
            }
        }
    }
    return 0;
}


char* alloc_parser(char* line){
    int id = alloc_validator(line);
    char* ptr_1 = NULL;
    char* ptr_2 = NULL;
    char* parameter = malloc(sizeof(char)* 100);
    if(id == 0){
        return NULL;
    }else if(id == 1){
        ptr_1 = strstr(line, "malloc(");
        ptr_2 = strstr(line, ";");
        strncpy(parameter, ptr_1 + 7, line + strlen(line) - ptr_1 - 9);
    } else if(id == 2){
        ptr_1 = strstr(line, "calloc(");
        ptr_2 = strstr(line, ";");
        strncpy(parameter, ptr_1 + 7, line + strlen(line) - ptr_1 - 9);
    }
    return parameter;
}


char* get_type(char* line) {
  // Returns the type of the line as a char *
  // for pointers and [] need to remove space
  char* type = malloc(sizeof(char) * MAX_TYPE_LENGTH);
  char fmt[MAX_TYPE_LENGTH];
  char temp[strlen(line) + 1];
  char* types_native[] = {"void", "int", "float", "char"};
  char* types_ext[] = {" %[*]", "%[ ]"};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 2; j++) {
      strcpy(type, types_native[i]);
      strcat(type, types_ext[j]);
      sprintf(fmt, "%s", type);
      if (sscanf(line, fmt, temp) > 0) {
      	strcpy(type, types_native[i]);
      	strcat(type, j ? "" : "*");
        return type;
      }
    }
  }
  free(type);
  return NULL;
}

char* get_function_name(char* line, FILE* fp){
    //returns null if not a function 

    char* name = malloc(sizeof(char) * MAX_NAME_LENGTH);
    char type[MAX_TYPE_LENGTH];
    char fmt[MAX_TYPE_LENGTH];

    if (has_type(line) == 0){
        return NULL;
    }
    if (has_eql(line) == 1){
        return NULL;
    }
    if (checks_next_line(fp) == 0){
        return NULL;
    }
    
    strcpy(type, get_type(line));
    sprintf(fmt, "%s%%[^(]", type);
    sscanf(line, fmt, name);

    return name;  
}
// char* parse_by_type(char* string, char* format){
//     char* new = malloc(sizeof(char) * (strlen(string)+1)) ;
//     sscanf(string,format,new);
//     new[strlen(new)] = 0;
//     return new;
// }

VAR* parse_by_comma(VAR* head, char* string, char* size, char* type){
    char* copyof;
    char* token;
    copyof = strdup(string);

    while((token = strtok_r(copyof,",", &copyof))){
        head = append(head, size, token, type);
    }
    return head;
}

char* parse_by_eql(char* string){
    //Return the substring before the "="
    char* new = malloc(sizeof(char) * (strlen(string)+1)) ;
    char* copyof;
    char* token;
    copyof = strdup(string);

    token = strtok_r(copyof,"=", &copyof);
    strncat(new, token, strlen(token));
    
    new[strlen(new)] = 0;
    return new;
}

// VAR* var_parse_append(VAR* head, char* line){
//     //this function would run on the assumptions of:
//         //we know the line is a variable declaration
//         //1. ifloops validator passed 0
//         //2. function validator passed 0
//         //3. Knowing what scope it has global/local     
//     char size[MAX_NAME_LENGTH];
//     char name[MAX_NAME_LENGTH];
//     char type[MAX_TYPE_LENGTH];
//     char semi_parse[MAX_VAR_LENGTH];
//     char *no_space = remove_space(line);
//     if(strstr(no_space, "int[")){
//         if (has_eql(line) == 1){
//             if (alloc_validator(line) == 1){
//             }else{
//             }
//         }
//     }else if(strstr(no_space, "char[")){
//         if (has_eql(line) == 1){
//             if (alloc_validator(line) == 1){
//             }else{
//             }
//         }
//     }else if(strstr(no_space, "float[")){
//         if (has_eql(line) == 1){
//             if (alloc_validator(line) == 1){
//             }else{
//             }
//         }
//     }else if(strstr(no_space, "int*")){
//         if (has_eql(line) == 1){
//             if (alloc_validator(line) == 1){
//             }else{
//             }
//         }
//     }else if(strstr(no_space,"char*")){
//         if (has_eql(line) == 1){
//             if (alloc_validator(line) == 1){
//             }else{
//             }
//         }
//     }else if(strstr(no_space,"float*")){
//         if (has_eql(line) == 1){
//             if (alloc_validator(line) == 1)
//             }else{
//             }
//         }
//     }else if(strstr(no_space,"int")){
//             strcpy(size ,"sizeof(int)");
//             strcpy(type ,"int");
//             if (has_eql(line) == 1){ //checks for = in the line
//                 if (alloc_validator(line) == 1){
//                 //*alloc has been called
//                 }else if(comma_validator(line) == 1){
//                 //multiple assignment with = existing
//                     strcpy(semi_parse, parse_by_eql(parse_by_type(remove_semicolon(no_space))));
//                     head = parse_by_comma(head, semi_parse , size, type);
//                     return head;
//                 } 
//                 //single assignment with = existing
//                 strcpy(name,parse_by_eql(parse_by_type(remove_semicolon(no_space))));
//                 return append(head, size, name, type);
//             }else if(comma_validator(line) == 1){
//                 //multi assignment without = existing
//                 strcpy(semi_parse ,parse_by_type(remove_semicolon_space(line)));
//                 head = parse_by_comma(head, semi_parse, size, type);
//                 return head;
//             }
//             //single assignment without = existing 
//             strcpy(name ,parse_by_type(remove_semicolon(no_space)));
//             return append(head, size, name, type);
//     }else if(strstr(no_space,"char")){
//         if (has_eql(line) == 1){
//             if (alloc_validator(line) == 1){               
//             }else{
//             }
//         }
//     }else if(strstr(no_space,"float")){
//         if (has_eql(line) == 1){
//             if (alloc_validator(line) == 1){                
//             }else{
//             }
//         }
//     }else{
//         return NULL;
//     }
//     return append(head, size, name, type);
// }


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
    
    char line[MAX_LINE_LENGTH+1];
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
// int main(int argc, char**argv){
//     FILE* fp =  fopen(argv[1], "r");


    //fclose(fp);
    //free(file_line_1);

//     return 0;
// }
