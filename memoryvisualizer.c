#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 100
#define MAX_TYPE_LENGTH 50
#define MAX_NAME_LENGTH 50
#define MAX_VAR_LENGTH 50

/*structs and linked lists*/

typedef struct variable {
  char size[MAX_NAME_LENGTH];
  char name[MAX_VAR_LENGTH];
  char type[MAX_TYPE_LENGTH];
  struct variable *next;
} VAR;

typedef struct function {
  //  int totallines;
  //  int totalvar;
  char name[MAX_NAME_LENGTH];
  VAR *var_head;  // local variables
  struct function *next;
} FUN;

VAR *get_new_var(char *size, char *name, char *type) {
  VAR *variable = malloc(sizeof(VAR));
  strcpy(variable->name, name);
  strcpy(variable->type, type);
  strcpy(variable->size, size);
  variable->next = NULL;
  return variable;
}

VAR *append_var(VAR *head, char *size, char *name, char *type) {
  // Returns new head after inserting newvar
  VAR *newvar = get_new_var(size, name, type);
  newvar->next = head;
  return newvar;
}

FUN *get_new_fun(char *name) {
  FUN *res = malloc(sizeof(FUN));
  strcpy(res->name, name);
  res->var_head = NULL;
  res->next = NULL;
  return res;
}

FUN *append_fun(FUN *head, FUN *fun) {
  fun->next = head;
  return fun;
}

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

VAR *parse_by_comma(VAR *head, char *string, char *size, char *type) {
  char *copyof;
  char *token;
  copyof = strdup(string);

  while ((token = strtok_r(copyof, ",", &copyof))) {
    head = append_var(head, size, token, type);
  }
  return head;
}


/*program error handler*/
void errors(int argc, char **argv) {
  FILE *file = fopen(argv[1], "r");

  if (file == NULL) {
    perror("fopen");
    exit(1);
  }
  fclose(file);
}
/*tokenizer functions*/

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
  char *param = malloc(sizeof(char) * 100);
  for (int i = 0; i < 2; i++) {
    if ((alloc_ptr = strstr(line, allocs[i])) != NULL &&
        (sc_ptr = strchr(line, ';')) != NULL) {
      strncpy(param, alloc_ptr + strlen(allocs[i]),
              rstrchr(line, ')') - alloc_ptr);
      return param;
    }
  }
  free(param);
  return NULL;
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

char *parse_by_eql(char *string) {
  // Return the substring before the "="
  char *new = malloc(sizeof(char) * (strlen(string) + 1));
  char *copyof;
  char *token;
  copyof = strdup(string);

  token = strtok_r(copyof, "=", &copyof);
  strncat(new, token, strlen(token));

  new[strlen(new)] = 0;
  return new;
}

char *get_var_name(char *type, char *line) {
  char *names = calloc(MAX_NAME_LENGTH, sizeof(char));
  char *index_star;
  char *index_eql;
  char *index_semi;
  char *index_type;

  if ((index_semi = rstrchr(line, ';')) == NULL) {
      index_semi = line + strlen(line);
  }
  index_type = strstr(line, type) + strlen(type);
  if ((index_star = strchr(line, '*')) != NULL) {
    if ((index_eql = strchr(line, '=')) != NULL) {
      strncpy(names, index_star + 1, index_eql - index_star - 1);
    } else {
      strncpy(names, index_star + 1, index_semi - index_star - 1);
    }

  } else {
    if ((index_eql = strchr(line, '=')) != NULL) {
      strncpy(names, index_type, index_eql - index_type);
    } else {
      strncpy(names, index_type, index_semi - index_type);
    }
  }
  return names;
}

VAR* get_function_params(VAR* var, char* line){
    //Returns function parameters, or null if no parameters
    char *index_open;
    char *index_close;
    char *copyof;
    char *token;
    char *parameters = calloc(MAX_NAME_LENGTH ,sizeof(char));
    char *type = calloc(MAX_NAME_LENGTH ,sizeof(char));
    char *name = calloc(MAX_NAME_LENGTH ,sizeof(char));

    char size[MAX_NAME_LENGTH];
    char variable[MAX_NAME_LENGTH];

    index_open = strchr(line, '(');
    index_close = strchr(line, ')');

    
    strncpy(parameters, index_open + 1, index_close - index_open - 1);
    strip_whitespace(parameters);

    copyof = strdup(parameters);
    
    if (strlen(parameters) > 0){
        while ((token = strtok_r(copyof, ",", &copyof))) {
            strip_whitespace(token);
            strcpy(type, get_type(token));
            sprintf(size, "sizeof(%s)", type);
            strcpy(name, get_var_name(type, token));
            var = append_var(var, size, name, type);
        } 
        free(type);
        free(name);
        free(parameters);
        return var;
    }
   free(parameters);
   return NULL;
}



int main(int argc, char **argv) {
  char line[MAX_LINE_LENGTH];
  char var[MAX_VAR_LENGTH];
  char size[MAX_VAR_LENGTH];
  char function_name[MAX_NAME_LENGTH];

  char *type;

  VAR *global_variables = NULL;
  VAR *local_variables = NULL;

  FUN* functions = NULL;

  FILE *fp = fopen(argv[1], "r");
  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    strip_whitespace(line);
    if ((type = get_type(line)) != NULL) {
      // Is a variable or function
      if (line[strlen(line) - 1] == ';') {
        sprintf(size, "sizeof(%s)", type);
        // Is a global variable definition
        // we need to check if the line has commas
        if (strchr(line, ',')) {
          // If it does have commas, we need to find the variables names by
          // parsing by commas
          strcpy(var, get_var_name(type, line));
          global_variables = parse_by_comma(global_variables, var, size, type);
        } else {
          // single variables parsed as global
          strcpy(var, get_var_name(type, line));
          global_variables = append_var(global_variables, size, var, type);
        }
        // Also need to deal with tricky stuff like arrays or static strings
      } else {
        // Is a function definition
        // First get function name:
        local_variables = NULL;
        strcpy(function_name, get_function_name(line));
        FUN *new_function = get_new_fun(function_name);
        // parse parameters
        local_variables = get_function_params(local_variables, line);
        // parse inner variables
        while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
          strip_whitespace(line);
          if (line[strlen(line) - 1] == '}') break;
          if ((type = get_type(line)) != NULL) {
            // proper var dec
            sprintf(size, "sizeof(%s)", type);
            // Is a global variable definition
            // we need to check if the line has commas
            if (strchr(line, ',')) {
              // If it does have commas, we need to find the variableS names by
              // parsing by commas
              strcpy(var, get_var_name(type, line));
              local_variables =
                  parse_by_comma(local_variables, var, size, type);
            } else {
              // single variables parserd as global
              strcpy(var, get_var_name(type, line));
              local_variables = append_var(local_variables, size, var, type);
            }
            //malloc with type case

          }
          //malllocs with no type case 

        }
        // add funtion to function LL's,
        new_function -> var_head = local_variables;
        functions = append_fun(functions, new_function);
        
      }
      free(type);
    }
    
  }



  printf("------------GLOBAL--------------\n");
  printf("name\ttype\tsize\n");
  printf("\n");
  while (global_variables != NULL) {
    printf("%s\t%s\t%s\n", global_variables->name, global_variables->type,
           global_variables->size);
    global_variables = global_variables->next;
  }

  printf("------------functions--------------\n");
  printf("f-name\tv-name\ttype\tsize\n");
  printf("\n");
  while (functions != NULL) {
      while(functions->var_head != NULL) {
          printf("%s\t%s\t%s\t%s\n",functions->name, functions->var_head->name, functions->var_head->type,
           functions->var_head->size);
            functions->var_head = functions->var_head->next;
      }
    functions = functions->next;
  }

  printf("---------------------------------\n");
    
  fclose(fp);

  return 0;
}
