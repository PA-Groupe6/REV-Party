
/**
 * @file genericlist.c
 * @author VALLAT Ugo
 * @date 30/10/2023
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
 * @remark En cas d'erreur, la variable errno est positionnée à la valeur appropriée, il est
 * fortement recomandé de la vérifier surtout pour les fonctions ne renvoyant pas de pointeur
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

/* Définition de la structure list*/
struct s_gen_list {
    unsigned int memory_size; /* Taille du tableau en mémoire */
    unsigned int size;        /* taille de la liste (nombre éléments) */
    void **tab;               /* tableau des poiteur (générique) */
};

/**
 * @brief Verifie si l'argument est NULL, si vrai position errno à EFAULT
 * et affiche un warning
 * @param[in] arg argument à vérifier
 * @param[in] caller Nom de la fonction appelante
 * @return true si arg = NULL, false sinon
 *
 */
bool testArgNull(void *arg, char *caller) {
    if (arg == NULL) {
        warnl("genericlist.c", "hisArgNull", "dans %s > erreur pointeur null", caller);
        errno = EFAULT;
        return true;
    }
    return false;
}

GenList *createGenList(unsigned int size) {
    GenList *l = malloc(sizeof(GenList));
    if (l == NULL)
        exitl("genericlist.c", "createGenList", EXIT_FAILURE, "erreur malloc list");

    l->tab = malloc(sizeof(void *) * size);
    if (l->tab == NULL)
        exitl("genericlist.c", "createGenList", EXIT_FAILURE, "erreur malloc tab");

    l->memory_size = size;
    l->size = 0;
    return l;
}

void deleteGenList(ptrGenList *l) {
    /* test l != NULL */
    if (testArgNull(l, "deleteGenList") || testArgNull((*l), "deleteGenList"))
        return;

    /* libération de la mémoire */
    free((*l)->tab);
    free((*l));
    *l = NULL;
}

/**
 * @brief Modifie l'espace mémoire aloué au tableau
 *
 * @param l Pointeur vers la liste
 * @param new_size Nouvelle taille du tableau
 * @pre l != NULL
 */
void adjustMemorySize(GenList *l, unsigned int new_size) {
    /* nouvelle taille de la liste */
    l->memory_size = new_size;

    /* modification taille du tableau */
    l->tab = realloc(l->tab, new_size * sizeof(void *));
    if (new_size != 0 && l->tab == NULL)
        exitl("genericlist.c", "adjustMemorySize", EXIT_FAILURE, "echec realloc tab");
}

GenList *genListAdd(GenList *l, void *v) {
    /* test l != NULL */
    if (testArgNull(l, "genListAdd"))
        return NULL;

    /* agrandissement de la liste si pleine */
    if (l->size == l->memory_size)
        adjustMemorySize(l, l->memory_size + 8);

    /* Ajout de la valeur */
    l->tab[l->size] = v;
    l->size++;
    return l;
}

GenList *genListInsert(GenList *l, void *v, unsigned int i) {
    /* vérification paramêtres */
    if (testArgNull(l, "genListInsert"))
        return NULL;
    if (i > l->size) {
        warnl("genericlist.c", "genListInsert", "position (%d) invalide", i);
        errno = EINVAL;
        return NULL;
    }

    /* agrandissement de la liste si pleine */
    if (l->size >= l->memory_size)
        adjustMemorySize(l, l->memory_size + 8);

    /* décale tous les éléments */
    for (int j = l->size - 1; j >= (int)i; j--)
        l->tab[j + 1] = l->tab[j];

    /* ajoute le nouvel élément */
    l->tab[i] = v;
    l->size++;

    return l;
}

int genListPop(GenList *l) {
    /* vérification paramêtre */
    if (testArgNull(l, "listPop"))
        return -1;
    if (l->size <= 0) {
        warnl("list.c", "listPop", "liste déjà vide");
        errno = EPERM;
        return -1;
    }
    /* suppression de l'élément */
    l->size--;
    adjustMemorySize(l, l->size);
    return 0;
}

int genListRemove(GenList *l, unsigned int i) {
    /* vérification paramêtres */
    if (testArgNull(l, "genListRemove"))
        return -1;
    if (i >= l->size) {
        warnl("genericlist.c", "genListRemove", "position (%d) invalide", i);
        errno = EINVAL;
        return -1;
    }
    /* suppression de l'élément */
    for (int j = i; j < (int)l->size - 1; j++)
        l->tab[j] = l->tab[j + 1];
    l->size--;
    adjustMemorySize(l, l->size);
    return 0;
}

bool genListEmpty(GenList *l) {
    if (testArgNull(l, "listEmpty"))
        return true;
    return l->size == 0;
}

unsigned int genListSize(GenList *l) {
    if (testArgNull(l, "genListSize"))
        return 0;
    return l->size;
}

GenList *genListCopy(GenList *l) {
    /* vérification paramêtre */
    if (testArgNull(l, "listCopy"))
        return NULL;

    /* création nouvelle liste */
    GenList *new = createGenList(l->size);
    if (errno) {
        warnl("genericlist.c", "genListCopy", "Echec création copie");
        return NULL;
    }

    /* copie des éléments */
    for (unsigned int i = 0; i < l->size; i++) {
        genListAdd(new, l->tab[i]);
        if (errno) {
            warnl("genericlist.c", "genListCopy", "Echec copie éléments");
            return NULL;
        }
    }

    return new;
}

void *genListGet(GenList *l, unsigned int i) {
    /* vérification paramêtre */
    if (testArgNull(l, "genListGet"))
        return NULL;
    if (i >= l->size) {
        warnl("genericlist.c", "genListGet", "position (%d) invalide", i);
        errno = EINVAL;
        return NULL;
    }

    return l->tab[i];
}

void displayGenList(GenList *l) {
    /* vérification paramêtre */
    if (testArgNull(l, "displayGenList"))
        return;

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

/* Définition de la structure list_ite */
struct s_gen_list_ite {
    GenList *list;              /* liste à parcourir */
    int cur;                    /* position actuelle */
    unsigned int (*fnext)(int); /* focntion de déplcament */
    bool next;                  /* appel à fonction next */
};

unsigned int next_forward(int i) { return i + 1; }

unsigned int next_backward(int i) { return i - 1; }

GenListIte *createGenListIte(GenList *l, int dir) {
    if (testArgNull(l, "createGenListIte"))
        return NULL;

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
        warnl("genericlist.c", "createGenListIte", "sens de parcours invalide");
        free(ite);
        return NULL;
    }
    return ite;
}

bool genListIteHasNext(GenListIte *ite) {
    if (testArgNull(ite, "genListIteHasNext"))
        return false;
    unsigned int next = ite->fnext(ite->cur);
    return (next < ite->list->size);
}

int genListIteNext(GenListIte *ite) {
    if (testArgNull(ite, "genListIteNext"))
        return -1;
    if (!genListIteHasNext(ite)) {
        warnl("genericlist.c", "genListIteNext", "aucun élément à lire");
        return -1;
    }

    ite->cur = ite->fnext(ite->cur);
    ite->next = true;
    return 0;
}

void *genListIteGetValue(GenListIte *ite) {
    if (testArgNull(ite, "genListIteGetValue"))
        return NULL;
    if (!ite->next) {
        warnl("genericlist.c", "genListIteGetValue", "lecture sans appel à next");
        return NULL;
    }
    return genListGet(ite->list, ite->cur);
}

void deleteGenListIte(ptrGenListIte *ite) {
    if (testArgNull(ite, "genListIteGetValue"))
        return;
    free((*ite)->list);
    free((*ite));
    *ite = NULL;
}

/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

void printGenListLog(GenList *l) {
    if (testArgNull(l, "printGenListLog"))
        return;
    char buff[2048];

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