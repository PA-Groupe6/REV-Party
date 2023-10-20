
#include <stdio.h>
#include <stdlib.h>

/*


*/

void printl(char* msg) {
    printf("[printl] %s\n", msg);
}

void redirect(FILE* out) {}


void exitl(char* file_name, char* fun_name, char* message , int exit_value) {
    printf("[exitl] %s > %s : %s\n",file_name, fun_name, message);
    exit(exit_value);
}


void warnl(char* file_name, char* fun_name, char* message ) {
    printf("[exitl] %s > %s : %s\n",file_name, fun_name, message);
}