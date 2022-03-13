# static-analyser-C

TO RUN:
Please enter in terminal
gcc memoryvisualizer.c -o memory
./memory <file.c>



A static analyzer which takes in sourcecode.c and outputs a memory model for the program.
- Description:
    - lists the name of the variables in the corresponding memory regions.
    - together with the function to where it belongs.
    - its type and size in the machine where the analysis is being performed.
    - program will leave variables or memory requested by  reported without considering whether they have been deallocated or released: a permanent trace

    - The program will report a couple of additional statistics such as
        - total number of lines in the file
        - total number of functions
        - total number of lines per functions
        - total number of variables per function
    - 
    - The program will report results to "standard output", and any potential error (e.g. specified file not found) 

- File Assumptions:
    - all the code to analyze will be contained in one single file
    - the code will be proper C code
    - functions will not be split in declaration and implementation, they will be just defined in one single place in the file
    - within functions, variables declarations will happen only at the beggining of the functions
   
- Source code assumptions:
    - Functions:
        - { }  positioned under name of the function
        - variables declarations are always at the beginning of the functions 
        - variables declarations do not expand more than one line 
    - Variable declarations:
        -  TYPE var1, var2, ...;	
 TYPE var11, var12, ...; 
    - Possible types:
        - int, float, char, void
        - int *, float *, char *
        - int [], float [], char[]

Main Parsing strategy:
The problem boiled down to being external variables and internal, which would be all globally declared variables and locally (by the function). This meant being able to identify between the two, and iterate throught each funciton and grabbing the variables in according structs

Strcuts used:

typedef struct variable {
  char size[MAX_NAME_LENGTH];
  char name[MAX_VAR_LENGTH];
  char type[MAX_TYPE_LENGTH];
  struct variable *next;
} VAR;

typedef struct function {
  char name[MAX_NAME_LENGTH];
  VAR *var_head;  // local variables
  VAR *heap_head; // locally vars that have memory on heap
  struct function *next;
} FUN;


The variables struct contains the fields of size, name, type and the pointer to the next linked list. The reasoning behind all the feilds being of type char* is to get around the allocation of sizes to malloced and alloced arguments, since the sizeoutput of those types will be sizeof(parametr inside).

For each memory partition, a linked list has been used. By checking the declaration inside main one can see:
   
    VAR *global_variables = NULL;
    VAR *local_variables = NULL;
    VAR *heap_variables = NULL;
    VAR *RODATA_variables = NULL;

Each linked list used the same VAR stuct, by this method we will keep the number of structs to the minimal.

Functions used for linked list operations:

FUN *append_fun(FUN *head, FUN *fun) : 
    Appends fun to the existing head function linked list
FUN *get_new_fun(char *name):
    Create and give a new FUN* function struct
VAR *append_var(VAR *head, char *size, char *name, char *type):
    Append the existing linked list head to the given VAR struct
VAR *get_new_var(char *size, char *name, char *type):
    Create and give a new VAR* varible struct


To find global variables the following formula was used:

if (LINE HAS TYPE) AND if (LINE ENDS IN  == ';') 

Then we must be a global variabled, until we get to function header
inside of a function header 

While(next line is not null) and if (line[strlen(line) - 1] == '}') break; 

Then we have finished looking at the local scope of the function and we can append to our function stuct:

new_function -> var_head = local_variables
new_function -> heap_head = heap_variables
functions = append_fun(functions, new_function)


Most used functions:

VAR* get_function_params(VAR* var, char* line)

    The function takes the current line which by assumption is the function header, parses the parameters and append to the given Var LL. Returning the new head

char *get_var_name(char *type, char *line)

    This function takes the type and the current lines and returns the name of the variable names as a char*

char *get_type(char *line)

    This OP robust function takes the current line as input and outputs the type of the line (either function or variable) as a char*

char *rstrchr(char *str, char c)

    Locally created strchr function that reads ther line from the right to find first occurance of char c. Returns a char* to that character.

VAR *parse_by_comma(VAR *head, char *string, char *size, char *type)
    
VAR *parse_by_comma_array(VAR *head, char *string, char *type)

Thank you for reading this, hope you enjoy;
Further documentation within source code.



