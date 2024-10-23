# Static Memory Analyzer

## Overview

This project is a Static Memory Analyzer written in C that takes in a C source code file as input and outputs a memory model of the program. The tool performs static analysis to track variables and memory usage, providing a detailed report of the memory model, including variable sizes, types, and locations in memory, while also providing useful statistics about the code.

## Features

	•	Memory Visualization: Lists variable names in their corresponding memory regions, along with the function to which they belong, their type, and their size as determined by the machine.
	•	Permanent Trace: Reports variables and memory requested, without considering whether they have been deallocated or released.
	•	Statistics Reporting:
	•	Total number of lines in the file
	•	Total number of functions
	•	Total number of lines per function
	•	Total number of variables per function
	•	Linked Lists for Memory Partitions: Uses linked lists to store global, local, heap, and read-only (RODATA) variables, and outputs them efficiently.

## How It Works

The analyzer parses the C source code and identifies all variables and functions in the file. It categorizes variables based on their memory location (stack, heap, or global) and produces a visual representation of the memory model. Additionally, the program reports statistics about the structure of the source code.

	1.	Source Code Parsing: Identifies global and local variables, function parameters, and heap allocations.
	2.	Memory Model: Outputs the memory model by tracking where variables reside (stack, heap, global memory, or read-only data).
	3.	Error Handling: Reports any errors, such as “file not found,” and outputs results to standard output.

## Usage

To compile and run the analyzer, use the following commands:

gcc memoryvisualizer.c -o memory
./memory <file.c>

	•	<file.c>: The C source code file you wish to analyze. The program outputs the memory model and statistics about the file.

Program Assumptions

	•	All the code to analyze is contained in a single file.
	•	The code is valid C code.
	•	Functions are not split between declaration and implementation.
	•	Variable declarations occur only at the beginning of functions.
	•	The format for variable declarations follows:

TYPE var1, var2, ...;


	•	Supported variable types:
	•	int, float, char, void
	•	Pointers (int *, float *, char *)
	•	Arrays (int [], float [], char [])

## Main Parsing Strategy

The program differentiates between global and local variables. It iterates through each function, identifying the local variables within their scope and appending them to the appropriate memory partition (global, local, heap, or read-only data).

##  Key Data Structures

	•	Variable Struct (VAR):

typedef struct variable {
    char size[MAX_NAME_LENGTH];  // Size of the variable
    char name[MAX_VAR_LENGTH];   // Name of the variable
    char type[MAX_TYPE_LENGTH];  // Type of the variable
    struct variable *next;       // Pointer to the next variable in the list
} VAR;


	•	Function Struct (FUN):

typedef struct function {
    int totallines;               // Number of lines per function
    char name[MAX_NAME_LENGTH];   // Name of the function
    VAR *var_head;                // Local variables
    VAR *heap_head;               // Variables allocated on the heap
    VAR *var_lit;                 // Variables in read-only data (RODATA)
    struct function *next;        // Pointer to the next function in the list
} FUN;



##  Code Example

A typical function analyzed by the tool would be represented as follows:

int *arr = malloc(10 * sizeof(int));  // Tracked as a heap variable
arr[0] = 5;
free(arr);  // Memory operations are tracked, even after free

##  Linked Lists for Memory Tracking

	•	Global Variables: Stored in the global_variables linked list.
	•	Local Variables: Stored in the local_variables linked list.
	•	Heap Variables: Stored in the heap_variables linked list.
	•	Read-Only Data (RODATA) Variables: Stored in RODATA_variables.

##  Functions for Linked List Operations

	•	FUN *append_fun(FUN *head, FUN *fun): Appends a function to the list.
	•	VAR *append_var(VAR *head, char *size, char *name, char *type): Appends a variable to the list.
	•	VAR *get_new_var(char *size, char *name, char *type): Creates a new variable struct.

##  Most Used Functions

	•	VAR* get_function_params(VAR* var, char* line): Parses function parameters and appends them to the local variable list.
	•	char *get_var_name(char *type, char *line): Extracts variable names from a line.
	•	char *get_type(char *line): Determines whether the line is declaring a function or a variable.

Print Functions

The program outputs results in the specified format, using the following functions:

	•	void print_function_heap(FUN* head)
	•	void print_global_vars(VAR *head)
	•	void print_ro_vars(VAR *head)
	•	void print_function_stack(FUN *head)

Statistics Reported

	•	Total number of lines in the file (excluding braces {}).
	•	Total number of lines per function.
	•	Total number of variables per function.

Further Documentation

For more details, refer to the comments within the source code.

Thank You
