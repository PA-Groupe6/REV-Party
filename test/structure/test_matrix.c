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
#define NB_LINE 10      /* NB_LINE > 2 */
#define NB_COLUMN 20    /* NB_COLUMN > 2 */
#define DEFAULT_VALUE -1
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
}

void beforeEach() {
    emptyStringBuilder(string_builder);
}

void afterEach() {
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
Matrix* newUniqueMatrix(unsigned int nbl, unsigned int nbc) {
    Matrix* m = createMatrix(nbl, nbc, DEFAULT_VALUE);
    for(unsigned int l = 0; l < nbl; l++) {
        for(unsigned int c = 0; c < nbc; c++) {
            matrixSet(m, l, c, l*nbc+c);
        }
    }
    return m;
}

bool testUniqueMatrixWithIte(Matrix* m,int l, int c, unsigned nbl, unsigned nbc) {
    if(!m) return false;
    /* Creation de l'iterateur */
    MatrixIte *ite = createMatrixIte(m, l, c, NULL, NULL);
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
            if(!matrixIteHasNext(ite)){
                printsb(" <+>--- echec has next\n");
                return false;
            }
            if(matrixIteNext(ite) != (int)(i*line_size+j)) {
                printsb(" <+>--- echec next\n");
                return false;
            }
            if(matrixIteGetValue(ite) != (int)(i*line_size+j)) {
                printsb(" <+>--- echec get value\n");
                return false;
            }
        }
    }

    /* Fin du parcours */
    if(matrixIteHasNext(ite)) {
        printsb(" <+>--- echec next fin\n");
        return false;
    }
    deleteMatrixIte(&ite);
    return true;
}



/*
    =============
    === TESTS ===
    =============
*/

bool testCreateMatrix() {
    emptyStringBuilder(string_builder);
    printsb("\n <+>--- Test Matrix Create ---<+>");

    /* test matrice taille (0,0) */
    printsb( "\ntest sur liste taille (0,0)...");
    Matrix* m1 = createMatrix(0, 0, DEFAULT_VALUE);
    if(!m1) return false;
    if((int)matrixNbLines(m1) != 0) return false;
    if((int)matrixNbColonnes(m1) != 0) return false;
    printsb( "\n\t- test passé\n");
    deleteMatrix(&m1);

    /* test matrice taille (NB_COLUMN,0) */
    printsb( "\ntest sur liste taille (NB_COLUMN,0)...");
    Matrix* m2 = createMatrix(NB_COLUMN, 0, DEFAULT_VALUE);
    if(!m2) return false;
    if((int)matrixNbLines(m2) != NB_COLUMN) return false;
    if((int)matrixNbColonnes(m2) != 0) return false;
    printsb( "\n\t- test passé\n");
    deleteMatrix(&m2);

    /* test matrice taille (0,NB_COLUMN) */
    printsb( "\ntest sur liste taille (0,NB_COLUMN)...");
    Matrix* m3 = createMatrix(0, NB_COLUMN, DEFAULT_VALUE);
    if(!m3) return false;
    if((int)matrixNbLines(m3) != 0) return false;
    if((int)matrixNbColonnes(m3) != NB_COLUMN) return false;
    printsb( "\n\t- test passé\n");
    deleteMatrix(&m3);

    /* test matrice taille (NB_COLUMN,NB_COLUMN) */
    printsb( "\ntest sur liste taille (NB_COLUMN,NB_COLUMN)...");
    Matrix* m4 = createMatrix(NB_COLUMN, NB_COLUMN, DEFAULT_VALUE);
    if(!m4) return false;
    if((int)matrixNbLines(m4) != NB_COLUMN) return false;
    if((int)matrixNbColonnes(m4) != NB_COLUMN) return false;
    for(int l = 0; l < NB_COLUMN; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            if(matrixGet(m4, l, c) != DEFAULT_VALUE) return false;
        }
    }
    printsb( "\n\t- test passé\n");
    deleteMatrix(&m4);

    return true;
}

bool testMatrixSet() {
    emptyStringBuilder(string_builder);
    printsb( "\n <+>--- Test Matrix Set ---<+>");

    /* Créer matrice */
    Matrix* m = createMatrix(NB_LINE, NB_COLUMN, DEFAULT_VALUE);

    printsb( "\ntest valeur par défaut...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            if(matrixGet(m, l, c) != DEFAULT_VALUE) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    printsb( "\n\t- test passé\n");

    printsb( "\ntest set toutes les valeur à 0...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, 0);
            if(matrixGet(m, l, c) != 0) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    printsb( "\n\t- test passé\n");

    printsb( "\ntest set toutes les valeur à -99...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, -99);
            if(matrixGet(m, l, c) != -99) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    printsb( "\n\t- test passé\n");
    deleteMatrix(&m);

    return true;
}

bool testMatrixErase() {
    emptyStringBuilder(string_builder);
    printsb( "\n <+>--- Test Matrix Erase ---<+>");

    /* Créer matrice */
    Matrix* m = createMatrix(NB_LINE, NB_COLUMN, DEFAULT_VALUE);

    printsb( "\ntest effacer valeur par défaut...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixErase(m, l, c);
            if(matrixGet(m, l, c) != DEFAULT_VALUE) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    printsb( "\n\t- test passé\n");

    printsb( "\ntest effacer valeur autre...");
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, l+c);
            matrixErase(m, l, c);
            if(matrixGet(m, l, c) != DEFAULT_VALUE) return false;
        }
    }
    if((int)matrixNbLines(m) != NB_LINE) return false;
    if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    printsb( "\n\t- test passé\n");

    deleteMatrix(&m);
    return true;
}


bool testMatrixInsertRemoveLine() {
    emptyStringBuilder(string_builder);
    printsb( "\n <+>--- Test Matrix Insert/Remove Line ---<+>");

    Matrix* m = createMatrix(0,0,DEFAULT_VALUE);

    /* Insert dans matrice (0,0)*/
    printsb( "\ntest insert début matrice (0,0)...");
    for(int l = 0; l < NB_LINE; l++) {
        matrixInsertLine(m, 0);
        if((int)matrixNbLines(m) != l+1) return false;
        if((int)matrixNbColonnes(m) != 0) return false;
    }
    printsb( "\n\t- test passé\n");

    /* Remove dans matrice (0,0)*/
    printsb( "\ntest remove début matrice (NB_LINE,0)...");
    for(int l = NB_LINE; l > 0; l--) {
        matrixRemoveLine(m, 0);
        if((int)matrixNbLines(m) != l-1) return false;
        if((int)matrixNbColonnes(m) != 0) return false;
    }
    printsb( "\n\t- test passé\n");
    deleteMatrix(&m);

    /* Création matrice (NB_LINE,NB_COLUMN) et modification des valeurs */
    m = createMatrix(NB_LINE, NB_COLUMN, DEFAULT_VALUE);
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, OTHER_VALUE);
        }
    }

    /* Insert dans rangs impairs */
    printsb( "\ntest insert rang impair matrice (NB_LINE,NB_COLUMN)...");
    for(int l = NB_LINE; l >= 0; l--) {
        matrixInsertLine(m, l);
        if((int)matrixNbLines(m) != NB_LINE + (NB_LINE-l+1)) return false;
        if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    }
    printsb( "\n\t- insertion faite, taille correcte\n");
    
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
    printsb( "\n\t- test passé\n");

    /* Remove des rangs impairs */
    printsb( "\ntest remove rang impair matrice (2*NB_LINE+1,NB_COLUMN)...");
    for(int l = 0; l <= NB_LINE; l++) {
        matrixRemoveLine(m, l);
        if((int)matrixNbLines(m) != (2*NB_LINE-l)) return false;
        if((int)matrixNbColonnes(m) != NB_COLUMN) return false;
    }
    printsb( "\n\t- remove fait, taille correcte\n");
    
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
    printsb( "\n <+>--- Test Matrix Insert/Remove Column ---<+>");

    Matrix* m = createMatrix(0,0,DEFAULT_VALUE);

    /* Insert dans matrice (0,0)*/
    printsb( "\ntest insert début matrice (0,0)...");
    for(int c = 0; c < NB_COLUMN; c++) {
        matrixInsertColumn(m, 0);
        if((int)matrixNbLines(m) != 0) return false;
        if((int)matrixNbColonnes(m) != c+1) return false;
    }
    printsb( "\n\t- test passé\n");

    /* Remove dans matrice (0,0)*/
    printsb( "\ntest remove début matrice (0,NB_COLUMN)...");
    for(int c = NB_COLUMN; c > 0; c--) {
        matrixRemoveColumn(m, 0);
        if((int)matrixNbLines(m) != 0) return false;
        if((int)matrixNbColonnes(m) != c-1) return false;
    }
    printsb( "\n\t- test passé\n");
    deleteMatrix(&m);

    /* Création matrice (NB_LINE,NB_COLUMN) et modification des valeurs */
    m = createMatrix(NB_LINE, NB_COLUMN, DEFAULT_VALUE);
    for(int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            matrixSet(m, l, c, OTHER_VALUE);
        }
    }

    /* Insert dans rangs impairs */
    printsb( "\ntest insert rang impair matrice (NB_LINE,NB_COLUMN)...");
    for(int c = NB_COLUMN; c >= 0; c--) {
        matrixInsertColumn(m, c);
        if((int)matrixNbLines(m) != NB_LINE) return false;
        if((int)matrixNbColonnes(m) != NB_COLUMN + (NB_COLUMN-c+1)) return false;
    }
    printsb( "\n\t- insertion faite, taille correcte\n");
    
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
    printsb( "\n\t- test passé\n");

    /* Remove des rangs impairs */
    printsb( "\ntest remove rang impair matrice (NB_LINE,2*NB_COLUMN+1)...");
    for(int c = 0; c <= NB_COLUMN; c++) {
        matrixRemoveColumn(m, c);
        if((int)matrixNbLines(m) != NB_LINE) return false;
        if((int)matrixNbColonnes(m) != 2*NB_COLUMN-c) return false;
    }
    printsb( "\n\t- remove fait, taille correcte\n");
    
    /* Vérification des valeurs */
    for(unsigned int l = 0; l < NB_LINE; l++) {
        for(int c = 0; c < NB_COLUMN; c++) {
            if(matrixGet(m, l, c) != OTHER_VALUE) return false;
        }
    }
    printsb( "\n\t- test passé\n");
    deleteMatrix(&m);
    return true;
}

int fun_for_ite(int v, unsigned l, unsigned c, void* buff) {
    bool* test = (bool*)buff;
    if(v != (int)(l*NB_COLUMN+c)) *test = false;
    return 0;
}

bool testMatrixIte() {
    emptyStringBuilder(string_builder);
    printsb( "\n <+>--- Test Matrix Insert/Remove Column ---<+>");

    Matrix* m;
    MatrixIte *ite;

    /*  
        =================================================
         TEST MATRIX (0,0) / (NB_LINE,0) / (0,NB_COLUMN) 
        =================================================
    */
    /* test itérateur matrice (0,0)*/
    printsb( "\ntest iterateur avec matrice (0,0) ...");
    m = createMatrix(0, 0, -1);
    if(!testUniqueMatrixWithIte(m, -1, -1, 0, 0)) return false;
    deleteMatrix(&m);
    printsb( "\n\t- test passé\n");

    /* test itérateur matrice (NB_LINE,0) */
    printsb( "\ntest iterateur avec matrice (NB_LINE,0) ...");
    m = createMatrix(NB_LINE, 0, -1);
    if(!testUniqueMatrixWithIte(m, -1, -1, NB_LINE, 0)) return false;
    if(!testUniqueMatrixWithIte(m, 1, -1, NB_LINE, 0)) return false;
    deleteMatrix(&m);
    printsb( "\n\t- test passé\n");

    /* test itérateur matrice (0,NB_COLUMN) */
    printsb( "\ntest iterateur avec matrice (0,NB_COLUMN) ...");
    m = createMatrix(0, NB_COLUMN, -1);
    if(!testUniqueMatrixWithIte(m, -1, -1, 0, NB_COLUMN)) return false;
    if(!testUniqueMatrixWithIte(m, -1, 1, 0, NB_COLUMN)) return false;
    deleteMatrix(&m);
    printsb( "\n\t- test passé\n");


    /*  
        =================================
         TEST MATRIX (NB_LINE,NB_COLUMN)  
        =================================
    */
    m = newUniqueMatrix(NB_LINE, NB_COLUMN);

    /* test itérateur sur CASE */
    printsb( "\ntest iterateur sur case avec matrice (NB_LINE,NB_COLUMN) ...");
    for(unsigned l = 0; l < NB_LINE; l++) {
        for(unsigned c = 0; c < NB_COLUMN; c++) {
            if(!testUniqueMatrixWithIte(m, l, c, NB_LINE, NB_COLUMN)) return false;
        }
    }
    printsb( "\n\t- test passé\n");

    /* test itérateur sur LIGNE*/
    printsb( "\ntest iterateur sur ligne avec matrice (NB_LINE,NB_COLUMN)...");
    for(unsigned l = 0; l < NB_LINE; l++) {
        if(!testUniqueMatrixWithIte(m, l, -1, NB_LINE, NB_COLUMN)) return false;
    }
    printsb( "\n\t- test passé\n");


    /* test itérateur sur COLONNE */
    printsb( "\ntest iterateur sur colonne avec matrice (NB_LINE,NB_COLUMN)...");
    for(unsigned c = 0; c < NB_COLUMN; c++) {
        if(!testUniqueMatrixWithIte(m, -1, c, NB_LINE, NB_COLUMN)) return false;
    }
    printsb( "\n\t- test passé\n");

    /* test itérateur sur matrice*/
    printsb( "\ntest iterateur sur matrice avec matrice (NB_LINE,NB_COLUMN)...");
    if(!testUniqueMatrixWithIte(m, -1, -1, NB_LINE, NB_COLUMN)) return false;
    printsb( "\n\t- test passé\n");

    /* test itérateur avec fun */
    bool test = true;
    ite = createMatrixIte(m, -1, -1, fun_for_ite, &test);
    if(!ite) return false;
    for(unsigned l = 0; l < NB_LINE; l++) {
        for(unsigned c = 0; c < NB_COLUMN; c++) {
            if(!matrixIteHasNext(ite)) return false;
            if(matrixIteNext(ite) != (int)(l*NB_COLUMN+c)) return false;
            if(matrixIteGetValue(ite) != 0) return false;
        }
    }
    if(matrixIteHasNext(ite)) return false;
    deleteMatrixIte(&ite);
    if(!test) return false;
    for(unsigned l = 0; l < NB_LINE; l++) {
        for(unsigned c = 0; c < NB_COLUMN; c++) {
            if(matrixGet(m, l, c) != 0) return false;
        }
    }
    deleteMatrix(&m);
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

    test_fun(testCreateMatrix, 1, "testCreateMatrix");
    test_fun(testMatrixSet, 2, "testMatrixSet");
    test_fun(testMatrixErase, 4, "testMatrixErase");
    test_fun(testMatrixInsertRemoveLine, 8, "testMatrixInsertRemoveLine");
    test_fun(testMatrixInsertRemoveColumn, 16, "testMatrixInsertRemoveColumn");
    test_fun(testMatrixIte, 32, "testMatrixIte");

    afterAll();

    return return_value;

}