/**
 * @file test_genericlist.c
 * @author Ugo VALLAT
 * @brief Test sur la structure de données liste générique
 * 
 * @remarks Les fonction suivantes ne sont pas explicitement testé mais sont
 * utilisée dans les tests des autres fonctions : 
 * - deleteGenList
 * - genListGet
 * - genListSize
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/structure/genericlist.h"


/* dimensions liste générique test */
#define LIST_SIZE 500


/*
    ===================
    === OUTILS TEST ===
    ===================
*/

/* Variables globales */
StringBuilder* string_builder;
int return_value;

#define printsb(msg) addLineStringBuilder(string_builder, msg)

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
    =====================
    === OUTILS LISTES ===
    =====================
*/

/**
 * @brief Crée une genListe dont chaque valeur est égale à sa position
 * 
 * @param size GenList
 * @return GenList
 */
GenList* createUniqueGenList(unsigned long size) {
    GenList* l = createGenList(size);
    for(unsigned long i = 0; i < size; i++)
        genListAdd(l, (void*)i);
    return l;
}

bool testUniqueGenList(GenList* l, unsigned long size) {
    if(genListSize(l) != size) return echecTest(" X-- erreur taille");
    for(unsigned long i = 0; i < size; i++) {
        if(genListGet(l, i) != (void*)i) return echecTest(" X-- valeur incorrecte");
    }
    return true;
}

bool testGenListEquals(GenList* l1, GenList* l2) {
    if(genListSize(l1) != genListSize(l2)) return echecTest(" X-- taille différente");
    unsigned size = genListSize(l1);
    for(unsigned i = 0; i < size; i++)
        if(genListGet(l1, i) != genListGet(l2, i)) return echecTest(" X-- veleur différente");
    return true;
}

/*
    =============
    === TESTS ===
    =============
*/




bool testGenListCreate() {
    GenList *l;

    /* test genListe taille (0) */
    printsb( "\ntest sur genListe taille (0)...");
    l = createGenList(0);
    if(!l) return echecTest(" X-- Pointeur null");
    if(genListSize(l) != 0) return echecTest(" X-- taille != 0");
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    /* test genListe taille (1) */
    printsb( "\ntest sur genListe taille (1)...");
    l = createGenList(1);
    if(!l) return echecTest(" X-- Pointeur null");
    if(genListSize(l) != 0) return echecTest(" X-- taille != 0");
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    /* test genListe taille (LIST_SIZE) */
    printsb( "\ntest sur genListe taille (1)...");
    l = createGenList(LIST_SIZE);
    if(!l) return echecTest(" X-- Pointeur null");
    if(genListSize(l) != 0) return echecTest(" X-- taille != 0");
    printsb("\n\t-test passé\n");
    deleteGenList(&l);
    return true; 
}

bool testGenListAdd() {
    GenList* l;

    /* test sur genListe 0 */
    printsb( "\ntest sur genListe taille (0)...");
    l = createGenList(0);
    for(unsigned long i = 0; i < LIST_SIZE; i++) {
        genListAdd(l, (void*)i);
        if(!testUniqueGenList(l, i+1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    /* test sur genListe LIST_SIZE / 2 */
    printsb( "\ntest sur genListe taille (LIST_SIZE / 2)...");
    l = createGenList(LIST_SIZE/2);
    for(unsigned long i = 0; i < LIST_SIZE; i++) {
        genListAdd(l, (void*)i);
        if(!testUniqueGenList(l, i+1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    return true;
}

bool testGenListPop() {
    GenList* l;

    /* test sur genListe 0 */
    printsb( "\ntest sur genListe taille (0)...");
    l = createUniqueGenList(LIST_SIZE);
    for(unsigned long i = LIST_SIZE; i > 0; i--) {
        if(genListPop(l) != (void*)(i-1)) return echecTest(" X-- erreur valeur retournée");
        if(!testUniqueGenList(l, i-1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    return true;
}

bool testGenListInsert() {
    GenList* l;

    /* test sur genListe 0 */
    printsb( "\ntest insert position 0...");
    l = createGenList(0);
    for(unsigned long i = 0; i < LIST_SIZE; i++) {
        genListInsert(l, (void*)i, 0);
        for(unsigned long j = 0; j < i+1; j++) {
            if(genListGet(l, i-j) != (void*)j) return echecTest(" X-- valeur incorrecte");
        }
    }
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    /* test sur genListe fin */
    printsb( "\ntest insert position fin...");
    l = createGenList(0);
    for(unsigned long i = 0; i < LIST_SIZE; i++) {
        genListInsert(l, (void*)i, i);
        if(!testUniqueGenList(l, i+1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    return true;
}

bool testGenListRemove() {
    GenList* l;

    /* test sur genListe 0 */
    printsb( "\ntest remove position 0...");
    l = createUniqueGenList(LIST_SIZE);
    for(unsigned long i = 0; i < LIST_SIZE; i++) {
        if(genListRemove(l, 0) != (void*)i) return echecTest(" X-- valeur retournée incorrecte");
        for(unsigned long j = 0; j < LIST_SIZE - i -1; j++) {
            if(genListGet(l, j) != (void*)(j+i+1)) return echecTest(" X-- valeur incorrecte");
        }
    }
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    /* test sur genListe fin */
    printsb( "\ntest remove fin...");
    l = createUniqueGenList(LIST_SIZE);
    for(unsigned long i = LIST_SIZE; i  > 0; i--) {
        if(genListRemove(l, i-1) != (void*)(i-1)) return echecTest(" X-- valeur retournée incorrecte");
        if(!testUniqueGenList(l, i-1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    return true;
}

bool testGenListSet() {
    GenList* l;

    /* test set à 0 */
    printsb( "\ntest set à 0...");
    unsigned long j;
    l = createUniqueGenList(LIST_SIZE);
    for(unsigned long i = 0; i < LIST_SIZE; i++) {
        genListSet(l,0,i);
        for(j = 0; j < i+1; j++) {
            if(genListGet(l, j) != (void*)0) return echecTest(" X-- valeur incorrecte");
        }
        for(; j < LIST_SIZE; j++) {
            if(genListGet(l, j) != (void*)j) return echecTest(" X-- valeur incorrecte");
        }
    }
    printsb("\n\t-test passé\n");

    /* test set à i*/
    printsb( "\ntest set à i...");
    for(unsigned long i = 0; i < LIST_SIZE; i++) {
        genListSet(l,(void*)i,i);
        for(j = 0; j < i+1; j++) {
            if(genListGet(l, j) != (void*)j) return echecTest(" X-- valeur incorrecte");
        }
        for(; j < LIST_SIZE; j++) {
            if(genListGet(l, j) != (void*)0) return echecTest(" X-- valeur incorrecte");
        }
    }
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    return true;
}

bool testGenListEmpty() {
    GenList* l;

    /* test sur genListe 0 */
    printsb( "\ntest genList memory_size = 0...");
    l = createGenList(0);
    if(!genListEmpty(l)) return echecTest(" X-- echec genListe vide");
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    /* test sur genListe 0 */
    printsb( "\ntest genList memory_size = LIST_SIZE...");
    l = createGenList(LIST_SIZE);
    if(!genListEmpty(l)) return echecTest(" X-- echec genListe vide");
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    /* test sur genListe 0 */
    printsb( "\ntest genList  size = LIST_SIZE...");
    l = createUniqueGenList(LIST_SIZE);
    if(genListEmpty(l)) return echecTest(" X-- echec genListe vide");
    printsb("\n\t-test passé\n");
    deleteGenList(&l);

    return true;
}

bool testGenListCopy() {
    GenList* l;
    GenList* cpl;

    /* test sur genListe 0 */
    printsb( "\ntest genList taille = 0");
    l = createGenList(0);
    cpl = genListCopy(l);
    if(!cpl) return echecTest(" X-- pointeur null");
    if(!testGenListEquals(l, cpl)) return false;
    printsb("\n\t-test passé\n");
    deleteGenList(&l);
    deleteGenList(&cpl);

    /* test sur genListe LIST_SIZE */
    printsb( "\ntest genList taille = LIST_SIZE");
    l = createUniqueGenList(LIST_SIZE);
    cpl = genListCopy(l);
    if(!cpl) return echecTest(" X-- pointeur null");
    if(!testGenListEquals(l, cpl)) return false;
    printsb("\n\t-test passé\n");
    deleteGenList(&l);
    deleteGenList(&cpl);


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

    test_fun(testGenListCreate, 1, "testCreate");
    test_fun(testGenListAdd, 2, "testGenListAdd");
    test_fun(testGenListPop, 4, "testGenListPop");
    test_fun(testGenListInsert, 8, "testGenListInsert");
    test_fun(testGenListRemove, 16, "testGenListRemove");
    test_fun(testGenListSet, 32, "testGenListSet");
    test_fun(testGenListEmpty, 64, "testGenListEmpty");
    test_fun(testGenListCopy, 64, "testGenListCopy");


    afterAll();

    return return_value;

}
