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



int main(int argc, char ** argv){

    char*s = "void function()";
    char*b = "void function(  )";
    char*c = "void function(int a, char c )"; 

    printf("%s\n", get_function_params(s));
    printf("%s\n", get_function_params(b));
    printf("%s\n", get_function_params(c));

    return 0;
}