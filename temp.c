#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main(int argc, char** argv) {
   
    char fmt[256];
    char* s = "int* functionname()";
    char string[256];
    char type[256] = "int*";
    
    sprintf(fmt, "%s%%[^(]", type);
    sscanf(s, fmt, string );
    printf("%s", string);

    //VAR* head = NULL;
    //VAR* headcopy = NULL;
    //char* s = "float i = hello(sizeof(somethign), 349);";
    // char *size = "sizeof(int)";
    // char *type = "int";
    
    // head = parse_by_comma(head, semi_parse , size, type);
   // printf("%s", alloc_parser(s));

    // while (head!=NULL){
    //     printf("%s\n", head->name);
    //     head = head -> next;
    // }
    
    //printf("result remove semi %s\n", remove_semicolon(s));
    //printf("result remove space %s\n", remove_space(s));
    //printf("result remove semi and space %s\n", remove_space(remove_semicolon(s)));
    //test(s);
    
    return 0;
}