/**
 * @file test_majority_judgment.c
 * @author Ugo VALLAT
 * @brief Test sur jugement majoritaire
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/utils/csv_reader.h"
#include "../../src/module/majority_judgment.h"


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
    char buff[256] = "\n X-- ";
    strncat(buff, msg,256);
    printsb(buff);
    return false;
}



bool testMajorityJudgmentOnDuel(char* file) {
    GenList* lwinner;
    Bale* bale;
    /* chargment du bale */
    printsb("\t- chargement ballot\n");
    bale = csvToBale(file);
    displayBaleLog(bale);

    /* calcul des gagnants */
    printsb("\t- calcul\n");
    lwinner = theWinnerMajorityJudgment(bale);
    if(!lwinner) return echecTest("pointeur null\n");
    if(genListSize(lwinner) == 0) return echecTest("Aucun gagnant\n");
    displayListMajorityJudgment(lwinner);

    /* libération de la mémoire */
    deleteBale(&bale);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testTheWinnerMajorityJudgment() {


    printsb("\ntest sur bale 1...");
    testMajorityJudgmentOnDuel("test/ressource/bale_1.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 2...");
    testMajorityJudgmentOnDuel("test/ressource/bale_2.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 3...");
    testMajorityJudgmentOnDuel("test/ressource/bale_3.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 4...");
    testMajorityJudgmentOnDuel("test/ressource/bale_4.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 5...");
    testMajorityJudgmentOnDuel("test/ressource/bale_5.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 6...");
    testMajorityJudgmentOnDuel("test/ressource/bale_6.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 7...");
    testMajorityJudgmentOnDuel("test/ressource/bale_7.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 8...");
    testMajorityJudgmentOnDuel("test/ressource/bale_8.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 9...");
    testMajorityJudgmentOnDuel("test/ressource/bale_9.csv");
    printsb( "\n\t- test passé\n");

    
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

    test_fun(testTheWinnerMajorityJudgment, 1, "testTheWinnerMajorityJudgment");

    
    afterAll();

    return return_value;

}