/**
 * @file test_bale.c
 * @author Ugo VALLAT
 * @brief Test sur uninominale 1 et 2 tours
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/utils/csv_reader.h"
#include "../../src/module/single_member.h"


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

void displayWinnerSingleMember(WinnerSingle *winner) {
    printf("\t<+> %s : %3.2f\n", winner->name, winner->score);
}

bool testOneRoundOnBale(char* file) {
    GenList* lwinner;
    Bale* bale;
    printsb("\t- chargement ballot\n");
    bale = csvToBale(file);
    printBaleLog(bale);
    printsb("\t- calcul\n");
    lwinner = theWinnerOneRound(bale);
    if(!lwinner) return echecTest(" X-- pointeur null\n");
    if(genListSize(lwinner) == 0) return echecTest(" X-- Aucun gagnant\n");
    printf("\nListe de vainqueurs : \n");
    for(unsigned i = 0; i < genListSize(lwinner); i++) {
        displayWinnerSingleMember(genListGet(lwinner, i));
    }
    deleteBale(&bale);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testTheWinnerOneRound() {

    printsb("\ntest sur ballot 1...");
    testOneRoundOnBale("test/ressource/unit/bale_1.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 2...");
    testOneRoundOnBale("test/ressource/unit/bale_2.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 3...");
    testOneRoundOnBale("test/ressource/unit/bale_3.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 4...");
    testOneRoundOnBale("test/ressource/unit/bale_4.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 5...");
    testOneRoundOnBale("test/ressource/unit/bale_5.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 6...");
    testOneRoundOnBale("test/ressource/unit/bale_6.csv");
    printsb( "\n\t- test passé\n");


    printsb("\ntest sur ballot 7...");
    testOneRoundOnBale("test/ressource/unit/bale_7.csv");
    printsb( "\n\t- test passé\n");



    return true;

}

void displayWinnerSingleMemberTwo(WinnerSingleTwo *winner) {
    printf("\t<+> Tour(%d) %s : %3.2f\n", winner->round, winner->name, winner->score);
}

bool testTwoRoundsOnBale(char* file) {
    GenList* lwinner;
    Bale* bale;

    /* chargement du CSV */
    printsb("\t- chargement ballot\n");
    bale = csvToBale(file);
    printBaleLog(bale);
    
    /* calcul des cainqueurs */
    printsb("\t- calcul\n");
    lwinner = theWinnerTwoRounds(bale);
    if(!lwinner) return echecTest(" X-- pointeur null\n");
    if(genListSize(lwinner) == 0) return echecTest(" X-- Aucun gagnant\n");

    /* Affichage des vainqueurs */
    for(unsigned i = 0; i < genListSize(lwinner); i++) {
        displayWinnerSingleMemberTwo(genListGet(lwinner, i));
    }

    /* libération mémoire */
    deleteBale(&bale);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testTheWinnerTwoRounds() {

    printsb("\ntest sur ballot 1...");
    testTwoRoundsOnBale("test/ressource/unit/bale_1.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 2...");
    testTwoRoundsOnBale("test/ressource/unit/bale_2.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 3...");
    testTwoRoundsOnBale("test/ressource/unit/bale_3.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 4...");
    testTwoRoundsOnBale("test/ressource/unit/bale_4.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 5...");
    testTwoRoundsOnBale("test/ressource/unit/bale_5.csv");
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 6...");
    testTwoRoundsOnBale("test/ressource/unit/bale_6.csv");
    printsb( "\n\t- test passé\n");


    printsb("\ntest sur ballot 7...");
    testTwoRoundsOnBale("test/ressource/unit/bale_7.csv");
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

    test_fun(testTheWinnerOneRound, 1, "testTheWinnerOneRound");
    test_fun(testTheWinnerTwoRounds, 1, "testTheWinnerTwoRounds");

    
    afterAll();

    return return_value;

}