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
#include "../logger.h"


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
    /* vérification paramêtre */
    if (i < 0 || i > l->size) {
        warnl("sdlist.c", "listInsert", "position (i) invalide");
        return NULL;
    }
    /* agrandissement de la liste si pleine */
    if(l->size == l->memorySize){ /* if full : copy the list into a bigger one */
        if((l = listCopy(l, listSize(l)+64)) == NULL)
            exitl("sdlist.c", "listInsert", "impossible d'ajouter un element", EXIT_FAILURE);
        deleteList(*ptrl);
    }
    
    /* décale tous les éléments */
    for (int j = l->size; j >= i; j--)
        l->tab[j+1] = l->tab[j];

    /* ajoute le nouvel élément */
    l->tab[i] = v;
    l->size++;
    *ptrl = l;

    return l;
}

int listRemove(ptrList l, int i) {
    /* vérification paramêtre */
    if (i < 0 || i > l->size) {
        warnl("sdlist.c", "listRemove", "position (i) invalide");
        return -1;
    }

    for(int j = i; j < l->size; j++)
        l->tab[j] = l->tab[j+1];
    l->size--;
    return 0;
}


bool listEmpty(ptrList l) {
    return l->size == 0;
}

unsigned int listSize(ptrList l) {
    return l->size;
}


ptrList listCopy(ptrList oldList, int size) {
    if(size < oldList->size) {
        warnl("sdlist.c", "listCopy", "invalid size for new list");
        return NULL;
    }
    ptrList newList = createList(size);

    newList->size = oldList->size;
    for(int i = 0; i < oldList->size; i++)
        newList->tab[i] = oldList->tab[i];

    return newList;
}











