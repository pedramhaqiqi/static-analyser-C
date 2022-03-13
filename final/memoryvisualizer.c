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
  VAR *heap_head; // locally vars that have memory on heap
  struct function *next;
} FUN;

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

void strip_star(char *line) {
  char temp[strlen(line) + 1];
  int i;
  for (i = strlen(line) - 1; i >= 0; i--) {
    if (line[i] != '*') break;
  }
  line[i + 1] = '\0';
  for (i = 0; i < strlen(line); i++) {
    if (line[i] != '*') break;
  }
  memmove(temp, &line[i], strlen(&line[i]) + 1);
  for (i = 0; i <= strlen(line); i++) {
    line[i] = temp[i];
  }
}

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
  strip_whitespace(name);
  VAR *newvar = get_new_var(size, name, type);
  newvar->next = head;
  return newvar;
}

FUN *get_new_fun(char *name) {
  FUN *res = malloc(sizeof(FUN));
  strip_whitespace(name);
  strcpy(res->name, name);
  res->var_head = NULL;
  res->next = NULL;
  res->heap_head = NULL;
  return res;
}

FUN *append_fun(FUN *head, FUN *fun) {
  fun->next = head;
  return fun;
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
        int index = -1 * (int)(param - cm_ptr);
        param[index] = '*';
      }
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
  char *types_ext[] = {" %[(**)]", " %[*]", "%[ ]"};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      strcpy(type, types_native[i]);
      strcat(type, types_ext[j]);
      sprintf(fmt, "%s", type);
      if (sscanf(line, fmt, temp) > 0) {
        strcpy(type, types_native[i]);
        strcat(type, j == 0 ? "**" : (j == 1 ? "*" : ""));
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
            strip_whitespace(name);
            strip_star(name);
            strip_whitespace(name);
            var = append_var(var, size, name, type);
        } 
        free(type);
        free(name);
        free(parameters);
        return var;
    }
   free(type);
   free(name);
   free(parameters);
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

int get_array_argument_count(char *string){
    
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


VAR *parse_by_comma_array(VAR *head, char *string, char *type) {
  char *copyof;
  char *token;
  char *array_size;
  char *arrayname;
  char *before_eql;
  char *type_ptr;

  char tmptype[MAX_TYPE_LENGTH];
  char tmpsize[MAX_NAME_LENGTH];
  char tmpstring[MAX_LINE_LENGTH];

  int count;

  
  
  type_ptr = strstr(string, type) + strlen(type); 
  strcpy(tmpstring, type_ptr);
  
  before_eql = parse_by_eql(tmpstring);
  strip_whitespace(before_eql);
  sprintf(tmptype,"%s[]",type); 
  
  copyof = strdup(before_eql);
  if(strchr(string, '=')) {
      
      if(strchr(before_eql, ',')){
          
          while ((token = strtok_r(copyof, ",", &copyof))) {
            strip_whitespace(token);
            array_size = get_array_size(token);
            arrayname = get_array_name(token);
            sprintf(tmpsize,"%s*sizeof(%s)", array_size, type);
            head = append_var(head, tmpsize, arrayname, tmptype);
            return head;
        }
      }else{
          
          if((array_size = get_array_size(before_eql))){
              arrayname = get_array_name(before_eql);
              sprintf(tmpsize,"%s*sizeof(%s)", array_size, type);
              head = append_var(head, tmpsize, arrayname, tmptype);
              return head;
          }else{
              
              arrayname = get_array_name(before_eql);
              count = get_array_argument_count(string);
              sprintf(tmpsize,"%d*sizeof(%s)", count, type);
              head = append_var(head, tmpsize, arrayname, tmptype);
              return head;
          }
      }
  }

  while ((token = strtok_r(copyof, ",", &copyof))) {
    strip_whitespace(token);
    array_size = get_array_size(token);
    arrayname = get_array_name(token);
    sprintf(tmpsize,"%s*sizeof(%s)", array_size, type);
    head = append_var(head, tmpsize, arrayname, tmptype);
  }
  return head;
}

char* get_literral(char* line){
    char* op_ptr;
    char* cl_ptr;
    char* lit = calloc(MAX_NAME_LENGTH ,sizeof(char));


    op_ptr = strchr(line, '"');
    cl_ptr = rstrchr(line, '"');

    strncpy(lit, op_ptr+1, cl_ptr - op_ptr -1);
    return lit;
}


void print_function_heap(FUN* head) {
  VAR * heap_var;
    while (head != NULL) {
        heap_var = head->heap_head;
        while(heap_var != NULL) {
          printf("%s\t%s\t%s\t%s\n", heap_var->name,head->name, heap_var ->type,
           heap_var ->size);
            heap_var  = heap_var->next;
      }
      head = head->next;
    }
}

void print_global_vars(VAR *head) {
    while (head != NULL) {
        printf("%s\t%s\t%s\t%s\n", head->name,"global", head->type,
        head->size);
        head = head->next;
    }
}

void print_ro_vars(VAR *head) {
    while (head != NULL) {
        printf("%s\t%s\t%s\t%s\n", head->name,"literal", head->type,
        head->size);
        head = head->next;
    }
}

void print_function_stack(FUN * head){
  VAR * stack_var;
    while (head != NULL) {
      stack_var = head->var_head;
        while(stack_var != NULL) {
          printf("%s\t%s\t%s\t%s\n", stack_var->name,head->name, stack_var ->type,
           stack_var ->size);
            stack_var  = stack_var->next;
      }
        head = head->next;
    }
}

VAR * get_reverse_var(VAR * head) {
  VAR * res = NULL;
  VAR * temp = NULL;
  while (head != NULL) {
    temp = head->next;
    head->next = res;
    res = head;
    head = temp;
  }
  return res;
}

FUN * get_reverse_fun(FUN * head) {
  FUN * res = NULL;
  FUN * temp = NULL;
  while (head != NULL) {
    head->var_head = get_reverse_var(head->var_head);
    head->heap_head = get_reverse_var(head->heap_head);
    temp = head->next;
    res = append_fun(res, head);
    head = temp;
  }
  return res;
}

int main(int argc, char **argv) {
  char line[MAX_LINE_LENGTH];
  char var[MAX_VAR_LENGTH];
  char svar[MAX_VAR_LENGTH];
  char size[MAX_VAR_LENGTH];
  char function_name[MAX_NAME_LENGTH];
  char tmpsize[MAX_NAME_LENGTH];
  char tmptype[MAX_NAME_LENGTH];

  char *de_star_type = calloc(MAX_NAME_LENGTH ,sizeof(char));
  char *array_size;
  char *type;
  char *array_name;
  char *lit_string;

  VAR *global_variables = NULL;
  VAR *local_variables = NULL;
  VAR *heap_variables = NULL;
  VAR *RODATA_variables = NULL;

  FUN* functions = NULL;

  errors(argc, argv);

  FILE *fp = fopen(argv[1], "r");
  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    strip_whitespace(line);
    
    if ((type = get_type(line)) != NULL) { 
        
      // Is a variable or function
      if (line[strlen(line) - 1] == ';') {
         
        sprintf(size, "sizeof(%s)", type);

        // Is a global variable definition
        // we need to check if the line has commas
         
        if(strchr(get_var_name(type, line), '[')){
             strcpy(var,get_var_name(type, line));
            //Global array declaration
                if(strchr(line, ',')){
                   
                    //mutliple declaration (must have array size)
                    //single arrays with = {, , ,}
                    // type array [1], arra[2];
                    // type array [1], arra[2] = ....;  
                   
                    global_variables = parse_by_comma_array(global_variables, line, type);
                     
                }else{
                    //Two cases: array size, no array size 
                     sprintf(tmptype,"%s[]",type); 
                    if((array_size = get_array_size(var))){
                        array_name = get_array_name(var);
                        sprintf(tmpsize,"%s*sizeof(%s)", array_size, type);
                        global_variables = append_var(global_variables, tmpsize, array_name, tmptype);

                    }else{
                        lit_string = get_literral(line);
                        sprintf(tmpsize,"%lu*sizeof(%s)", strlen(lit_string), type);
                        array_name = get_array_name(var);
                        global_variables = append_var(global_variables, tmpsize, array_name, tmptype);
                        
                    }
                }
                
            } 
        else if(strchr(line, ',')) {
           
          // If it does have commas, we need to find the variables names by
          // parsing by commas 
          
          strcpy(var, get_var_name(type, line));
          global_variables = parse_by_comma(global_variables, var, size, type);
          
        } else {
            
          // single variables parsed as global
          strcpy(var, get_var_name(type, line));
          if (strcmp(type, "char*") == 0){
              if(rstrchr(line, '"')){
                  strncpy(de_star_type, type, strlen(type) - 1);
                  lit_string = get_literral(line);
                  sprintf(svar, "*%s", var);
                  sprintf(size, "%lu*sizeof(%s)", strlen(lit_string),de_star_type);

                  RODATA_variables = append_var(RODATA_variables, size, svar, type);
              }
          }
          global_variables = append_var(global_variables, size, var, type);
        }
        
        
        // Also need to deal with tricky stuff like arrays or static strings
      } else { //START PARSING FUNCTION
           
        // Is a function definition
        // First get function name:
        local_variables = NULL;
        heap_variables = NULL;
        strcpy(function_name, get_function_name(line));
        FUN *new_function = get_new_fun(function_name);
        // parse parameters
        if (get_function_params(local_variables,line) != NULL){
            local_variables = get_function_params(local_variables, line);
        }
        // parse inner variables
        while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
          strip_whitespace(line);
          if (line[strlen(line) - 1] == '}') break;
          if ((type = get_type(line)) != NULL) {
            // proper var dec
            sprintf(size, "sizeof(%s)", type);
            // we need to check if the line has commas
             if(strchr(get_var_name(type, line), '[')){
             strcpy(var,get_var_name(type, line));
            //array declaration
                if(strchr(line, ',')){
                   
                    //mutliple declaration (must have array size)
                    //single arrays with = {, , ,}
                    // type array [1], arra[2];
                    // type array [1], arra[2] = ....;  
                   
                    local_variables = parse_by_comma_array(local_variables, line, type);
                     
                }else{
                    //Two cases: array size, no array size 
                     sprintf(tmptype,"%s[]",type); 
                    if((array_size = get_array_size(var))){
                        array_name = get_array_name(var);
                        sprintf(tmpsize,"%s*sizeof(%s)", array_size, type);
                        local_variables = append_var(local_variables, tmpsize, array_name, tmptype);

                    }else{
                        
                        lit_string = get_literral(line);
                        sprintf(tmpsize,"%lu*sizeof(%s)", strlen(lit_string), type);
                        array_name = get_array_name(var);
                        local_variables = append_var(local_variables, tmpsize, array_name, tmptype);
                        
                    }
                }
                
            } else if (alloc_parser(line)) {
              //alllocate heap var on stack
                strcpy(var, get_var_name(type, line));
                local_variables = append_var(local_variables, size, var , type);

                //allocate hon heap
                strncpy(de_star_type, type, strlen(type) - 1);
                sprintf(size, "sizeof(%s)", alloc_parser(line));
                sprintf(svar, "*%s", var);
                heap_variables = append_var(heap_variables, size, svar, de_star_type);
                free(de_star_type);
            }else if (strchr(line, ',')) {
              // If it does have commas, we need to find the variables names by
              // parsing by commas
              strcpy(var, get_var_name(type, line));
              local_variables =
                  parse_by_comma(local_variables, var, size, type);
            } else {
              // single variables parserd 
              if (strcmp(type, "char*") == 0){
                if(rstrchr(line, '"')){
                    strncpy(de_star_type, type, strlen(type) - 1);
                    lit_string = get_literral(line);
                    sprintf(svar, "*%s", var);
                    sprintf(size, "%lu*sizeof(%s)", strlen(lit_string),de_star_type);

                    RODATA_variables = append_var(RODATA_variables, size, svar, type);
                }
            }
              strcpy(var, get_var_name(type, line));
              local_variables = append_var(local_variables, size, var, type);
            }
            

          }
           //*alloced that does not contain type
           
           if (alloc_parser(line)){
               VAR* temp = local_variables;
               char *temp_name = calloc(MAX_NAME_LENGTH ,sizeof(char));
               char *eq_ptr;
               eq_ptr = strchr(line, '=');
               strncpy(temp_name, line, eq_ptr - line - 1);
               strip_whitespace(temp_name);
               while(temp != NULL){
                   if(strcmp(temp->name, temp_name) == 0){
                        strncpy(de_star_type, temp->type, strlen(temp->type) - 1);
                        sprintf(size, "sizeof(%s)", alloc_parser(line));
                        sprintf(svar, "*%s", temp->name);
                        heap_variables = append_var(heap_variables, size, svar, de_star_type);
                        free(de_star_type);
                        free(temp_name);
                   }
                   temp = temp->next;
               }
               
           } 
       
        }
        // add funtion to function LL's,(NULL CHECK?)
        new_function -> var_head = local_variables;
        new_function -> heap_head = heap_variables;
        functions = append_fun(functions, new_function); 
      }
      free(type); 
    }
  }

  fclose(fp);
  
  int i;

  // Reverse everything:
  functions = get_reverse_fun(functions);
  global_variables = get_reverse_var(global_variables);
  
  printf(">>> Memory Model Layout <<<\n");
  printf("***  exec // text ***\n");
  printf("\t%s\n", argv[1]);

  printf("### ROData ###       scope type  size\n");

  print_ro_vars(RODATA_variables);

  printf("### static data ###\n");
  print_global_vars(global_variables);

  printf("### heap ###\n");
  print_function_heap(functions);


  printf("####################\n");
  printf("### unused space ###\n");
  printf("####################\n");


  printf("### stack ###\n");
  print_function_stack(functions);



  printf("**** STATS ****\n");

  printf("Total number of lines in the file\n");

  fp = fopen(argv[1], "r");
  for (i=0; fgets(line, MAX_LINE_LENGTH, fp) != NULL; i++){
    ;
  }
  fclose(fp);

  printf("%d\n", i);

  printf("Total number of functions: \n");

  FUN * temp = functions;
  for (i=0; temp != NULL; temp=temp->next, i++) {
    ;
  }

  printf("%d\n", i);

  printf("Total number of lines per functions:\n");
  
  printf("Total number of variables per function:\n");

  printf("//////////////////////////////\n");

  return 0;
}
