#include <stdio.h>
#include <stdlib.h>

void hot_run(char* program) {
    int status;

    if(fork() == 0){ 
        status = system(program);
        exit(0);
    } 
}
