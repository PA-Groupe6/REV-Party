
/**
 * @file list.c
 * @author VALLAT Ugo
 *
 * @brief Implémentation de la liste pseudo statique d'entiers
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
 *
 * @remark En cas d'erreur, toutes les fonctions de list exit le progamme avec un
 * message d'erreur
 */

#include "list.h"
#include "../logger.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>

/*------------------------------------------------------------------*/
/*                        STRUCTURE LIST                            */
/*------------------------------------------------------------------*/

/* Définition de la structure list*/
struct s_list {
    unsigned int memory_size; /* Taille du tableau en mémoire */
    unsigned int size;        /* taille de la liste (nombre éléments) */
    int *tab;                 /* tableau des valeurs */
};

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Exit le programme avec message si arg == NULL
 * @param[in] arg argument à vérifier
 * @param[in] caller Nom de la fonction appelante
 */
void testArgNull(void *arg, char *caller) {
    if (arg == NULL)
        exitl("list.c", "hisArgNull", EXIT_FAILURE, "dans %s > erreur pointeur null", caller);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
List *createList(unsigned int size) {
    List *l = malloc(sizeof(List));
    if (l == NULL)
        exitl("list.c", "createList", EXIT_FAILURE, "erreur malloc list");

    l->tab = malloc(sizeof(int) * size);
    if (l->tab == NULL && size != 0)
        exitl("list.c", "createList", EXIT_FAILURE, "erreur malloc tab");

    l->memory_size = size;
    l->size = 0;
    return l;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void deleteList(ptrList *l) {
    /* test l != NULL */
    testArgNull(l, "deleteList");
    testArgNull((*l), "deleteList");

    /* libération de la mémoire */
    free((*l)->tab);
    free((*l));
    *l = NULL;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Modifie l'espace mémoire aloué au tableau
 *
 * @param l Pointeur vers la liste
 * @param new_size Nouvelle taille du tableau
 * @pre l != NULL
 */
void adjustMemorySize(List *l, unsigned int new_size) {
    testArgNull(l, "adjustMemorySize");

    /* nouvelle taille de la liste */
    l->memory_size = new_size;

    /* modification taille du tableau */
    l->tab = realloc(l->tab, new_size * sizeof(int));
    if (new_size != 0 && l->tab == NULL)
        exitl("list.c", "adjustMemorySize", EXIT_FAILURE, "echec realloc tab");
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listAdd(List *l, int v) {
    /* test l != NULL */
    testArgNull(l, "listAdd");

    /* agrandissement de la liste si pleine */
    if (l->size == l->memory_size)
        adjustMemorySize(l, l->memory_size + 8);

    /* Ajout de la valeur */
    l->tab[l->size] = v;
    l->size++;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listInsert(List *l, int v, unsigned int i) {
    /* vérification paramêtres */
    testArgNull(l, "listInsert");
    if (i > l->size)
        exitl("list.c", "listInsert", EXIT_FAILURE, "position (i) invalide");

    /* agrandissement de la liste si pleine */
    if (l->size >= l->memory_size)
        adjustMemorySize(l, l->memory_size + 8);

    /* décale tous les éléments */
    for (int j = l->size - 1; j >= (int)i; j--)
        l->tab[j + 1] = l->tab[j];

    /* ajoute le nouvel élément */
    l->tab[i] = v;
    l->size++;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listPop(List *l) {
    /* vérification paramêtre */
    testArgNull(l, "listPop");
    if (l->size <= 0)
        exitl("list.c", "listPop", EXIT_FAILURE, "liste déjà vide");

    /* suppression de l'élément */
    l->size--;
    adjustMemorySize(l, l->size);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listRemove(List *l, unsigned int i) {
    /* vérification paramêtres */
    testArgNull(l, "listRemove");
    if (i >= l->size)
        exitl("list.c.c", "listRemove", EXIT_FAILURE, "position (i) invalide");

    /* suppression de l'élément */
    for (int j = i; j < (int)l->size - 1; j++)
        l->tab[j] = l->tab[j + 1];
    l->size--;
    adjustMemorySize(l, l->size);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
bool listEmpty(List *l) {
    testArgNull(l, "listEmpty");
    return l->size == 0;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
unsigned int listSize(List *l) {
    testArgNull(l, "lestSize");
    return l->size;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
List *listCopy(List *l) {
    /* vérification paramêtre */
    testArgNull(l, "listCopy");

    /* création nouvelle liste */
    List *new = createList(l->size);

    /* copie des éléments */
    for (unsigned int i = 0; i < l->size; i++) {
        listAdd(new, l->tab[i]);
    }
    return new;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
int listGet(List *l, unsigned int i) {
    /* vérification paramêtre */
    testArgNull(l, "listGet");
    if (i >= l->size)
        exitl("list.c", "listGet", EXIT_FAILURE, "position (%d) invalide", i);

    return l->tab[i];
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listSet(List *l, int v, unsigned int i) {
    /* vérification paramêtre */
    testArgNull(l, "listSet");
    if (i >= l->size)
        exitl("list.c", "listSet", EXIT_FAILURE, "position (%d) invalide", i);

    l->tab[i] = v;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void displayList(List *l) {
    /* vérification paramêtre */
    testArgNull(l, "displayList");

    if (l->size == 0)
        printl("[ ]");
    else {
        printl("[ %d,", l->tab[0]);
        for (unsigned int i = 1; i < l->size; i++) {
            printl(", %d", l->tab[i]);
        }
        printl(" ]");
    }
}

/*------------------------------------------------------------------*/
/*                         ITERATEUR                                */
/*------------------------------------------------------------------*/

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 *
 * @brief Définition de la structure list_ite
 */
struct s_list_ite {
    List *list;                 /* liste à parcourir */
    int cur;                    /* position actuelle */
    unsigned int (*fnext)(int); /* focntion de déplcament */
    bool next;                  /* appel à fonction next */
};

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
unsigned int next_forward(int i) { return i + 1; }

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
unsigned int next_backward(int i) { return i - 1; }

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
ListIte *createListIte(List *l, int dir) {
    testArgNull(l, "createListIte");

    /* création de l'itérateur */
    ListIte *ite = malloc(sizeof(ListIte));

    /* copie de la liste */
    ite->list = listCopy(l);

    /* Initialisation des paramêtres */
    if (dir == FROM_BEGIN) {
        ite->fnext = next_forward;
        ite->next = false;
        ite->cur = -1;
    } else if (dir == FROM_END) {
        ite->fnext = next_backward;
        ite->next = false;
        ite->cur = l->size;
    } else
        exitl("list.c", "createListIte", EXIT_FAILURE, "sens de parcours invalide");
    return ite;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
bool listIteHasNext(ListIte *ite) {
    testArgNull(ite, "listIteHasNext");
    unsigned int next = ite->fnext(ite->cur);
    return (next < ite->list->size);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listIteNext(ListIte *ite) {
    testArgNull(ite, "listIteNext");
    if (!listIteHasNext(ite))
        exitl("list.c", "listIteNext", EXIT_FAILURE, "aucun élément à lire");

    ite->cur = ite->fnext(ite->cur);
    ite->next = true;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
int listIteGetValue(ListIte *ite) {
    testArgNull(ite, "listIteGetValue");
    if (!ite->next)
        exitl("list.c", "listIteGetValue", EXIT_FAILURE, "lecture sans appel à next");

    return listGet(ite->list, ite->cur);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void deleteListIte(ptrListIte *ite) {
    testArgNull(ite, "deleteListIte");
    free((*ite)->list);
    free((*ite));
    *ite = NULL;
}

/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void printListLog(List *l) {
    testArgNull(l, "printListLog");

    printl("\n<+>------------[ list.c ]-----------<+>\n\n");
    printl("[list.c] size = %d\n", l->size);
    printl("[list.c] memory size = %d\n", l->memory_size);
    if (l->size <= 0)
        printl("[list.c] list = [");
    else {
        printl("[list.c] list = [ %d ", l->tab[0]);
        for (unsigned int i = 1; i < l->size; i++) {
            printl(", %d", l->tab[i]);
        }
    }
    printl(" ]\n\n<->---------------------------------<->\n");
}

#endif