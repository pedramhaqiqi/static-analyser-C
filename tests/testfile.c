#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memoryvisualizer.c"


    // char typ[MAX_NAME_LENGHT];
    // char* ptr_1 = strchr(line, '(');
    // char ptr_2[MAX_NAME_LENGHT];
    // strncpy(ptr_2, line, ptr_1 - line);
    // sscanf(ptr_2, "%s %s", typ, name);

int main(int argc, char** argv) {
    

    FILE* fp =  fopen(argv[1], "r");
    char line[MAX_LINE_LENGHT];
    VAR* head = NULL;
    
    while(fgets(line, MAX_LINE_LENGHT,fp) != NULL){
        int colon = has_colon(line);
        if (get_type(line) && colon == 1){
            if(has_eql(line) == 1){
                int alloc = alloc_validator(line);
                if(alloc = 1){

                }else if(alloc = 2){

                }       
            }
        } else if(get_type) && checks_next_line(fp) )
    }

    fclose(fp);

    return 0;
}