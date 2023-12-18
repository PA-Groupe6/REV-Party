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
    =======================
    === DONNEES DE TEST ===
    =======================
*/

#define NB_DUEL 20
#define MAX_NB_WINNER 10

unsigned nb_winners_uni1_ref[NB_DUEL] = {0,0,0,4,3,2,2,1,0,1,1};
char label_winners_uni1_ref[NB_DUEL][MAX_NB_WINNER][MAX_LENGHT_LABEL] = {
    {""},{""},{""},
    {"C1","C2","C3","C4"},  // 3
    {"C1","C2","C3"},       // 4
    {"C1","C2"},            // 5
    {"C1","C3"},            // 6
    {"C1"},                 // 7
    {""},
    {"C4"},                 // 9
    {"C3"}                  // 10
};

unsigned nb_winners_uni2_ref[NB_DUEL] = {0,0,0,8,6,4,4,1,0,3,5};
char label_winners_uni2_ref[NB_DUEL][MAX_NB_WINNER][MAX_LENGHT_LABEL] = {
    {""},{""},{""},
    {"C1","C2","C3","C4","C1","C2","C3","C4"}, // 3
    {"C1","C2","C3","C1","C2","C3"},           // 4
    {"C1","C2","C1","C2"},
    {"C1","C3","C1","C3"},
    {"C1"},
    {""},
    {"C4","C1","C1"},
    {"C3","C1","C2","C4","C3"}
};

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

bool verifResultSingleOne(GenList* result, unsigned num_test) {
#ifdef DEBUG
    if(num_test >= NB_DUEL) exitl("test_condorcet_schulze", "", EXIT_FAILURE, "num_test invalide %d >= %d", num_test, NB_DUEL);
#endif
    unsigned nb_winners = genListSize(result);
    if(nb_winners != nb_winners_uni1_ref[num_test]) return echecTest("Nombre de gagnants différent");
    
    WinnerSingle *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = (WinnerSingle*)genListGet(result, i);
        if(strcmp(wtmp->name, label_winners_uni1_ref[num_test][i])) return echecTest("mauvais gagnant");
    }

    return true;
}

bool verifResultSingleTwo(GenList* result, unsigned num_test) {
#ifdef DEBUG
    if(num_test >= NB_DUEL) exitl("test_condorcet_schulze", "", EXIT_FAILURE, "num_test invalide %d >= %d", num_test, NB_DUEL);
#endif
    unsigned nb_winners = genListSize(result);
    if(nb_winners != nb_winners_uni2_ref[num_test]) return echecTest("Nombre de gagnants différent");
    
    WinnerSingle *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = (WinnerSingle*)genListGet(result, i);
        if(strcmp(wtmp->name, label_winners_uni2_ref[num_test][i])) return echecTest("mauvais gagnant");
    }

    return true;
}



bool testOneRoundOnBale(char* file, unsigned num_test) {
    GenList* lwinner;
    Bale* bale;
    printsb("\t- chargement ballot\n");
    bale = csvToBale(file);

    printsb("\t- calcul\n");
    lwinner = theWinnerOneRound(bale);
    if(!lwinner) return echecTest("pointeur null\n");
    if(!verifResultSingleOne(lwinner, num_test)) {
        printl("\n\n<+>---------------------------- [ %s ] :\n\n", file);
        displayBaleLog(bale);
        displayListWinnerSingle(lwinner);

        deleteBale(&bale);
        while(!genListEmpty(lwinner))
            free(genListPop(lwinner));
        deleteGenList(&lwinner);
        return false;
    }

    deleteBale(&bale);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testTheWinnerOneRound() {


    printsb("\ntest sur ballot 3...");
    if(!testOneRoundOnBale("test/ressource/bale_3.csv", 3)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 4...");
    if(!testOneRoundOnBale("test/ressource/bale_4.csv", 4)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 5...");
    if(!testOneRoundOnBale("test/ressource/bale_5.csv", 5)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 6...");
    if(!testOneRoundOnBale("test/ressource/bale_6.csv", 6)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 7...");
    if(!testOneRoundOnBale("test/ressource/bale_7.csv", 7)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 9...");
    if(!testOneRoundOnBale("test/ressource/bale_9.csv", 9)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 10...");
    if(!testOneRoundOnBale("test/ressource/bale_10.csv", 10)) return false;
    printsb( "\n\t- test passé\n");

    return true;

}



bool testTwoRoundsOnBale(char* file, unsigned num_test) {
    GenList* lwinner;
    Bale* bale;

    /* chargement du CSV */
    printsb("\t- chargement ballot\n");
    bale = csvToBale(file);
    
    /* calcul des cainqueurs */
    printsb("\t- calcul\n");
    lwinner = theWinnerTwoRounds(bale);
    if(!lwinner) return echecTest("pointeur null\n");
    if(genListSize(lwinner) == 0) return echecTest("Aucun gagnant\n");
    if(!verifResultSingleTwo(lwinner, num_test)) {
        printl("\n\n<+>---------------------------- [ %s ] :\n\n", file);
        displayBaleLog(bale);
        displayListWinnerSingleTwo(lwinner);

        deleteBale(&bale);
        while(!genListEmpty(lwinner))
            free(genListPop(lwinner));
        deleteGenList(&lwinner);
        return false;
    }

    /* libération mémoire */
    deleteBale(&bale);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testTheWinnerTwoRounds() {

    printsb("\ntest sur ballot 3...");
    if(!testTwoRoundsOnBale("test/ressource/bale_3.csv", 3)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 4...");
    if(!testTwoRoundsOnBale("test/ressource/bale_4.csv", 4)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 5...");
    if(!testTwoRoundsOnBale("test/ressource/bale_5.csv", 5)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 6...");
    if(!testTwoRoundsOnBale("test/ressource/bale_6.csv", 6)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 7...");
    if(!testTwoRoundsOnBale("test/ressource/bale_7.csv", 7)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 9...");
    if(!testTwoRoundsOnBale("test/ressource/bale_9.csv", 9)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur ballot 10...");
    if(!testTwoRoundsOnBale("test/ressource/bale_10.csv", 10)) return false;
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