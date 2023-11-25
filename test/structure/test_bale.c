/**
 * @file test_bale.c
 * @author Ugo VALLAT
 * @brief Test sur la structure de données matrice
 * 
 * @remarks Les fonction suivantes ne sont pas explicitement testé mais sont
 * utilisée dans les tests des autres fonctions : 
 * - deleteBale
 * - baleGet
 * - baleNbLines
 * - baleNbColumn
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/structure/bale.h"
#include "label_test_set.h"

/* dimension de la matrice de test */
#define NB_VOTER 20     /* NB_VOTER > 2 */
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
    printsb(msg);
    return false;
}


/*
    =======================
    === OUTILS MATRICES ===
    =======================
*/

/**
 * @author Ugo VALLAT
 * @date 15/11/2023
 * @brief Crée une matrice(l,c) dont chaque case(i,j) = i*c+j 
 * 
 * @return nouvelle matrice
 */
Bale* newUniqueBale(unsigned int nbl, unsigned int nbc, GenList* labels) {
    Bale* b = createBale(nbl, nbc, labels);
    for(unsigned int l = 0; l < nbl; l++) {
        for(unsigned int c = 0; c < nbc; c++) {
            baleSetValue(b, l, c, l*nbc+c);
        }
    }
    return b;
}

bool testUniqueBaleWithIte(Bale* m,int l, int c, unsigned nbl, unsigned nbc) {
    if(!m) return false;
    /* Creation de l'iterateur */
    BaleIte *ite = createBaleIte(m, l, c, NULL, NULL);
    if(!ite){
        printsb(" <+>--- echec cration ite\n");
        return false;
    }
    /* initialisation paramêtres */
    int line_size = nbc;
    int i,j, imax, jmax;
    (l==-1 || l+1 > (int)nbl) ? (imax = nbl) : (imax = l+1);
    (c==-1 || c+1 > (int)nbc) ? (jmax = nbc) : (jmax = c+1);

    /* parcours */
    (l==-1) ? (i = 0) : (i = l);
    for(; i < imax; i++) {
        for(((c == -1)?(j = 0):(j = c)); j < jmax; j++) {
            if(!baleIteHasNext(ite)){
                printsb(" <+>--- echec has next\n");
                return false;
            }
            if(baleIteNext(ite) != (int)(i*line_size+j)) {
                printsb(" <+>--- echec next\n");
                return false;
            }
            if(baleIteGetValue(ite) != (int)(i*line_size+j)) {
                printsb(" <+>--- echec get value\n");
                return false;
            }
        }
    }

    /* Fin du parcours */
    if(baleIteHasNext(ite)) {
        printsb(" <+>--- echec next fin\n");
        return false;
    }
    deleteBaleIte(&ite);
    return true;
}



/*
    =============
    === TESTS ===
    =============
*/

bool testCreateBale() {
    GenList* labels;
    Bale* b;

    /* test ballot taille (0,0) */
    printsb( "\ntest sur ballot taille (0,0)...");
    labels = createGenList(0);
    b = createBale(0, 0, labels);
    if(!b) return echecTest("\n X-- pointeur null");
    if((int)baleNbCandidat(b) != 0) return echecTest("\n X-- erreur nombre candidats");
    if((int)baleNbVoter(b) != 0) return echecTest("\n X-- erreur nombre votants");
    printsb( "\n\t- test passé\n");
    deleteBale(&b);
    deleteGenList(&labels);

    /* test ballot taille (NB_VOTER,0) */
    printsb( "\ntest sur ballot taille (NB_VOTER,0)...");
    labels = createGenList(0);
    b = createBale(NB_VOTER, 0, labels);
    if(!b) return echecTest("\n X-- pointeur null");
    if((int)baleNbCandidat(b) != 0) return echecTest("\n X-- erreur nombre candidats");
    if((int)baleNbVoter(b) != NB_VOTER) return echecTest("\n X-- erreur nombre votants");
    printsb( "\n\t- test passé\n");
    deleteBale(&b);
    deleteGenList(&labels);

    /* test ballot taille (0,NB_CANDIDAT) */
    printsb( "\ntest sur ballot taille (0,NB_CANDIDAT)...");
    labels = loadLabelsInList(1);
    b = createBale(0, NB_CANDIDAT, labels);
    if(!b) return echecTest("\n X-- pointeur null");
    if((int)baleNbCandidat(b) != NB_CANDIDAT) return echecTest("\n X-- erreur nombre candidats");
    if((int)baleNbVoter(b) != 0) return echecTest("\n X-- erreur nombre votants");
    printsb( "\n\t- test passé\n");
    deleteBale(&b);
    deleteGenList(&labels);

    /* test ballot taille (NB_VOTER,NB_CANDIDAT) */
    printsb( "\ntest sur ballot taille (NB_VOTER,NB_CANDIDAT)...");
    labels = loadLabelsInList(1);
    b = createBale(NB_VOTER, NB_CANDIDAT, labels);
    if(!b) return echecTest("\n X-- pointeur null");
    if((int)baleNbCandidat(b) != NB_CANDIDAT) return echecTest("\n X-- erreur nombre candidats");
    if((int)baleNbVoter(b) != NB_VOTER) return echecTest("\n X-- erreur nombre votants");
    for(unsigned l = 0; l < NB_VOTER; l++)
        for(unsigned c = 0; c < NB_CANDIDAT; c++)
            if(baleGetValue(b, l, c) != DEFAULT_VALUE) return echecTest("\n X-- valeur incorrecte");
    printsb( "\n\t- test passé\n");
    deleteBale(&b);
    deleteGenList(&labels);

    return true;
}

bool testSetToValue(int value, unsigned id_label) {
    GenList* labels;
    Bale* b;
    labels = loadLabelsInList(id_label);
    if(!labels) return echecTest("\n X-- Erreur chargement labels");
    b = createBale(NB_VOTER, NB_CANDIDAT, labels);
    for(int l = 0; l < NB_VOTER; l++) {
        for(int c = 0; c < NB_CANDIDAT; c++) {
            baleSetValue(b, l, c, value);
            if(baleGetValue(b, l, c) != value) return echecTest("\n X-- valeur incorrecte");
        }
    }
    if((int)baleNbVoter(b) != NB_VOTER) return echecTest("\n X-- erreur nombre votants");
    if((int)baleNbCandidat(b) != NB_CANDIDAT) return echecTest("\n X-- erreur nombre candidats");
    deleteBale(&b);
    deleteGenList(&labels);
    return true;
}

bool testBaleSet() {
    
    printsb( "\ntest set toutes les valeur à 0...");
    if(!testSetToValue(0,1)) return echecTest("\n X-- Echec avec label1");
    if(!testSetToValue(0, 2)) return echecTest("\n X-- Echec avec label2");
    if(!testSetToValue(0, 3)) return echecTest("\n X-- Echec avec label3");
    if(!testSetToValue(0, 4)) return echecTest("\n X-- Echec avec label4");
    if(!testSetToValue(0, 5)) return echecTest("\n X-- Echec avec label5");
    printsb( "\n\t- test passé\n");

    printsb( "\ntest set toutes les valeur à -1 ...");
    if(!testSetToValue(-1, 1)) return echecTest("\n X-- Echec avec label1");
    if(!testSetToValue(-1, 2)) return echecTest("\n X-- Echec avec label2");
    if(!testSetToValue(-1, 3)) return echecTest("\n X-- Echec avec label3");
    if(!testSetToValue(-1, 4)) return echecTest("\n X-- Echec avec label4");
    if(!testSetToValue(-1, 5)) return echecTest("\n X-- Echec avec label5");
    printsb( "\n\t- test passé\n");

    return true;
}

bool checkLabels(unsigned id_label) {
    GenList* labels;
    Bale* b;
    char* candidat_label;
    labels = loadLabelsInList(id_label);
    if(!labels) return echecTest("\n X-- Erreur chargement labels");
    b = createBale(NB_VOTER, NB_CANDIDAT, labels);
    for(unsigned i = 0; i < NB_CANDIDAT; i++) {
        candidat_label = baleColumnToLabel(b, i);
        if(strcmp(candidat_label, genListGet(labels, i)) != 0) return echecTest("\n X-- labels différents");
        free(candidat_label);
    }
    deleteGenList(&labels);
    deleteBale(&b);
    return true;
}

bool testBaleColumnToLabel() {
    printsb( "\ntest label de colonne ...");
    if(!checkLabels(1)) return echecTest("\n X-- Echec avec label1");
    if(!checkLabels(2)) return echecTest("\n X-- Echec avec label2");
    if(!checkLabels(3)) return echecTest("\n X-- Echec avec label3");
    if(!checkLabels(4)) return echecTest("\n X-- Echec avec label4");
    if(!checkLabels(5)) return echecTest("\n X-- Echec avec label5");
    printsb( "\n\t- test passé\n");

    return true;
}

bool searchLabels(unsigned id_label) {
    GenList* labels;
    Bale* b;
    int col;
    labels = loadLabelsInList(id_label);
    if(!labels) return echecTest("\n X-- Erreur chargement labels");
    b = createBale(NB_VOTER, NB_CANDIDAT, labels);
    for(unsigned i = 0; i < NB_CANDIDAT; i++) {
        col = baleLabelToColumn(b, genListGet(labels, i));
        if(col == -1) return echecTest("\n X-- label introuvable");
        if(col != (int)i) return echecTest("\n X-- labels différents");
    }
    deleteGenList(&labels);
    deleteBale(&b);
    return true;
}

bool testBaleLabelToColumn () {
    printsb( "\ntest colonne de label ...");
    if(!searchLabels(1)) return echecTest("\n X-- Echec avec label1");
    if(!searchLabels(2)) return echecTest("\n X-- Echec avec label2");
    if(!searchLabels(3)) return echecTest("\n X-- Echec avec label3");
    if(!searchLabels(4)) return echecTest("\n X-- Echec avec label4");
    if(!searchLabels(5)) return echecTest("\n X-- Echec avec label5");
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

    test_fun(testCreateBale, 1, "testCreateBale");
    test_fun(testBaleSet, 2, "testBaleSet");
    test_fun(testBaleColumnToLabel, 1, "testBaleColumnToLabel");
    test_fun(testBaleLabelToColumn, 1, "testBaleLabelToColumn");


    afterAll();

    return return_value;

}