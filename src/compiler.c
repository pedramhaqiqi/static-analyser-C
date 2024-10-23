#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 

#define MAX_LINE_LENGTH 100
#define MAX_TYPE_LENGTH 30
#define MAX_NAME_LENGTH 30
#define MAX_VAR_LENGTH 30

void strip_whitespace(char *line) {
  char temp[strlen(line) + 1];
  int i;
  for (i = strlen(line) - 1; i >= 0; i--) {
    if (!isspace(line[i])) break;
  }
  line[i + 1] = '\0';
  for (i = 0; i < strlen(line); i++) {
    if (!isspace(line[i])) break;
  }
  memmove(temp, &line[i], strlen(&line[i]) + 1);
  for (i = 0; i <= strlen(line); i++) {
    line[i] = temp[i];
  }
}

char *get_type(char *line) {
  // Returns the type of the line as a char *
  // for pointers and [] need to remove space
  char *type = malloc(sizeof(char) * MAX_TYPE_LENGTH);
  char fmt[MAX_TYPE_LENGTH];
  char temp[strlen(line) + 1];
  char *types_native[] = {"void", "int", "float", "char"};
  char *types_ext[] = {" %[*]", "%[ ]"};

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
char *get_function_name(char *line) {
  // Assume line contains a function header

  char *name = malloc(sizeof(char) * MAX_NAME_LENGTH);
  char type[MAX_TYPE_LENGTH];
  char fmt[MAX_TYPE_LENGTH];

  strcpy(type, get_type(line));
  sprintf(fmt, "%s%%[^(]", type);
  sscanf(line, fmt, name);

  return name;
}


char *get_function_params(char* line){
    char *index_open;
    char *index_close;
    char *parameters = calloc(MAX_NAME_LENGTH ,sizeof(char));
   
    index_open = strchr(line, '(');
    index_close = strchr(line, ')');
   
    strncpy(parameters, index_open + 1, index_close - index_open - 1);
    strip_whitespace(parameters);

    if (strlen(parameters) > 0){
        strncpy(parameters, index_open + 1, index_close - index_open - 1);
        return parameters;
    }
    
   free(parameters);
   return NULL;
}
char *rstrchr(char *str, char c) {
    //(reads from right) Returns pointer to c in str, and null if it doesnt exist
  for (int i = strlen(str)-1; i >= 0; --i) {
    if (str[i] == c) return &str[i];
  }
  return NULL;
}

char *alloc_parser(char *line) {
  char *allocs[] = {"malloc(", "calloc("};
  char *alloc_ptr;
  char *sc_ptr;
  char *cm_ptr;
  char *param = malloc(sizeof(char) * 100);
  for (int i = 0; i < 2; i++) {
    if ((alloc_ptr = strstr(line, allocs[i])) != NULL &&
        (sc_ptr = strchr(line, ';')) != NULL) {
      strncpy(param, alloc_ptr + strlen(allocs[i]),
              rstrchr(line, ')') - alloc_ptr);
      //Pedrams added feature to replace Callocs comma with *, 
      //i.e lenght, sizeof(int) ---> lenght * sizeof(int)
      if((cm_ptr = strchr(param, ','))){
        printf("%s\n",cm_ptr);
        int index = -1 * (int)(param - cm_ptr);
        printf("%d\n",index);
        param[index] = '*';
      }
      return param;
    }
  }
  free(param);
  return NULL;
}
char* get_array_size(char* name){
    char* size = calloc(MAX_NAME_LENGTH ,sizeof(char));
    char* op_ptr;
    char* cl_ptr;
    op_ptr = strchr(name, '[');
    cl_ptr = strchr(name, ']');

    strncpy(size, op_ptr + 1, cl_ptr - op_ptr - 1);
    strip_whitespace(size);
    if (strlen(size)>0){
        return size;
    }
    free(size);
    return NULL;
}

char* get_array_name(char* name){
    char* arryname = calloc(MAX_NAME_LENGTH ,sizeof(char));
    char* op_ptr;
    op_ptr = strchr(name, '[');
    strncpy(arryname, name, op_ptr - name);
    strip_whitespace(arryname);
    return arryname;
}



int get_argument_count(char *string){
    
    char* op_ptr;
    char* cl_ptr;
    char* token;
   
    int i = 0;
    char* arryname = calloc(MAX_NAME_LENGTH ,sizeof(char));
   
    op_ptr = strchr(string, '{');
    cl_ptr = strchr(string, '}');

    strncpy(arryname, op_ptr + 1, cl_ptr - op_ptr - 1);
    strip_whitespace(arryname);
    if(strlen(arryname) == 0){
        return 0;
    }

    while ((token = strtok_r(arryname, ",", &arryname))){
        i = i + 1;
    } 

    free(arryname);
    return i;
}

int main(int argc, char ** argv){
    
    char*s = "name0[6] = {1,2,3,4,5}";
    char*s1 = "name1[7] = {   }";
  

    
    printf("%d\n",get_argument_count(s));
    
    printf("%d\n",get_argument_count(s1));

   
    return 0;
}