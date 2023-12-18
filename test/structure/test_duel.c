/**
 * @file test_duel.c
 * @author Ugo VALLAT
 * @brief Test sur la structure de données duel
 * 
 * @remarks Les fonction suivantes ne sont pas explicitement testé mais sont
 * utilisée dans les tests des autres fonctions : 
 * - deleteDuel
 * - duelGetValue
 * - duelNbCandidat
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/structure/duel.h"
#include "label_test_set.h"
#include "../../src/utils/csv_reader.h"

/* dimension de la matrice de test */
#define NB_CANDIDAT NB_LABELS    /* NB_CANDIDAT > 2 */
#define OTHER_VALUE 2   /* OTHER_VALUE != DEFAULT_VALUE */



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


/*
    ====================
    === OUTILS DUELS ===
    ====================
*/

/**
 * @author Ugo VALLAT
 * @date 15/11/2023
 * @brief Crée une matrice(l,c) dont chaque case(i,j) = i*c+j 
 * 
 * @return nouvelle matrice
 */
Duel* newUniqueDuel(unsigned int nbc, GenList* labels) {
    Duel *d = createDuel(nbc, labels);
    for(unsigned l = 0; l < nbc; l++)
        for(unsigned c = 0; c < nbc; c++)
            duelSetValue(d, l, c, l*nbc+c);
    return d;
}




/*
    =============
    === TESTS ===
    =============
*/

bool testCreateDuel() {
    Duel *d;
    GenList *labels;

    /* test duel taille (0,0) */
    printsb( "\ntest sur duel taille (0,0)...");
    labels = createGenList(0);
    d = createDuel(0, labels);
    if(!d) return echecTest("pointeur null");
    if(duelNbCandidat(d) != 0) return echecTest("nbCandidat != 0");
    printsb( "\n\t- test passé\n");
    deleteDuel(&d);
    deleteGenList(&labels);

    /* test duel taille (NB_CANDIDAT,NB_CANDIDAT) */
    printsb( "\ntest sur duel taille (NB_CANDIDAT,NB_CANDIDAT)...");
    labels = loadLabelsInList(1);
    d = createDuel(NB_CANDIDAT, labels);
    if(!d) return echecTest("pointeur null");
    if(duelNbCandidat(d) != NB_CANDIDAT) return echecTest("nbCandidat != NB_CANDIDAT");
    printsb( "\n\t- test passé\n");
    deleteDuel(&d);
    deleteGenList(&labels);

    return true;
}

bool testDuelSetValue() {
    Duel *d;
    GenList *labels;

    printsb("\ntest set");
    labels = loadLabelsInList(1);
    d = newUniqueDuel(NB_CANDIDAT, labels);

    if(!d) return echecTest("pointeur null");
    for(unsigned l = 0; l<NB_CANDIDAT; l++)
        for(unsigned c = 0; c<NB_CANDIDAT; c++)
            if(duelGetValue(d, l, c) != (int)(l*NB_CANDIDAT+c)) return echecTest("valeur différente");
    printsb("\n\t - test pâssé");

    deleteDuel(&d);
    deleteGenList(&labels);

    return true;
}


bool checkLabels(unsigned id_label) {
    Duel *d;
    GenList* labels;
    char* candidat_label;
    labels = loadLabelsInList(id_label);

    if(!labels) return echecTest("Erreur chargement labels");
    d = createDuel(NB_CANDIDAT, labels);
    for(unsigned i = 0; i < NB_CANDIDAT; i++) {
        candidat_label = duelIndexToLabel(d, i);
        if(strcmp(candidat_label, genListGet(labels, i)) != 0) return echecTest("\n X-- labels différents");
        free(candidat_label);
    }
    deleteGenList(&labels);
    deleteDuel(&d);
    return true;
}

bool testDuelIndexToLabel() {
    printsb( "\ntest index to label ...");
    if(!checkLabels(1)) return echecTest("Echec avec label1");
    if(!checkLabels(2)) return echecTest("Echec avec label2");
    if(!checkLabels(3)) return echecTest("Echec avec label3");
    if(!checkLabels(4)) return echecTest("Echec avec label4");
    if(!checkLabels(5)) return echecTest("Echec avec label5");
    printsb( "\n\t- test passé\n");

    return true;
}


bool searchLabels(unsigned id_label) {
    Duel* d;
    GenList* labels;
    int index;

    labels = loadLabelsInList(id_label);
    if(!labels) return echecTest("Erreur chargement labels");

    d = createDuel(NB_CANDIDAT, labels);
    for(unsigned i = 0; i < NB_CANDIDAT; i++) {
        index = duelLabelToIndex(d, genListGet(labels, i));
        if(index == -1) return echecTest("label introuvable");
        if(index != (int)i) return echecTest("labels différents");
    }
    deleteGenList(&labels);
    deleteDuel(&d);
    return true;
}

bool testDuelLabelToIndex () {
    printsb( "\ntest index de label ...");
    if(!searchLabels(1)) return echecTest("Echec avec label1");
    if(!searchLabels(2)) return echecTest("Echec avec label2");
    if(!searchLabels(3)) return echecTest("Echec avec label3");
    if(!searchLabels(4)) return echecTest("Echec avec label4");
    if(!searchLabels(5)) return echecTest("Echec avec label5");
    printsb( "\n\t- test passé\n");

    return true;
}


bool convertionBaleToDuel(char* path_bale, char* path_duel_ref) {
    Duel* d_ref, *d_frome_b;
    Bale* b;
    unsigned nb_candidats;
    char* label, *label_ref;

    /* chargment de la donnée */
    b = csvToBale(path_bale);
    if(!b) return echecTest("Echec chargement bale 8");
    d_ref = csvToDuel(path_duel_ref);
    if(!d_ref) return echecTest("Echec chargement duel of bale 8");
    printsb("\n chargement de la donnée réussi...");

    /* convertion bale to duel */
    d_frome_b = duelFromBale(b);
    if(!d_frome_b) return echecTest("Pointeur null");
    
    /* test nouveau duel */
    nb_candidats = duelNbCandidat(d_frome_b);
    if(nb_candidats != duelNbCandidat(d_ref)) return echecTest("taille différente");
    for(unsigned l = 0; l < nb_candidats; l++) {
        /* test labels */
        label_ref = duelIndexToLabel(d_ref, l);
        label = duelIndexToLabel(d_frome_b, l);
        if(strcmp(label, label_ref) != 0) return echecTest("label différent");
        free(label);
        free(label_ref);

        /* test valeurs */
        for(unsigned c = 0; c < nb_candidats; c++) {
            if(duelGetValue(d_ref, l, c) != duelGetValue(d_frome_b, l, c))
                return echecTest("Valeur différente");
        }
    }

    /* libération de la mémoire */
    deleteBale(&b);
    deleteDuel(&d_ref);
    deleteDuel(&d_frome_b);
    printsb( "\n\t- test passé\n");

    return true;
}


bool testDuelFromBale () {
    

    printsb( "\ntest sur bale  8...");
    if(!convertionBaleToDuel("test/ressource/bale_8.csv", "test/ressource/duel_of_bale_8.csv")) return false;
    printsb( "\n\t- test passé\n");

    printsb( "\ntest sur bale  9...");
    if(!convertionBaleToDuel("test/ressource/bale_9.csv", "test/ressource/duel_of_bale_9.csv")) return false;
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

    test_fun(testCreateDuel, 1, "testCreateDuel");
    test_fun(testDuelSetValue, 2, "testDuelSetValue");
    test_fun(testDuelLabelToIndex, 4, "testDuelLabelToIndex");
    test_fun(testDuelIndexToLabel, 8, "testDuelIndexToLabel");
    test_fun(testDuelFromBale, 8, "testDuelFromBale");




    afterAll();

    return return_value;

}