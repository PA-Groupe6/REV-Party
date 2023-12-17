/**
 * @file test_list.c
 * @author Ugo VALLAT
 * @brief Test sur la structure de données list
 * 
 * @remarks Les fonction suivantes ne sont pas explicitement testé mais sont
 * utilisée dans les tests des autres fonctions : 
 * - deleteList
 * - listGet
 * - listSize
 */


/**
 * @file test_list.c
 * @author Ugo VALLAT
 * @brief Test sur la structure de données list
 * 
 * @remarks Les fonction suivantes ne sont pas explicitement testé mais sont
 * utilisée dans les tests des autres fonctions : 
 * - deleteList
 * - listGet
 * - listSize
 */


#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include "../../src/logger.h"
#include "../test_utils.h"
#include "../../src/structure/list.h"


/* dimensions list test */
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
 * @brief Crée une liste dont chaque valeur est égale à sa position
 * 
 * @param size List
 * @return List
 */
List* createUniqueList(unsigned size) {
    List* l = createList(size);
    for(unsigned i = 0; i < size; i++)
        listAdd(l, i);
    return l;
}

bool testUniqueList(List* l, unsigned size) {
    if(listSize(l) != size) return echecTest(" X-- erreur taille");
    for(unsigned i = 0; i < size; i++) {
        if(listGet(l, i) != (int)i) return echecTest(" X-- valeur incorrecte");
    }
    return true;
}

bool testListEquals(List* l1, List* l2) {
    if(listSize(l1) != listSize(l2)) return echecTest(" X-- taille différente");
    unsigned size = listSize(l1);
    for(unsigned i = 0; i < size; i++)
        if(listGet(l1, i) != listGet(l2, i)) return echecTest(" X-- veleur différente");
    return true;
}

/*
    =============
    === TESTS ===
    =============
*/




bool testListCreate() {
    List *l;

    /* test liste taille (0) */
    printsb( "\ntest sur liste taille (0)...");
    l = createList(0);
    if(!l) return echecTest(" X-- Pointeur null");
    if(listSize(l) != 0) return echecTest(" X-- taille != 0");
    printsb("\n\t-test passé\n");
    deleteList(&l);

    /* test liste taille (1) */
    printsb( "\ntest sur liste taille (1)...");
    l = createList(1);
    if(!l) return echecTest(" X-- Pointeur null");
    if(listSize(l) != 0) return echecTest(" X-- taille != 0");
    printsb("\n\t-test passé\n");
    deleteList(&l);

    /* test liste taille (LIST_SIZE) */
    printsb( "\ntest sur liste taille (1)...");
    l = createList(LIST_SIZE);
    if(!l) return echecTest(" X-- Pointeur null");
    if(listSize(l) != 0) return echecTest(" X-- taille != 0");
    printsb("\n\t-test passé\n");
    deleteList(&l);
    return true; 
}

bool testListAdd() {
    List* l;

    /* test sur liste 0 */
    printsb( "\ntest sur liste taille (0)...");
    l = createList(0);
    for(unsigned i = 0; i < LIST_SIZE; i++) {
        listAdd(l, i);
        if(!testUniqueList(l, i+1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteList(&l);

    /* test sur liste LIST_SIZE / 2 */
    printsb( "\ntest sur liste taille (LIST_SIZE / 2)...");
    l = createList(LIST_SIZE/2);
    for(unsigned i = 0; i < LIST_SIZE; i++) {
        listAdd(l, i);
        if(!testUniqueList(l, i+1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteList(&l);

    return true;
}

bool testListPop() {
    List* l;

    /* test sur liste 0 */
    printsb( "\ntest sur liste taille (0)...");
    l = createUniqueList(LIST_SIZE);
    for(unsigned i = LIST_SIZE; i > 0; i--) {
        if(listPop(l) != (int)i-1) return echecTest(" X-- erreur valeur retournée");
        if(!testUniqueList(l, i-1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteList(&l);

    return true;
}

bool testListInsert() {
    List* l;

    /* test sur liste 0 */
    printsb( "\ntest insert position 0...");
    l = createList(0);
    for(unsigned i = 0; i < LIST_SIZE; i++) {
        listInsert(l, i, 0);
        for(unsigned j = 0; j < i+1; j++) {
            if(listGet(l, i-j) != (int)j) return echecTest(" X-- valeur incorrecte");
        }
    }
    printsb("\n\t-test passé\n");
    deleteList(&l);

    /* test sur liste fin */
    printsb( "\ntest insert position fin...");
    l = createList(0);
    for(unsigned i = 0; i < LIST_SIZE; i++) {
        listInsert(l, i, i);
        if(!testUniqueList(l, i+1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteList(&l);

    return true;
}

bool testListRemove() {
    List* l;

    /* test sur liste 0 */
    printsb( "\ntest remove position 0...");
    l = createUniqueList(LIST_SIZE);
    for(unsigned i = 0; i < LIST_SIZE; i++) {
        if(listRemove(l, 0) != (int)i) return echecTest(" X-- valeur retournée incorrecte");
        for(unsigned j = 0; j < LIST_SIZE - i -1; j++) {
            if(listGet(l, j) != (int)(j+i+1)) return echecTest(" X-- valeur incorrecte");
        }
    }
    printsb("\n\t-test passé\n");
    deleteList(&l);

    /* test sur liste fin */
    printsb( "\ntest remove fin...");
    l = createUniqueList(LIST_SIZE);
    for(unsigned i = LIST_SIZE; i  > 0; i--) {
        if(listRemove(l, i-1) != (int)i-1) return echecTest(" X-- valeur retournée incorrecte");
        if(!testUniqueList(l, i-1)) return false;
    }
    printsb("\n\t-test passé\n");
    deleteList(&l);

    return true;
}

bool testListSet() {
    List* l;

    /* test set à 0 */
    printsb( "\ntest set à 0...");
    unsigned j;
    l = createUniqueList(LIST_SIZE);
    for(unsigned i = 0; i < LIST_SIZE; i++) {
        listSet(l,0,i);
        for(j = 0; j < i+1; j++) {
            if(listGet(l, j) != 0) return echecTest(" X-- valeur incorrecte");
        }
        for(; j < LIST_SIZE; j++) {
            if(listGet(l, j) != (int)j) return echecTest(" X-- valeur incorrecte");
        }
    }
    printsb("\n\t-test passé\n");

    /* test set à i*/
    printsb( "\ntest set à i...");
    for(unsigned i = 0; i < LIST_SIZE; i++) {
        listSet(l,i,i);
        for(j = 0; j < i+1; j++) {
            if(listGet(l, j) != (int)j) return echecTest(" X-- valeur incorrecte");
        }
        for(; j < LIST_SIZE; j++) {
            if(listGet(l, j) != (int)0) return echecTest(" X-- valeur incorrecte");
        }
    }
    printsb("\n\t-test passé\n");
    deleteList(&l);

    return true;
}

bool testListEmpty() {
    List* l;

    /* test sur liste 0 */
    printsb( "\ntest list memory_size = 0...");
    l = createList(0);
    if(!listEmpty(l)) return echecTest(" X-- echec liste vide");
    printsb("\n\t-test passé\n");
    deleteList(&l);

    /* test sur liste 0 */
    printsb( "\ntest list memory_size = LIST_SIZE...");
    l = createList(LIST_SIZE);
    if(!listEmpty(l)) return echecTest(" X-- echec liste vide");
    printsb("\n\t-test passé\n");
    deleteList(&l);

    /* test sur liste 0 */
    printsb( "\ntest list  size = LIST_SIZE...");
    l = createUniqueList(LIST_SIZE);
    if(listEmpty(l)) return echecTest(" X-- echec liste vide");
    printsb("\n\t-test passé\n");
    deleteList(&l);

    return true;
}

bool testListCopy() {
    List* l;
    List* cpl;

    /* test sur liste 0 */
    printsb( "\ntest list taille = 0");
    l = createList(0);
    cpl = listCopy(l);
    if(!cpl) return echecTest(" X-- pointeur null");
    if(!testListEquals(l, cpl)) return false;
    printsb("\n\t-test passé\n");
    deleteList(&l);
    deleteList(&cpl);

    /* test sur liste LIST_SIZE */
    printsb( "\ntest list taille = LIST_SIZE");
    l = createUniqueList(LIST_SIZE);
    cpl = listCopy(l);
    if(!cpl) return echecTest(" X-- pointeur null");
    if(!testListEquals(l, cpl)) return false;
    printsb("\n\t-test passé\n");
    deleteList(&l);
    deleteList(&cpl);


    return true;
}

bool testListIte() {
    List* l;
    ListIte* ite;



    /* test sur une liste vide */
    l = createList(0);

    printsb( "\ntest List ite taille=0 FROM_BEGIN");
    ite = createListIte(l, FROM_BEGIN);
    if(!ite) return echecTest(" X-- pointeur null");
    if(listIteHasNext(ite)) return echecTest(" X-- hasNext");
    deleteListIte(&ite);
    if(ite) return echecTest(" X-- delete iterator != null");
    printsb("\n\t-test passé\n");

    printsb( "\ntest List ite taille=0 FROM_END");
    ite = createListIte(l, FROM_END);
    if(!ite) return echecTest(" X-- pointeur null");
    if(listIteHasNext(ite)) return echecTest(" X-- hasNext");
    deleteListIte(&ite);
    if(ite) return echecTest(" X-- delete iterator != null");
    printsb("\n\t-test passé\n");

    deleteList(&l);

    /* test sur une liste taille LIST_SIZE */
    l = createUniqueList(LIST_SIZE);

    printsb( "\ntest List ite taille=LIST_SIZE FROM_BEGIN");
    ite = createListIte(l, FROM_BEGIN);
    if(!ite) return echecTest(" X-- pointeur null");
    for(unsigned i = 0; i < LIST_SIZE; i++) {
        if(!listIteHasNext(ite)) return echecTest(" X-- !hasNext avant fin");
        listIteNext(ite);
        if(listIteGetValue(ite) != (int)i) return echecTest(" X-- GetValue != i");
    }
    if(listIteHasNext(ite)) return echecTest(" X-- hasnext fin");
    deleteListIte(&ite);
    if(ite) return echecTest(" X-- delete iterator != null");
    printsb("\n\t-test passé\n");

    printsb( "\ntest List ite taille=LIST_SIZE FROM_END");
    ite = createListIte(l, FROM_END);
    if(!ite) return echecTest(" X-- pointeur null");
    for(unsigned i = LIST_SIZE-1; i < LIST_SIZE; i--) {
        if(!listIteHasNext(ite)) return echecTest(" X-- !hasNext avant fin");
        listIteNext(ite);
        if(listIteGetValue(ite) != (int)i) return echecTest(" X-- GetValue != i");
    }
    if(listIteHasNext(ite)) return echecTest(" X-- hasnext fin");
    deleteListIte(&ite);
    if(ite) return echecTest(" X-- delete iterator != null");
    printsb("\n\t-test passé\n");

    deleteList(&l);
    


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

    test_fun(testListCreate, 1, "testCreate");
    test_fun(testListAdd, 2, "testListAdd");
    test_fun(testListPop, 4, "testListPop");
    test_fun(testListInsert, 8, "testListInsert");
    test_fun(testListRemove, 16, "testListRemove");
    test_fun(testListSet, 32, "testListSet");
    test_fun(testListEmpty, 64, "testListEmpty");
    test_fun(testListCopy, 128, "testListCopy");
    test_fun(testListIte, 129, "testListIte");


    afterAll();

    return return_value;

}
