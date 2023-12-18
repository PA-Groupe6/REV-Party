
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
#include "data_struct_utils.h"
#include <asm-generic/errno-base.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>

/*------------------------------------------------------------------*/
/*                        STRUCTURE LIST                            */
/*------------------------------------------------------------------*/

/* Définition de la structure list*/
struct s_list {
    unsigned memory_size; /* Taille du tableau en mémoire */
    unsigned size;        /* taille de la liste (nombre éléments) */
    int *tab;                 /* tableau des valeurs */
};

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
List *createList(unsigned memory_size) {
    List *l = malloc(sizeof(List));
    if (l == NULL)
        exitl("list.c", "createList", EXIT_FAILURE, "erreur malloc list");
    l->tab = malloc(sizeof(int) * memory_size);
    if (l->tab == NULL && memory_size != 0)
        exitl("list.c", "createList", EXIT_FAILURE, "erreur malloc tab");

    l->memory_size = memory_size;
    l->size = 0;
    return l;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void deleteList(ptrList *l) {
#ifdef DEBUG
    /* test l != NULL */
    testArgNull(l, "list.c", "deleteList", "l");
    testArgNull((*l), "list.c", "deleteList", "*l");
#endif

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
void adjustMemorySizeList(List *l, unsigned new_size) {
#ifdef DEBUG
    testArgNull(l, "list.c", "adjustMemorySizeList", "l");
#endif

    /* nouvelle taille de la liste */
    l->memory_size = new_size;

    /* modification taille du tableau */
    l->tab = realloc(l->tab, new_size * sizeof(int));
    if (new_size != 0 && l->tab == NULL)
        exitl("list.c", "adjustMemorySizeList", EXIT_FAILURE, "echec realloc tab");
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listAdd(List *l, int v) {
#ifdef DEBUG
    /* test l != NULL */
    testArgNull(l, "list.c", "listAdd", "l");
#endif

    /* agrandissement de la liste si pleine */
    if (l->size == l->memory_size)
        adjustMemorySizeList(l, l->memory_size + 8);

    /* Ajout de la valeur */
    l->tab[l->size] = v;
    l->size++;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listInsert(List *l, int v, unsigned i) {
#ifdef DEBUG
    /* vérification paramêtres */
    testArgNull(l, "list.c", "listInsert", "l");
    if (i > l->size)
        exitl("list.c", "listInsert", EXIT_FAILURE, "position (i) invalide");
#endif

    /* agrandissement de la liste si pleine */
    if (l->size >= l->memory_size)
        adjustMemorySizeList(l, l->memory_size + 8);

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
int listPop(List *l) {
#ifdef DEBUG
    /* vérification paramêtre */
    testArgNull(l, "list.c", "listPop", "l");
    if (l->size <= 0)
        exitl("list.c", "listPop", EXIT_FAILURE, "liste déjà vide");
#endif

    /* suppression de l'élément */
    int elem = l->tab[l->size-1];
    l->size--;
    adjustMemorySizeList(l, l->size);
    return elem;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
int listRemove(List *l, unsigned i) {
#ifdef DEBUG
    /* vérification paramêtres */
    testArgNull(l, "list.c", "listRemove", "l");
    if (i >= l->size)
        exitl("list.c.c", "listRemove", EXIT_FAILURE, "position (i) invalide");
#endif

    int elem = l->tab[i];

    /* suppression de l'élément */
    for (int j = i; j < (int)l->size - 1; j++)
        l->tab[j] = l->tab[j + 1];
    l->size--;
    adjustMemorySizeList(l, l->size);

    return elem;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
bool listEmpty(List *l) {
#ifdef DEBUG
    testArgNull(l, "list.c", "listEmpty", "l");
#endif

    return l->size == 0;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
unsigned listSize(List *l) {
#ifdef DEBUG
    testArgNull(l, "list.c", "lestSize", "l");
#endif

    return l->size;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
List *listCopy(List *l) {
#ifdef DEBUG
    /* vérification paramêtre */
    testArgNull(l, "list.c", "listCopy", "l");
#endif

    /* création nouvelle liste */
    List *new = createList(l->size);

    /* copie des éléments */
    for (unsigned i = 0; i < l->size; i++) {
        listAdd(new, l->tab[i]);
    }
    return new;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
int listGet(List *l, unsigned i) {
#ifdef DEBUG
    /* vérification paramêtre */
    testArgNull(l, "list.c", "listGet", "l");
    if (i >= l->size)
        exitl("list.c", "listGet", EXIT_FAILURE, "position (%d) invalide", i);
#endif

    return l->tab[i];
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void listSet(List *l, int v, unsigned i) {
#ifdef DEBUG
    /* vérification paramêtre */
    testArgNull(l, "list.c", "listSet", "l");
    if (i >= l->size)
        exitl("list.c", "listSet", EXIT_FAILURE, "position (%d) invalide", i);
#endif

    l->tab[i] = v;
}


void listClear(List *l) {
    testArgNull(l, "list.c", "listClear", "l");
    l->size = 0;
}


