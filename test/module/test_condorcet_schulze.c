/**
 * @file test_condorcet_schulze.c
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




bool testSchulzeOnDuel(char* file) {
    GenList* lwinner;
    Duel* duel;
    printsb("\t- chargement duel\n");
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

    // printsb("\ntest sur duel 1...");
    // printl("\n\n<+>--------------------------------- test sur [ duel 1 ] :\n\n");
    // testSchulzeOnDuel("test/ressource/duel_1.csv");
    // printsb( "\n\t- test passé\n");

    // printsb("\ntest sur duel 2...");
    // printl("\n\n<+>--------------------------------- test sur [ duel 2 ] :\n\n");
    // testSchulzeOnDuel("test/ressource/duel_2.csv");
    // printsb( "\n\t- test passé\n");

    // printsb("\ntest sur duel 3...");
    // printl("\n\n<+>--------------------------------- test sur [ duel 3 ] :\n\n");
    // testSchulzeOnDuel("test/ressource/duel_3.csv");
    // printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 4...");
    printl("\n\n<+>--------------------------------- test sur [ duel 4 ] :\n\n");
    testSchulzeOnDuel("test/ressource/duel_4.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 5...");
    printl("\n\n<+>--------------------------------- test sur [ duel 5 ] :\n\n");
    testSchulzeOnDuel("test/ressource/duel_5.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 6...");
    printl("\n\n<+>--------------------------------- test sur [ duel 6 ] :\n\n");
    testSchulzeOnDuel("test/ressource/duel_6.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 7...");
    printl("\n\n<+>--------------------------------- test sur [ duel 7 ] :\n\n");
    testSchulzeOnDuel("test/ressource/duel_7.csv");
    printsb( "\n\t- test passé\n");

    // printsb("\ntest sur duel 8...");
    // printl("\n\n<+>--------------------------------- test sur [ duel 8 ] :\n\n");
    // testSchulzeOnDuel("test/ressource/duel_of_bale_8.csv");
    // printsb( "\n\t- test passé\n");

    // printsb("\ntest sur duel 9...");
    // printl("\n\n<+>--------------------------------- test sur [ duel 9 ] :\n\n");
    // testSchulzeOnDuel("test/ressource/duel_of_bale_9.csv");
    // printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 10...");
    printl("\n\n<+>--------------------------------- test sur [ duel 10 ] :\n\n");
    testSchulzeOnDuel("test/ressource/duel_10.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 11...");
    printl("\n\n<+>--------------------------------- test sur [ duel 11 ] :\n\n");
    testSchulzeOnDuel("test/ressource/duel_11.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 12...");
    printl("\n\n<+>--------------------------------- test sur [ duel 12 ] :\n\n");
    testSchulzeOnDuel("test/ressource/duel_12.csv");
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

    test_fun(testSchulze, 1, "testSchulze");

    
    afterAll();

    return return_value;

}