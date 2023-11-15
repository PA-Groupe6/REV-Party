/**
 * @file test_matrix.c
 * @author Ugo VALLAT
 * @brief Test sur la structure de données matrice
 * 
 * @remarks Les fonction suivantes ne sont pas explicitement testé mais sont
 * utilisée dans les tests des autres fonctions : 
 * - deleteMatrix
 * - matrixGet
 * - matrixNbLines
 * - matrixNbColumn
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/structure/matrix.h"

/* dimension de la matrice de test */
#define NB_LINE 10
#define NB_COLUMN 20
#define DEFAULT_VALUE -1
#define OTHER_VALUE 2


StringBuilder* string_builder;
int return_value;

void segfault_handler() {

}

bool testCreateMatrix() {
    emptyStringBuilder(string_builder);
    addLineStringBuilder(string_builder, "\n <+>--- Test Matrix Create ---<+>");

    /* test matrice taille (0,0) */
    addLineStringBuilder(string_builder, "\ntest sur liste taille (0,0)...");
    Matrix* m1 = createMatrix(0, 0, DEFAULT_VALUE);
    if(!m1) return false;
    if((int)matrixNbLines(m1) != 0) return false;
    if((int)matrixNbColonnes(m1) != 0) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m1);

    /* test matrice taille (NB_COLUMN,0) */
    addLineStringBuilder(string_builder, "\ntest sur liste taille (NB_COLUMN,0)...");
    Matrix* m2 = createMatrix(NB_COLUMN, 0, DEFAULT_VALUE);
    if(!m2) return false;
    if((int)matrixNbLines(m2) != NB_COLUMN) return false;
    if((int)matrixNbColonnes(m2) != 0) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m2);

    /* test matrice taille (0,NB_COLUMN) */
    addLineStringBuilder(string_builder, "\ntest sur liste taille (0,NB_COLUMN)...");
    Matrix* m3 = createMatrix(0, NB_COLUMN, DEFAULT_VALUE);
    if(!m3) return false;
    if((int)matrixNbLines(m3) != 0) return false;
    if((int)matrixNbColonnes(m3) != NB_COLUMN) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m3);

    /* test matrice taille (NB_COLUMN,NB_COLUMN) */
    addLineStringBuilder(string_builder, "\ntest sur liste taille (NB_COLUMN,NB_COLUMN)...");
    Matrix* m4 = createMatrix(NB_COLUMN, NB_COLUMN, DEFAULT_VALUE);
    if(!m4) return false;
    if((int)matrixNbLines(m4) != NB_COLUMN) return false;
    if((int)matrixNbColonnes(m4) != NB_COLUMN) return false;
    for(int l = 0; l < NB_COLUMN; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            if(matrixGet(m4, l, c) != DEFAULT_VALUE) return false;
        }
    }
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m4);

    return true;
}

bool testMatrixSet() {
    emptyStringBuilder(string_builder);
    addLineStringBuilder(string_builder, "\n <+>--- Test Matrix Set ---<+>");

    /* Créer matrice */
    Matrix* m = createMatrix(NB_LINE, NB_COLUMN, DEFAULT_VALUE);

    addLineStringBuilder(string_builder, "\ntest valeur par défaut...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            if(matrixGet(m, l, c) != DEFAULT_VALUE) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    addLineStringBuilder(string_builder, "\ntest set toutes les valeur à 0...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, 0);
            if(matrixGet(m, l, c) != 0) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    addLineStringBuilder(string_builder, "\ntest set toutes les valeur à -99...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, -99);
            if(matrixGet(m, l, c) != -99) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m);

    return true;
}

bool testMatrixErase() {
    emptyStringBuilder(string_builder);
    addLineStringBuilder(string_builder, "\n <+>--- Test Matrix Erase ---<+>");

    /* Créer matrice */
    Matrix* m = createMatrix(NB_LINE, NB_COLUMN, DEFAULT_VALUE);

    addLineStringBuilder(string_builder, "\ntest effacer valeur par défaut...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixErase(m, l, c);
            if(matrixGet(m, l, c) != DEFAULT_VALUE) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    addLineStringBuilder(string_builder, "\ntest effacer valeur autre...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, l+c);
            matrixErase(m, l, c);
            if(matrixGet(m, l, c) != DEFAULT_VALUE) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    return true;
}


bool testMatrixInsertRemoveLine() {
    emptyStringBuilder(string_builder);
    addLineStringBuilder(string_builder, "\n <+>--- Test Matrix Insert/Remove Line ---<+>");

    Matrix* m = createMatrix(0,0,DEFAULT_VALUE);

    /* Insert dans matrice (0,0)*/
    addLineStringBuilder(string_builder, "\ntest insert début matrice (0,0)...");
    for(int l = 0; l < NB_LINE; l++) {
        matrixInsertLine(m, 0);
        if((int)matrixNbLines(m) != l+1) return false;
        if((int)matrixNbColonnes(m) != 0) return false;
    }
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    /* Remove dans matrice (0,0)*/
    addLineStringBuilder(string_builder, "\ntest remove début matrice (NB_LINE,0)...");
    for(int l = NB_LINE; l > 0; l--) {
        matrixRemoveLine(m, 0);
        if((int)matrixNbLines(m) != l-1) return false;
        if((int)matrixNbColonnes(m) != 0) return false;
    }
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m);

    /* Création matrice (NB_LINE,NB_COLUMN) et modification des valeurs */
    m = createMatrix(NB_LINE, NB_COLUMN, DEFAULT_VALUE);
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, OTHER_VALUE);
        }
    }

    /* Insert dans rangs impairs */
    addLineStringBuilder(string_builder, "\ntest insert rang impair matrice (NB_LINE,NB_COLUMN)...");
    for(int l = NB_LINE; l >= 0; l--) {
        matrixInsertLine(m, l);
        if((int)matrixNbLines(m) != NB_LINE + (NB_LINE-l+1)) return false;
        if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    }
    addLineStringBuilder(string_builder, "\n\t- insertion faite, taille correcte\n");
    
    /* Vérification des valeurs */
    int val;
    for(int l = 0; l < 2*NB_LINE+1; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            val = matrixGet(m, l, c);
            if(l%2==0) {
                if(val != DEFAULT_VALUE) return false;
            } else {
                if(val != OTHER_VALUE) return false;
            }
        }
    }
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    /* Remove des rangs impairs */
    addLineStringBuilder(string_builder, "\ntest remove rang impair matrice (2*NB_LINE+1,NB_COLUMN)...");
    for(int l = 0; l <= NB_LINE; l++) {
        matrixRemoveLine(m, l);
        if((int)matrixNbLines(m) != (2*NB_LINE-l)) return false;
        if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    }
    addLineStringBuilder(string_builder, "\n\t- remove fait, taille correcte\n");
    
    /* Vérification des valeurs */
    for(unsigned int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            if(matrixGet(m, l, c) != OTHER_VALUE) return false;
        }
    }
    deleteMatrix(&m);
    return true;
}

bool testMatrixInsertRemoveColumn() {
    emptyStringBuilder(string_builder);
    addLineStringBuilder(string_builder, "\n <+>--- Test Matrix Insert/Remove Column ---<+>");

    Matrix* m = createMatrix(0,0,DEFAULT_VALUE);

    /* Insert dans matrice (0,0)*/
    addLineStringBuilder(string_builder, "\ntest insert début matrice (0,0)...");
    for(int c = 0; c < NB_COLUMN; c++) {
        matrixInsertColumn(m, 0);
        if((int)matrixNbLines(m) != 0) return false;
        if((int)matrixNbColonnes(m) != c+1) return false;
    }
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    /* Remove dans matrice (0,0)*/
    addLineStringBuilder(string_builder, "\ntest remove début matrice (0,NB_COLUMN)...");
    for(int c = NB_COLUMN; c > 0; c--) {
        matrixRemoveColumn(m, 0);
        if((int)matrixNbLines(m) != 0) return false;
        if((int)matrixNbColonnes(m) != c-1) return false;
    }
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m);

    /* Création matrice (NB_LINE,NB_COLUMN) et modification des valeurs */
    m = createMatrix(NB_LINE, NB_COLUMN, DEFAULT_VALUE);
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, OTHER_VALUE);
        }
    }

    /* Insert dans rangs impairs */
    addLineStringBuilder(string_builder, "\ntest insert rang impair matrice (NB_LINE,NB_COLUMN)...");
    for(int c = NB_COLUMN; c >= 0; c--) {
        matrixInsertColumn(m, c);
        if((int)matrixNbLines(m) != NB_LINE) return false;
        if((int)matrixNbColonnes(m) != NB_COLUMN + (NB_COLUMN-c+1)) return false;
    }
    addLineStringBuilder(string_builder, "\n\t- insertion faite, taille correcte\n");
    
    /* Vérification des valeurs */
    int val;
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < 2*NB_COLUMN+1; c++) {
            val = matrixGet(m, l, c);
            if(c%2==0) {
                if(val != DEFAULT_VALUE) return false;
            } else {
                if(val != OTHER_VALUE) return false;
            }
        }
    }
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    /* Remove des rangs impairs */
    addLineStringBuilder(string_builder, "\ntest remove rang impair matrice (NB_LINE,2*NB_COLUMN+1)...");
    for(int c = 0; c <= NB_COLUMN; c++) {
        matrixRemoveColumn(m, c);
        if((int)matrixNbLines(m) != NB_LINE) return false;
        if((int)matrixNbColonnes(m) != 2*NB_COLUMN-c) return false;
    }
    addLineStringBuilder(string_builder, "\n\t- remove fait, taille correcte\n");
    
    /* Vérification des valeurs */
    for(unsigned int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            if(matrixGet(m, l, c) != OTHER_VALUE) return false;
        }
    }
    deleteMatrix(&m);
    return true;
}



int main() {
    #ifdef DEBUG
        init_logger(NULL);
    #endif

    string_builder = createStringBuilder();
    return_value = 0;

    if (!testCreateMatrix()) {
        return_value += 1; /* 2^0 */
        printFailure("testCreateMatrix");
        printStringBuilder(string_builder);
    } else printSuccess("testCreateMatrix");

    if (!testMatrixSet()) {
        return_value += 2; /* 2^1 */
        printFailure("testMatrixSet");
        printStringBuilder(string_builder);
    } else printSuccess("testMatrixSet");

    if (!testMatrixErase()) {
        return_value += 4; /* 2^2 */
        printFailure("testMatrixErase");
        printStringBuilder(string_builder);
    } else printSuccess("testMatrixErase");

    if (!testMatrixInsertRemoveLine()) {
        return_value += 8; /* 2^3 */
        printFailure("testMatrixInsertRemoveLine");
        printStringBuilder(string_builder);
    } else printSuccess("testMatrixInsertRemoveLine");

    if (!testMatrixInsertRemoveColumn()) {
        return_value += 16; /* 2^4 */
        printFailure("testMatrixInsertRemoveColumn");
        printStringBuilder(string_builder);
    } else printSuccess("testMatrixInsertRemoveColumn");


    return return_value;

}