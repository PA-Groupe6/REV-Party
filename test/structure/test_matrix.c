#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/structure/matrix.h"


StringBuilder* string_builder;
int return_value;

void segfault_handler() {

}

bool testCreateMatrix() {
    emptyStringBuilder(string_builder);
    addLineStringBuilder(string_builder, "\n <+>--- Test Matrix Create ---<+>");

    /* test matrice taille (0,0) */
    addLineStringBuilder(string_builder, "\ntest sur liste taille (0,0)...");
    Matrix* m1 = createMatrix(0, 0, -1);
    if(!m1) return false;
    if(matrixNbLines(m1) != 0) return false;
    if(matrixNbColonnes(m1) != 0) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m1);

    /* test matrice taille (10,0) */
    addLineStringBuilder(string_builder, "\ntest sur liste taille (10,0)...");
    Matrix* m2 = createMatrix(10, 0, -1);
    if(!m2) return false;
    if(matrixNbLines(m2) != 10) return false;
    if(matrixNbColonnes(m2) != 0) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m2);

    /* test matrice taille (0,10) */
    addLineStringBuilder(string_builder, "\ntest sur liste taille (0,10)...");
    Matrix* m3 = createMatrix(0, 10, -1);
    if(!m3) return false;
    if(matrixNbLines(m3) != 0) return false;
    if(matrixNbColonnes(m3) != 10) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m3);

    /* test matrice taille (10,10) */
    addLineStringBuilder(string_builder, "\ntest sur liste taille (10,10)...");
    Matrix* m4 = createMatrix(10, 10, -1);
    if(!m4) return false;
    if(matrixNbLines(m4) != 10) return false;
    if(matrixNbColonnes(m4) != 10) return false;
    for(int l = 0; l < 10; l++) {
        for(int c = 0; c < 10; c++) {
            if(matrixGet(m4, l, c) != -1) return false;
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
    Matrix* m = createMatrix(5, 10, -1);

    addLineStringBuilder(string_builder, "\ntest valeur par défaut...");
    for(int l = 0; l < 5; l++) {
        for(int c = 0; c < 10; c++) {
            if(matrixGet(m, l, c) != -1) return false;
        }
    }
    if(matrixNbLines(m) != 5) return false;
    if(matrixNbColonnes(m) != 10) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    addLineStringBuilder(string_builder, "\ntest set toutes les valeur à 0...");
    for(int l = 0; l < 5; l++) {
        for(int c = 0; c < 10; c++) {
            matrixSet(m, l, c, 0);
            if(matrixGet(m, l, c) != 0) return false;
        }
    }
    if(matrixNbLines(m) != 5) return false;
    if(matrixNbColonnes(m) != 10) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    addLineStringBuilder(string_builder, "\ntest set toutes les valeur à -99...");
    for(int l = 0; l < 5; l++) {
        for(int c = 0; c < 10; c++) {
            matrixSet(m, l, c, -99);
            if(matrixGet(m, l, c) != -99) return false;
        }
    }
    if(matrixNbLines(m) != 5) return false;
    if(matrixNbColonnes(m) != 10) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");
    deleteMatrix(&m);

    return true;
}

bool testMatrixErase() {
    emptyStringBuilder(string_builder);
    addLineStringBuilder(string_builder, "\n <+>--- Test Matrix Erase ---<+>");

    /* Créer matrice */
    Matrix* m = createMatrix(5, 10, -1);

    addLineStringBuilder(string_builder, "\ntest effacer valeur par défaut...");
    for(int l = 0; l < 5; l++) {
        for(int c = 0; c < 10; c++) {
            matrixErase(m, l, c);
            if(matrixGet(m, l, c) != -1) return false;
        }
    }
    if(matrixNbLines(m) != 5) return false;
    if(matrixNbColonnes(m) != 10) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

    addLineStringBuilder(string_builder, "\ntest effacer valeur autre...");
    for(int l = 0; l < 5; l++) {
        for(int c = 0; c < 10; c++) {
            matrixSet(m, l, c, l+c);
            matrixErase(m, l, c);
            if(matrixGet(m, l, c) != -1) return false;
        }
    }
    if(matrixNbLines(m) != 5) return false;
    if(matrixNbColonnes(m) != 10) return false;
    addLineStringBuilder(string_builder, "\n\t- test passé\n");

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


    return return_value;

}