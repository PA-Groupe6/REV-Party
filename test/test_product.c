#include <stdio.h>
#include <stdlib.h>
#include "test_utils.h"

#define CMD "bin/rev"
#define SRC_FILE "test/ressource/bale_1.csv"
#define OUTPUT_FILE "test/ressource/product/product.log"

int return_value;

void beforeAll() {
    return_value = 0;
}

void afterAll() {
    if (return_value == 0)
        remove(OUTPUT_FILE);
}

int main() {
    beforeAll();

    // test bin/rev -m all -i bale_1.csv > product.log
    char command[128];
    sprintf(command, "%s -m all -i %s > %s", CMD, SRC_FILE, OUTPUT_FILE);
    int status = system(command);
    printf("cmd: %s\nstatus: %d\n", command, status);

    afterAll();
    return return_value;
}