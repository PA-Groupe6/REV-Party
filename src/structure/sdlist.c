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
    unsigned int memory_size;
    unsigned int size;
    TYPE *tab;
};



ptrList createList(int size) {
    ptrList l = malloc(sizeof(List));
    if (l == NULL) exitl("sdlist.c", "createList", "erreur malloc", EXIT_FAILURE);
    l->memory_size = size;
    l->size = 0;
    l->tab = malloc(sizeof(TYPE)*size);
    return l;
}

void deleteList(ptrList l) {
    free(l->tab);
    free(l);
}

ptrList listAdd(ptrList l, TYPE v) {

    /* agrandissement de la liste si pleine */
    if(l->size == l->memory_size){ 
        /* création nouveau tableau */
        TYPE *new_tab = malloc(sizeof(TYPE)*(l->memory_size + 32));

        /* copie du tableau */
        for (int i = 0; i < l->size; i++) 
            new_tab[i] = l->tab[i];

        free(l->tab);
        l->tab = new_tab;
        l->memory_size = l->memory_size + 32;
    }

    l->tab[l->size] = v;
    l->size++;
    return l;
}

ptrList listInsert(ptrList l, TYPE v, int i) {
    /* vérification paramêtre */
    if (i < 0 || i > l->size) {
        warnl("sdlist.c", "listInsert", "position (i) invalide");
        return NULL;
    }
    /* agrandissement de la liste si pleine */
    if(l->size >= l->memory_size){ 
        /* création nouveau tableau */
        TYPE *new_tab = malloc(sizeof(TYPE)*(l->memory_size + 32));

        /* copie du tableau */
        for (int i = 0; i < l->size; i++) 
            new_tab[i] = l->tab[i];

        free(l->tab);
        l->tab = new_tab;
        l->memory_size = l->memory_size + 32;
    }
    
    /* décale tous les éléments */
    for (int j = l->size-1; j >= i; j--)
        l->tab[j+1] = l->tab[j];

    /* ajoute le nouvel élément */
    l->tab[i] = v;
    l->size++;

    return l;
}

int listPop(ptrList l) {
    if(l->size <= 0) return -1;
    l->size--;
    return 0;
}



int listRemove(ptrList l, int i) {
    /* vérification paramêtre */
    if (i < 0 || i >= l->size) {
        warnl("sdlist.c", "listRemove", "position (i) invalide");
        return -1;
    }

    for(int j = i; j < l->size-1; j++)
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


ptrList listCopy(ptrList old_list, int new_size) {
    if(new_size < old_list->size) {
        warnl("sdlist.c", "listCopy", "invalid size for new list");
        return NULL;
    }
    ptrList new_list = createList(new_size);

    new_list->size = old_list->size;
    for(int i = 0; i < old_list->size; i++)
        new_list->tab[i] = old_list->tab[i];

    return new_list;
}



TYPE listGet(ptrList l, int i) {
    /* vérification paramêtre */
    if (i < 0 || i >= l->size) {
        warnl("sdlist.c", "listGet", "position (i) invalide");
        return -1;
    }

    return l->tab[i];
}

void displayList(ptrList l) {
    if(l->size == 0) printf("[]");
    else {
        printf("[ %f", l->tab[0]);
        for (int i = 1; i < l->size; i++)
            printf(", %f", l->tab[i]);
        printf("]");
    }
}


#ifdef DEBUG

void printListLog(ptrList l) {
    printf("\n<+>------------[ sdlist ]-----------<+>\n\n");
    printf("[sdlist] size = %d\n", l->size);
    printf("[sdlist] memory size = %d\n", l->memory_size);
    if(l->size == 0) printf("[sdlist] list = ");
    else {
        printf("[sdlist] list = %f", l->tab[0]);
        for (int i = 1; i < l->size; i++)
            printf(" , %f", l->tab[i]);
    }
    printf("\n\n<->---------------------------------<->\n");
}

#endif






