/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structur list header
    \file list.h
    \author : Ugo VALLAT
    \date Date de création : 20/10/2023
 
 */
/*-----------------------------------------------------------------*/

/**
 * @brief Cette librairie implémente une liste pseudo statique d'entiers
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
 *
 * @note haute performance en lecture( O(1) ) mais faible en écriture ( O(n))
 *
 */


#ifndef __LIST__H__
#define __LIST__H__
#include <stdbool.h>
#include <stdlib.h>

/*------------------------------------------------------------------*/
/*                        STRUCTURE LIST                            */
/*------------------------------------------------------------------*/

typedef struct s_list List;
typedef List * ptrList;

ptrList createList(unsigned int size) {
    return NULL;
}

void deleteList(ptrList l) {

}

ptrList listAdd(ptrList l, int v) {
    return NULL;
}

ptrList listInsert(ptrList l, int v, int i) {
    return NULL;
}

int listPop(ptrList l) {
    return -1;
}

int listRemove(ptrList l, int i) {
    return -1;
}

int listGet(ptrList l, int i) {
    return 0;
}

bool listEmpty(ptrList l) {
    return false;
}

unsigned int listSize(ptrList l) {
    return 0;
}




/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @brief display in the logger all the information about the list and the elements
 * 
 * @param[in] l list to print in logger
 * @date 20/10/2023
 */
void printListLog(ptrList l){

}

#endif

#endif
