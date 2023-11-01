
/**
 * @file genericlist.c
 * @author VALLAT Ugo
 *
 * @brief Cette librairie implémente une liste générique pseudo statique d'entiers
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
 *
 * La liste ne contient que des pointeur génériques vers la donnée (void*)
 *
 * @note haute performance en lecture( O(1) ) mais faible en écriture ( O(n))
 *
 * @remark En cas d'erreur, toutes les fonctions de list exit le progamme avec un
 * message d'erreur
 */

#include "genericlist.h"
#include "../logger.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>

/*------------------------------------------------------------------*/
/*                     STRUCTURE LIST GENERIC                       */
/*------------------------------------------------------------------*/

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief  Définition de la structure list
 */
struct s_gen_list {
    unsigned int memory_size; /* Taille du tableau en mémoire */
    unsigned int size;        /* taille de la liste (nombre éléments) */
    void **tab;               /* tableau des poiteur (générique) */
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
GenList *createGenList(unsigned int size) {
    GenList *l = malloc(sizeof(GenList));
    if (l == NULL)
        exitl("genericlist.c", "createGenList", EXIT_FAILURE, "erreur malloc list");

    l->tab = malloc(sizeof(void *) * size);
    if (l->tab == NULL && size != 0)
        exitl("genericlist.c", "createGenList", EXIT_FAILURE, "erreur malloc tab");

    l->memory_size = size;
    l->size = 0;
    return l;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void deleteGenList(ptrGenList *l) {
    /* test l != NULL */
    testArgNull(l, "deleteGenList");
    testArgNull((*l), "deleteGenList");

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
void adjustMemorySize(GenList *l, unsigned int new_size) {
    testArgNull(l, "adjustMemorySize");

    /* nouvelle taille de la liste */
    l->memory_size = new_size;

    /* modification taille du tableau */
    l->tab = realloc(l->tab, new_size * sizeof(void *));
    if (new_size != 0 && l->tab == NULL)
        exitl("genericlist.c", "adjustMemorySize", EXIT_FAILURE, "echec realloc tab");
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void genListAdd(GenList *l, void *v) {
    /* test l != NULL */
    testArgNull(l, "genListAdd");

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
void genListInsert(GenList *l, void *v, unsigned int i) {
    /* vérification paramêtres */
    testArgNull(l, "genListInsert");
    if (i > l->size)
        exitl("genericlist.c", "genListInsert", EXIT_FAILURE, "position (%d) invalide", i);

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
void genListPop(GenList *l) {
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
void genListRemove(GenList *l, unsigned int i) {
    /* vérification paramêtres */
    testArgNull(l, "genListRemove");
    if (i >= l->size)
        exitl("genericlist.c", "genListRemove", EXIT_FAILURE, "position (%d) invalide", i);

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
bool genListEmpty(GenList *l) {
    testArgNull(l, "listEmpty");
    return l->size == 0;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
unsigned int genListSize(GenList *l) {
    testArgNull(l, "genListSize");
    return l->size;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
GenList *genListCopy(GenList *l) {
    /* vérification paramêtre */
    testArgNull(l, "listCopy");

    /* création nouvelle liste */
    GenList *new = createGenList(l->size);

    /* copie des éléments */
    for (unsigned int i = 0; i < l->size; i++)
        genListAdd(new, l->tab[i]);

    return new;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void *genListGet(GenList *l, unsigned int i) {
    /* vérification paramêtre */
    testArgNull(l, "genListGet");
    if (i >= l->size)
        exitl("genericlist.c", "genListGet", EXIT_FAILURE, "position (%d) invalide", i);

    return l->tab[i];
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void genListSet(GenList *l, void *v, unsigned int i) {
    /* vérification paramêtre */
    testArgNull(l, "genListSet");
    if (i >= l->size)
        exitl("genericlist.c", "genListSet", EXIT_FAILURE, "position (%d) invalide", i);

    l->tab[i] = v;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void displayGenList(GenList *l) {
    /* vérification paramêtre */
    testArgNull(l, "displayGenList");

    if (l->size == 0)
        printl("[ ]");
    else {
        printl("[ %f,", l->tab[0]);
        for (unsigned int i = 1; i < l->size; i++) {
            printl(", %f", l->tab[i]);
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
 * @brief Définition de la structure list_ite
 */
struct s_gen_list_ite {
    GenList *list;              /* liste à parcourir */
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
GenListIte *createGenListIte(GenList *l, int dir) {
    testArgNull(l, "createGenListIte");

    /* création de l'itérateur */
    GenListIte *ite = malloc(sizeof(GenListIte));

    /* copie de la liste */
    ite->list = genListCopy(l);
    if (errno)
        return NULL;

    /* Initialisation des paramêtres */
    if (dir == FROM_BEGIN) {
        ite->fnext = next_forward;
        ite->next = false;
        ite->cur = -1;
    } else if (dir == FROM_END) {
        ite->fnext = next_backward;
        ite->next = false;
        ite->cur = l->size;
    } else {
        exitl("genericlist.c", "createGenListIte", EXIT_FAILURE, "sens de parcours invalide");
        free(ite);
        return NULL;
    }
    return ite;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
bool genListIteHasNext(GenListIte *ite) {
    testArgNull(ite, "genListIteHasNext");
    unsigned int next = ite->fnext(ite->cur);
    return (next < ite->list->size);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void genListIteNext(GenListIte *ite) {
    testArgNull(ite, "genListIteNext");
    if (!genListIteHasNext(ite))
        exitl("genericlist.c", "genListIteNext", EXIT_FAILURE, "aucun élément à lire");

    ite->cur = ite->fnext(ite->cur);
    ite->next = true;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void *genListIteGetValue(GenListIte *ite) {
    testArgNull(ite, "genListIteGetValue");
    if (!ite->next)
        exitl("genericlist.c", "genListIteGetValue", EXIT_FAILURE, "lecture sans appel à next");

    return genListGet(ite->list, ite->cur);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void deleteGenListIte(ptrGenListIte *ite) {
    testArgNull(ite, "genListIteGetValue");
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
void printGenListLog(GenList *l) {
    testArgNull(l, "printGenListLog");

    printl("\n<+>------------[ genericlist.c ]-----------<+>\n\n");
    printl("[list.c] size = %d\n", l->size);
    printl("[list.c] memory size = %d\n", l->memory_size);
    if (l->size <= 0)
        printl("[list.c] list = [");
    else {
        printl("[list.c] list = [ %f ", l->tab[0]);
        for (unsigned int i = 1; i < l->size; i++) {
            printl(", %f", l->tab[i]);
        }
    }
    printl(" ]\n\n<->---------------------------------<->\n");
}

#endif