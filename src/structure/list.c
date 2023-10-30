
/**
 * @file list.c
 * @author VALLAT Ugo
 * @date 30/10/2023
 *
 * @brief Implémentation de la liste pseudo statique d'entiers
 *
 * Implémente la liste sous forme d'un tableau statique et alloue
 * de la mémoire dynamiquement lorsque qu'il est plein
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
 * @brief Verifie si l'argument est NULL, si vrai position errno à EFAULT
 * et affiche un warning
 * @param[in] arg argument à vérifier
 * @param[in] caller Nom de la fonction appelante
 * @return true si arg = NULL, false sinon
 *
 */
bool testArgNull(void *arg, char *caller) {
    if (arg == NULL) {
        warnl("list.c", "hisArgNull", "dans %s > erreur pointeur null", caller);
        errno = EFAULT;
        return true;
    }
    return false;
}

List *createList(unsigned int size) {
    List *l = malloc(sizeof(List));
    if (l == NULL)
        exitl("list.c.c", "createList", EXIT_FAILURE, "erreur malloc list");

    l->tab = malloc(sizeof(int) * size);
    if (l->tab == NULL)
        exitl("list.c.c", "createList", EXIT_FAILURE, "erreur malloc tab");

    l->memory_size = size;
    l->size = 0;
    return l;
}

void deleteList(ptrList *l) {
    /* test l != NULL */
    if (testArgNull(l, "deleteList") || testArgNull((*l), "deleteList"))
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
void adjustMemorySize(List *l, unsigned int new_size) {
    /* nouvelle taille de la liste */
    l->memory_size = new_size;

    /* modification taille du tableau */
    l->tab = realloc(l->tab, new_size * sizeof(int));
    if (new_size != 0 && l->tab == NULL)
        exitl("list.c", "adjustMemorySize", EXIT_FAILURE, "echec realloc tab");
}

List *listAdd(List *l, int v) {
    /* test l != NULL */
    if (testArgNull(l, "listAdd"))
        return NULL;

    /* agrandissement de la liste si pleine */
    if (l->size == l->memory_size)
        adjustMemorySize(l, l->memory_size + 8);

    /* Ajout de la valeur */
    l->tab[l->size] = v;
    l->size++;
    return l;
}

List *listInsert(List *l, int v, unsigned int i) {
    /* vérification paramêtres */
    if (testArgNull(l, "listInsert"))
        return NULL;
    if (i > l->size) {
        warnl("list.c.c", "listInsert", "position (i) invalide");
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

int listPop(List *l) {
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

int listRemove(List *l, unsigned int i) {
    /* vérification paramêtres */
    if (testArgNull(l, "listRemove"))
        return -1;
    if (i >= l->size) {
        warnl("list.c.c", "listRemove", "position (i) invalide");
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

bool listEmpty(List *l) {
    if (testArgNull(l, "listEmpty"))
        return true;
    return l->size == 0;
}

unsigned int listSize(List *l) {
    if (testArgNull(l, "lestSize"))
        return 0;
    return l->size;
}

List *listCopy(List *l) {
    /* vérification paramêtre */
    if (testArgNull(l, "listCopy"))
        return NULL;

    /* création nouvelle liste */
    List *new = createList(l->size);
    if (errno) {
        warnl("list.c", "listCopy", "Echec création copie");
        return NULL;
    }

    /* copie des éléments */
    for (unsigned int i = 0; i < l->size; i++) {
        listAdd(new, l->tab[i]);
        if (errno) {
            warnl("list.c", "listCopy", "Echec copie éléments");
            return NULL;
        }
    }

    return new;
}

int listGet(List *l, unsigned int i) {
    /* vérification paramêtre */
    if (testArgNull(l, "listGet"))
        return -1;
    if (i >= l->size) {
        warnl("list.c.c", "listGet", "position (i) invalide");
        errno = EINVAL;
        return -1;
    }

    return l->tab[i];
}

void displayList(List *l) {
    /* vérification paramêtre */
    if (testArgNull(l, "displayList"))
        return;

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

/* Définition de la structure list_ite */
struct s_list_ite {
    List *list;                 /* liste à parcourir */
    int cur;                    /* position actuelle */
    unsigned int (*fnext)(int); /* focntion de déplcament */
    bool next;                  /* appel à fonction next */
};

unsigned int next_forward(int i) { return i + 1; }

unsigned int next_backward(int i) { return i - 1; }

ListIte *createListIte(List *l, int dir) {
    if (testArgNull(l, "createListIte"))
        return NULL;

    /* création de l'itérateur */
    ListIte *ite = malloc(sizeof(ListIte));

    /* copie de la liste */
    ite->list = listCopy(l);
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
        warnl("list.c", "createListIte", "sens de parcours invalide");
        free(ite);
        return NULL;
    }
    return ite;
}

bool listIteHasNext(ListIte *ite) {
    if (testArgNull(ite, "listIteHasNext"))
        return false;
    unsigned int next = ite->fnext(ite->cur);
    return (next < ite->list->size);
}

int listIteNext(ListIte *ite) {
    if (testArgNull(ite, "listIteNext"))
        return -1;
    if (!listIteHasNext(ite)) {
        warnl("list.c", "listIteNext", "aucun élément à lire");
        return -1;
    }

    ite->cur = ite->fnext(ite->cur);
    ite->next = true;
    return 0;
}

int listIteGetValue(ListIte *ite) {
    if (testArgNull(ite, "listIteGetValue"))
        return -1;
    if (!ite->next) {
        warnl("list.c", "listIteGetValue", "lecture sans appel à next");
        return -1;
    }
    return listGet(ite->list, ite->cur);
}

void deleteListIte(ptrListIte *ite) {
    if (testArgNull(ite, "listIteGetValue"))
        return;
    free((*ite)->list);
    free((*ite));
    *ite = NULL;
}

/*------------------------------------------------------------------*/
/*                              DEBUG                               */
/*------------------------------------------------------------------*/

#ifdef DEBUG

void printListLog(List *l) {
    if (testArgNull(l, "printListLog"))
        return;
    char buff[2048];

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