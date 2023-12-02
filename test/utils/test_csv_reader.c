/**
 * @file test_bale.c
 * @author Ugo VALLAT
 * @brief Test sur le lecteur de CSV
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/utils/csv_reader.h"


/*
    ===================
    === OUTILS TEST ===
    ===================
*/

#define printsb(msg) addLineStringBuilder(string_builder, msg)

StringBuilder* string_builder;
int return_value;

void beforeAll() {
    init_logger(NULL);
    string_builder = createStringBuilder();
    return_value = 0;
}

void afterAll() {
    deleteStringBuilder(&string_builder);
    close_logger();
}

void beforeEach() {
    emptyStringBuilder(string_builder);
}

void afterEach() {
}

bool echecTest(char* msg) {
    printsb(msg);
    return false;
}




bool testCsvToBale() {

    Bale* b1 = csvToBale("test/utils/bale_test_1.csv");
    displayBale(b1);
    deleteBale(&b1);

    Bale* b2 = csvToBale("test/utils/bale_test_2.csv");
    displayBale(b2);
    deleteBale(&b2);

    Bale* b3 = csvToBale("test/utils/bale_test_3.csv");
    displayBale(b3);
    deleteBale(&b3);


    return true;

}








void test_fun(bool(*f)(), int fnb, char* fname) {
    beforeEach();
    bool test_success = f();
    afterEach();
    if (!test_success) {
        return_value += fnb;
        printFailure(fname);
        printStringBuilder(string_builder);
    } else printSuccess(fname);
}


int main() {
    beforeAll();

    test_fun(testCsvToBale, 1, "testCsvToBale");


    afterAll();

    return return_value;

}