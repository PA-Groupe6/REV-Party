/**
 * @file test_condorcet_minimac.c
 * @author Ugo VALLAT
 * @brief Test sur condorcet minimax
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/utils/csv_reader.h"
#include "../../src/module/condorcet.h"




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

bool testRankedPairsOnDuel(char* file) {
    GenList* lwinner;
    Duel* duel;
    printsb("\t- chargement ballot\n");
    duel = csvToDuel(file);
    displayDuelLog(duel);
    
    printsb("\t- calcul\n");
    lwinner = theWinnerRankedPairs(duel);

    if(!lwinner) return echecTest(" X-- pointeur null\n");

    printf("\nListe de vainqueurs : \n");
    for(unsigned i = 0; i < genListSize(lwinner); i++) {
        displayListWinnerCondorcet(lwinner, "RANKED PAIRS");
    }
    deleteDuel(&duel);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}


bool testRankedPairs() {

    printsb("\ntest sur duel 1...");
    testRankedPairsOnDuel("test/ressource/unit/duel_1.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 2...");
    testRankedPairsOnDuel("test/ressource/unit/duel_2.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 3...");
    testRankedPairsOnDuel("test/ressource/unit/duel_3.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 4...");
    testRankedPairsOnDuel("test/ressource/unit/duel_4.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 5...");
    testRankedPairsOnDuel("test/ressource/unit/duel_5.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 6...");
    testRankedPairsOnDuel("test/ressource/unit/duel_6.csv");
    printsb( "\n\t- test passé\n");


    printsb("\ntest sur duel 7...");
    testRankedPairsOnDuel("test/ressource/unit/duel_7.csv");
    printsb( "\n\t- test passé\n");


    return true;

}


bool testMinimaxOnDuel(char* file) {
    GenList* lwinner;
    Duel* duel;
    printsb("\t- chargement ballot\n");
    duel = csvToDuel(file);
    displayDuelLog(duel);
    
    printsb("\t- calcul\n");
    lwinner = theWinnerMinimax(duel);

    if(!lwinner) return echecTest(" X-- pointeur null\n");

    printf("\nListe de vainqueurs : \n");
    displayListWinnerCondorcet(lwinner, "MINIMAX");
    deleteDuel(&duel);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testMinimax() {

    printsb("\ntest sur duel 1...");
    testMinimaxOnDuel("test/ressource/unit/duel_1.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 2...");
    testMinimaxOnDuel("test/ressource/unit/duel_2.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 3...");
    testMinimaxOnDuel("test/ressource/unit/duel_3.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 4...");
    testMinimaxOnDuel("test/ressource/unit/duel_4.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 5...");
    testMinimaxOnDuel("test/ressource/unit/duel_5.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 6...");
    testMinimaxOnDuel("test/ressource/unit/duel_6.csv");
    printsb( "\n\t- test passé\n");


    printsb("\ntest sur duel 7...");
    testMinimaxOnDuel("test/ressource/unit/duel_7.csv");
    printsb( "\n\t- test passé\n");


    return true;

}

bool testSchulzeOnDuel(char* file) {
    GenList* lwinner;
    Duel* duel;
    printsb("\t- chargement ballot\n");
    duel = csvToDuel(file);
    displayDuelLog(duel);
    
    printsb("\t- calcul\n");
    lwinner = theWinnerSchulze(duel);

    if(!lwinner) return echecTest(" X-- pointeur null\n");

    printf("\nListe de vainqueurs : \n");
    displayListWinnerCondorcet(lwinner, "SCHULZE");
    deleteDuel(&duel);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testSchulze() {

    printsb("\ntest sur duel 1...");
    testSchulzeOnDuel("test/ressource/unit/duel_1.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 2...");
    testSchulzeOnDuel("test/ressource/unit/duel_2.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 3...");
    testSchulzeOnDuel("test/ressource/unit/duel_3.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 4...");
    testSchulzeOnDuel("test/ressource/unit/duel_4.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 5...");
    testSchulzeOnDuel("test/ressource/unit/duel_5.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 6...");
    testSchulzeOnDuel("test/ressource/unit/duel_6.csv");
    printsb( "\n\t- test passé\n");


    printsb("\ntest sur duel 7...");
    testSchulzeOnDuel("test/ressource/unit/duel_7.csv");
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

    test_fun(testRankedPairs, 1, "testRankedPairs");
    test_fun(testMinimax, 1, "testMinimax");
    test_fun(testSchulze, 1, "testSchulze");

    
    afterAll();

    return return_value;

}