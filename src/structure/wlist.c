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
#include "list.h"
#include "../logger.h"


/*------------------------------------------------------------------*/
/*                        STRUCTURE LIST                            */
/*------------------------------------------------------------------*/

/* Définition de la structure list*/
struct s_list {
    unsigned int memory_size; /* Taille du tableau en mémoire */
    unsigned int size; /* taille de la liste (nombre éléments) */
    int *tab; /* tableau des valeurs */
};



ptrList createList(unsigned int size) {
    ptrList l = malloc(sizeof(List));
    if (l == NULL) exitl("sdlist.c", "createList", "erreur malloc", EXIT_FAILURE);
    l->memory_size = size;
    l->size = 0;
    l->tab = malloc(sizeof(int)*size);
    return l;
}

void deleteList(ptrList l) {
    free(l->tab);
    free(l);
}

ptrList listAdd(ptrList l, int v) {

    /* agrandissement de la liste si pleine */
    if(l->size == l->memory_size){ 
        /* création nouveau tableau */
        int *new_tab = malloc(sizeof(int)*(l->memory_size + 32));

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

ptrList listInsert(ptrList l, int v, int i) {
    /* vérification paramêtre */
    if (i < 0 || i > l->size) {
        warnl("sdlist.c", "listInsert", "position (i) invalide");
        return NULL;
    }
    /* agrandissement de la liste si pleine */
    if(l->size >= l->memory_size){ 
        /* création nouveau tableau */
        int *new_tab = malloc(sizeof(int)*(l->memory_size + 32));

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

int listGet(ptrList l, int i) {
    /* vérification paramêtre */
    if (i < 0 || i >= l->size) {
        warnl("sdlist.c", "listGet", "position (i) invalide");
        return -1;
    }
    return l->tab[i];
}




/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

void printListLog(ptrList l) {
    char buff[1024];
    
    printl("\n<+>------------[ sdlist ]-----------<+>\n\n");
    sprintf(buff, "[sdlist] size = %d\n", l->size); printl(buff);
    sprintf(buff, "[sdlist] memory size = %d\n", l->memory_size); printl(buff);
    if(l->size == 0) printl("[sdlist] list = ");
    else {
        sprintf(buff, "[sdlist] list = %f", l->tab[0]);printl(buff);
        for (int i = 1; i < l->size; i++){
            sprintf(buff, " , %f", l->tab[i]);
            printl(buff);
        }
    }
    sprintf(buff, "\n\n<->---------------------------------<->\n");
}

#endif

