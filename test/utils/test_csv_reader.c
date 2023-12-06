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

    printsb("test sur bale 1...");
    Bale* b1 = csvToBale("test/ressource/bale_1.csv");
    displayBaleLog(b1);
    deleteBale(&b1);
    printsb("\t - test passé");

    printsb("test sur bale 2...");
    Bale* b2 = csvToBale("test/ressource/bale_2.csv");
    displayBaleLog(b2);
    deleteBale(&b2);
    printsb("\t - test passé");

    printsb("test sur bale 3...");
    Bale* b3 = csvToBale("test/ressource/bale_3.csv");
    displayBaleLog(b3);
    deleteBale(&b3);
    printsb("\t - test passé");

    printsb("test sur bale 4...");
    Bale* b4 = csvToBale("test/ressource/bale_4.csv");
    displayBaleLog(b4);
    deleteBale(&b4);
    printsb("\t - test passé");

    printsb("test sur bale 5...");
    Bale* b5 = csvToBale("test/ressource/bale_5.csv");
    displayBaleLog(b5);
    deleteBale(&b5);
    printsb("\t - test passé");

    printsb("test sur bale 6...");
    Bale* b6 = csvToBale("test/ressource/bale_6.csv");
    displayBaleLog(b6);
    deleteBale(&b6);
    printsb("\t - test passé");

    printsb("test sur bale 7...");
    Bale* b7 = csvToBale("test/ressource/bale_7.csv");
    displayBaleLog(b7);
    deleteBale(&b7);
    printsb("\t - test passé");

    return true;

}


bool testCsvToDuel() {
    Duel* d;

    printsb("test sur duel 1...");
    d = csvToDuel("test/ressource/duel_1.csv");
    displayDuelLog(d);
    deleteDuel(&d);
    printsb("\t - test passé");

    printsb("test sur duel 2...");
    d = csvToDuel("test/ressource/duel_2.csv");
    displayDuelLog(d);
    deleteDuel(&d);
    printsb("\t - test passé");

    printsb("test sur duel 3...");
    d = csvToDuel("test/ressource/duel_3.csv");
    displayDuelLog(d);
    deleteDuel(&d);
    printsb("\t - test passé");

    printsb("test sur duel 4...");
    d = csvToDuel("test/ressource/duel_4.csv");
    displayDuelLog(d);
    deleteDuel(&d);
    printsb("\t - test passé");

    printsb("test sur duel 5...");
    d = csvToDuel("test/ressource/duel_5.csv");
    displayDuelLog(d);
    deleteDuel(&d);
    printsb("\t - test passé");

    printsb("test sur duel 6...");
    d = csvToDuel("test/ressource/duel_6.csv");
    displayDuelLog(d);
    deleteDuel(&d);
    printsb("\t - test passé");

    printsb("test sur duel 7...");
    d = csvToDuel("test/ressource/duel_7.csv");
    displayDuelLog(d);
    deleteDuel(&d);
    printsb("\t - test passé");


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
    test_fun(testCsvToDuel, 1, "testCsvToDuel");


    afterAll();

    return return_value;

}