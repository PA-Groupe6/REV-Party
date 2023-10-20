/*-----------------------------------------------------------------*/
/**
    Projet REV party - Structur list implementation
    \file sdlist.h
    \author : Ugo VALLAT
    \date creation date : 20/10/2023
 
 */
/*-----------------------------------------------------------------*/


#include <malloc.h>
#include <stdlib.h>
#include "sdlist.h"
#include "../../logger.h"


struct s_list {
    unsigned int memorySize;
    unsigned int size;
    TYPE *tab;
};

ptrList createList(int size) {
    ptrList l = malloc(sizeof(List));
    if (l == NULL) exitl("sdlist.c", "createList", "erreur malloc", EXIT_FAILURE);
    l->memorySize = size;
    l->size = 0;
    l->tab = malloc(sizeof(TYPE)*size);
    return l;
}

void deleteList(ptrList l) {
    free(l->tab);
    free(l);
}

ptrList listAdd(ptrList *ptrl, TYPE v) {
    ptrList l = *ptrl;

    if(l->size == l->memorySize) /* if full : copy the list into a bigger one */
        if((l = listCopy(l, listSize(l)+64)) == NULL)
            exitl("sdlist.c", "listAdd", "impossible d'ajouter un element", EXIT_FAILURE);
    
    l->tab[l->size] = v;
    l->size++;
    *ptrl = l;
    return l;
}

ptrList listInsert(ptrList* ptrl, TYPE v, int i) {
    ptrList l = *ptrl;
    if (i < 0 || i > l->size) {
        warnl("sdlist.c", "listInsert", "position (i) invalide");
        return NULL;
    }
    if(l->size == l->memorySize) /* if full : copy the list into a bigger one */
        if((l = listCopy(l, listSize(l)+64)) == NULL)
            exitl("sdlist.c", "listInsert", "impossible d'ajouter un element", EXIT_FAILURE);

    
    

}










