
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
#include "data_struct_utils.h"
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
    unsigned memory_size; /* Taille du tableau en mémoire */
    unsigned size;        /* taille de la liste (nombre éléments) */
    void **tab;               /* tableau des poiteur (générique) */
};

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
GenList *createGenList(unsigned memory_size) {
    GenList *l = malloc(sizeof(GenList));
    if (l == NULL)
        exitl("genericlist.c", "createGenList", EXIT_FAILURE, "erreur malloc list");

    l->tab = malloc(sizeof(void *) * memory_size);
    if (l->tab == NULL)
        exitl("genericlist.c", "createGenList", EXIT_FAILURE, "erreur malloc tab");

    l->memory_size = memory_size;
    l->size = 0;
    return l;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void deleteGenList(ptrGenList *l) {
#ifdef DEBUG
    /* test l != NULL */
    testArgNull(l, "genericlist.c", "deleteGenList", "l");
    testArgNull((*l), "genericlist.c", "deleteGenList", "*l");
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
void adjustMemorySizeGenList(GenList *l, unsigned new_size) {
#ifdef DEBUG
    testArgNull(l, "genericlist.c", "adjustMemorySizeGenList", "l");
#endif

    /* nouvelle taille de la liste */
    l->memory_size = new_size;

    /* modification taille du tableau */
    l->tab = realloc(l->tab, new_size * sizeof(void *));
    if (new_size != 0 && l->tab == NULL)
        exitl("genericlist.c", "adjustMemorySizeGenList", EXIT_FAILURE, "echec realloc tab");
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void genListAdd(GenList *l, void *v) {
#ifdef DEBUG
    /* test l != NULL */
    testArgNull(l, "genericlist.c", "genListAdd", "l");
#endif

    /* agrandissement de la liste si pleine */
    if (l->size == l->memory_size)
        adjustMemorySizeGenList(l, l->memory_size + 8);

    /* Ajout de la valeur */
    l->tab[l->size] = v;
    l->size++;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void genListInsert(GenList *l, void *v, unsigned i) {
#ifdef DEBUG
    /* vérification paramêtres */
    testArgNull(l, "genericlist.c", "genListInsert", "l");
    if (i > l->size)
        exitl("genericlist.c", "genListInsert", EXIT_FAILURE, "position (%d) invalide", i);
#endif

    /* agrandissement de la liste si pleine */
    if (l->size >= l->memory_size)
        adjustMemorySizeGenList(l, l->memory_size + 8);

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
void* genListPop(GenList *l) {
#ifdef DEBUG
    /* vérification paramêtre */
    testArgNull(l, "genericlist.c", "listPop", "l");
    if (l->size <= 0)
        exitl("list.c", "listPop", EXIT_FAILURE, "liste déjà vide");
#endif

    /* suppression de l'élément */
    void* elem = l->tab[l->size-1];
    l->size--;
    adjustMemorySizeGenList(l, l->size);
    return elem;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void* genListRemove(GenList *l, unsigned i) {
#ifdef DEBUG
    /* vérification paramêtres */
    testArgNull(l, "genericlist.c", "genListRemove", "l");
    if (i >= l->size)
        exitl("genericlist.c", "genListRemove", EXIT_FAILURE, "position (%d) invalide", i);
#endif

    void* elem = l->tab[i];
    /* suppression de l'élément */
    for (int j = i; j < (int)l->size - 1; j++)
        l->tab[j] = l->tab[j + 1];
    l->size--;
    adjustMemorySizeGenList(l, l->size);
    return elem;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
bool genListEmpty(GenList *l) {
#ifdef DEBUG
    testArgNull(l, "genericlist.c", "listEmpty", "l");
#endif

    return l->size == 0;
}

/**
 * @date 18/12/2023
 * @author LAFORGE Mateo
*/
bool genListContains(GenList *l, void* e) {
#ifdef DEBUG
    testArgNull(l, "genericlist.c", "listContains", "l");
#endif
    for (unsigned int i = 0; i < genListSize(l); i++)
        if (genListGet(l, i) == e)
            return true;
    return false;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
unsigned genListSize(GenList *l) {
#ifdef DEBUG
    testArgNull(l, "genericlist.c", "genListSize", "l");
#endif

    return l->size;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
GenList *genListCopy(GenList *l) {
#ifdef DEBUG
    /* vérification paramêtre */
    testArgNull(l, "genericlist.c", "listCopy", "l");
#endif

    /* création nouvelle liste */
    GenList *new = createGenList(l->size);

    /* copie des éléments */
    for (unsigned i = 0; i < l->size; i++)
        genListAdd(new, l->tab[i]);

    return new;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void *genListGet(GenList *l, unsigned i) {
#ifdef DEBUG
    /* vérification paramêtre */
    testArgNull(l, "genericlist.c", "genListGet", "l");
    if (i >= l->size)
        exitl("genericlist.c", "genListGet", EXIT_FAILURE, "position (%d) invalide", i);
#endif

    return l->tab[i];
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void genListSet(GenList *l, void *v, unsigned i) {
#ifdef DEBUG
    /* vérification paramêtre */
    testArgNull(l, "genericlist.c", "genListSet", "l");
    if (i >= l->size)
        exitl("genericlist.c", "genListSet", EXIT_FAILURE, "position (%d) invalide", i);
#endif

    l->tab[i] = v;
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
    unsigned (*fnext)(int); /* focntion de déplcament */
    bool next;                  /* appel à fonction next */
};

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
unsigned next_forward_gen(int i) { return i + 1; }

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
unsigned next_backward_gen(int i) { return i - 1; }

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
GenListIte *createGenListIte(GenList *l, int dir) {
#ifdef DEBUG
    testArgNull(l, "genericlist.c", "createGenListIte", "l");
#endif

    /* création de l'itérateur */
    GenListIte *ite = malloc(sizeof(GenListIte));

    /* copie de la liste */
    ite->list = genListCopy(l);
    if (errno)
        return NULL;

    /* Initialisation des paramêtres */
    if (dir == FROM_BEGIN) {
        ite->fnext = next_forward_gen;
        ite->next = false;
        ite->cur = -1;
    } else if (dir == FROM_END) {
        ite->fnext = next_backward_gen;
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
#ifdef DEBUG
    testArgNull(ite, "genericlist.c", "genListIteHasNext", "ite");
#endif

    unsigned next = ite->fnext(ite->cur);
    return (next < ite->list->size);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void genListIteNext(GenListIte *ite) {
#ifdef DEBUG
    testArgNull(ite, "genericlist.c", "genListIteNext", "ite");
    if (!genListIteHasNext(ite))
        exitl("genericlist.c", "genListIteNext", EXIT_FAILURE, "aucun élément à lire");
#endif

    ite->cur = ite->fnext(ite->cur);
    ite->next = true;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void *genListIteGetValue(GenListIte *ite) {
#ifdef DEBUG
    testArgNull(ite, "genericlist.c", "genListIteGetValue", "ite");
    if (!ite->next)
        exitl("genericlist.c", "genListIteGetValue", EXIT_FAILURE, "lecture sans appel à next");
#endif

    return genListGet(ite->list, ite->cur);
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void deleteGenListIte(ptrGenListIte *ite) {
#ifdef DEBUG
    testArgNull(ite, "genericlist.c", "genListIteGetValue", "ite");
#endif

    deleteGenList(&((*ite)->list));
    free((*ite));
    *ite = NULL;
}
