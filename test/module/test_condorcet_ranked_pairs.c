/**
 * @file test_condorcet_ranked_pairs.c
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
    =======================
    === DONNEES DE TEST ===
    =======================
*/

#define NB_DUEL 20
#define MAX_NB_WINNER 10

unsigned nb_winners_ref[NB_DUEL] = {0,0,0,0,1,1,1,1,0,0,1};
char label_winners_ref[NB_DUEL][MAX_NB_WINNER][MAX_LENGHT_LABEL] = {
    {""},{""},{""},{""},
    {"C3"},         // 4
    {"C3"},         // 5
    {"C1"},         // 6
    {"C3"},         // 7
    {""},{""},
    {"C4"}          // 10

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

bool verifResultMinimax(GenList* result, unsigned num_test) {
#ifdef DEBUG
    if(num_test >= NB_DUEL) exitl("test_condorcet_ranked_pairs", "", EXIT_FAILURE, "num_test invalide %d >= %d", num_test, NB_DUEL);
#endif
    unsigned nb_winners = genListSize(result);
    if(nb_winners != nb_winners_ref[num_test]) return echecTest("Nombre de gagnants différent");
    
    WinnerCondorcet *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = (WinnerCondorcet*)genListGet(result, i);
        if(strcmp(wtmp->name, label_winners_ref[num_test][i])) return echecTest("mauvais gagnant");
    }

    return true;
}



bool testRankedPairsOnDuel(char* file, unsigned num_test) {
    GenList* lwinner;
    Duel* duel;
    printsb("\t- chargement duel\n");
    duel = csvToDuel(file);
    
    printsb("\t- calcul\n");
    lwinner = theWinnerRankedPairs(duel);

    if(!lwinner) return echecTest("pointeur null\n");
    if(!verifResultMinimax(lwinner, num_test)) {
        printl("\n\n<+>---------------------------- [ %s ] :\n\n", file);
        displayDuelLog(duel);
        displayListWinnerCondorcet(lwinner, "RANGEMENT DES PAIRS");

        deleteDuel(&duel);
        while(!genListEmpty(lwinner))
            free(genListPop(lwinner));
        deleteGenList(&lwinner);
        return false;
    }

    deleteDuel(&duel);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testRankedPairs() {

    printsb("\ntest sur duel 4...");
    if(!testRankedPairsOnDuel("test/ressource/duel_4.csv",4)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 5...");
    if(!testRankedPairsOnDuel("test/ressource/duel_5.csv",5)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 6...");
    if(!testRankedPairsOnDuel("test/ressource/duel_6.csv",6)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 7...");
    if(!testRankedPairsOnDuel("test/ressource/duel_7.csv",7)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur duel 10...");
    if(!testRankedPairsOnDuel("test/ressource/duel_10.csv",10)) return false;
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

    
    afterAll();

    return return_value;

}