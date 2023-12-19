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
    =======================
    === DONNEES DE TEST ===
    =======================
*/

#define NB_BALE 20
#define MAX_NB_WINNER 10

unsigned nb_winners_ref[NB_BALE] = {0,0,0,4,3,1,0,1,0,0,2,1,1};
char label_winners_ref[NB_BALE][MAX_NB_WINNER][MAX_LENGHT_LABEL] = {
    {""},{""},{""},
    {"C1","C2","C3","C4"},   // 3
    {"C1","C2","C3"},        // 4
    {"C1"},                  // 5 
    {""},
    {"C1"},                  // 7
    {""},{""},
    {"C1","C2"},             // 10 
    {"C1"},                  // 11
    {"C2"}                   // 12

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

bool verifResultMajorityJudgment(GenList* result, unsigned num_test) {
#ifdef DEBUG
    if(num_test >= NB_BALE) exitl("test_condorcet_schulze", "", EXIT_FAILURE, "num_test invalide %d >= %d", num_test, NB_BALE);
#endif
    unsigned nb_winners = genListSize(result);
    if(nb_winners != nb_winners_ref[num_test]) return echecTest("Nombre de gagnants différent");
    
    WinnerMajorityJudgment *wtmp;
    for(unsigned i = 0; i < nb_winners; i++) {
        wtmp = (WinnerMajorityJudgment*)genListGet(result, i);
        if(strcmp(wtmp->name, label_winners_ref[num_test][i])) return echecTest("mauvais gagnant");
    }

    return true;
}





bool testMajorityJudgmentOnDuel(char* file, unsigned num_test, bool is_bale_judgment) {
    GenList* lwinner;
    Bale* bale;
    
    /* chargment du bale */
    printsb("\n\t- chargement ballot");
    bale = csvToBale(file);

    /* calcul des gagnants */
    printsb("\n\t- calcul");
    lwinner = theWinnerMajorityJudgment(bale, is_bale_judgment);

    /* vérification des résultats */
    if(!lwinner) return echecTest("pointeur null\n");
    if(!verifResultMajorityJudgment(lwinner, num_test)) {
        printl("\n\n<+>---------------------------- [ %s ] :\n\n", file);
        displayBaleLog(bale);
        displayListWinnerMajorityJudgment(lwinner);
        return false;
    }

    /* libération de la mémoire */
    deleteBale(&bale);
    while(!genListEmpty(lwinner))
        free(genListPop(lwinner));
    deleteGenList(&lwinner);
    return true;
}

bool testTheWinnerMajorityJudgment() {

    printsb("\ntest sur bale 3...");
    if(!testMajorityJudgmentOnDuel("test/ressource/bale_3.csv",3,false)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 4...");
    if(!testMajorityJudgmentOnDuel("test/ressource/bale_4.csv", 4,false)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 5...");
    if(!testMajorityJudgmentOnDuel("test/ressource/bale_5.csv", 5,false)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 7...");
    if(!testMajorityJudgmentOnDuel("test/ressource/bale_7.csv", 7,false)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 10...");
    if(!testMajorityJudgmentOnDuel("test/ressource/bale_10.csv", 10,false)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 11...");
    if(!testMajorityJudgmentOnDuel("test/ressource/bale_11.csv", 11,false)) return false;
    printsb( "\n\t- test passé\n");

    printsb("\ntest sur bale 12...");
    if(!testMajorityJudgmentOnDuel("test/ressource/bale_12.csv", 12,true)) return false;
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