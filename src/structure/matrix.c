/**
 * @file matrix.c
 * @author VALLAT Ugo
 *
 * @brief Cette librairie implémente implémente une matrice d'entiers
 *
 * @note Matrice de taille fixe pour garrantir un accès en lecture et
 *  en écriture de l'ordre de O(1)
 *
 * @note Implémente un nombre varié d'accesseur et d'opérateurs pour
 * à privilégiés pour de meilleures performances.
 *
 * @remark En cas d'erreur, toutes les fonctions de list exit le progamme avec un
 * message d'erreur
 */

#include <asm-generic/errno-base.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../logger.h"
#include "data_struct_utils.h"
#include "genericlist.h"
#include "list.h"
#include "matrix.h"

/*------------------------------------------------------------------*/
/*                        STRUCTURE MATRIX                           */
/*------------------------------------------------------------------*/

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Définition de la structure matrice
 *
 * @remark Implémentation sous forme d'une liste de liste
 */
struct s_matrix {
    int default_value; /* Valeur par défault d'une case "vide" */
    unsigned int nbl;  /* nombre de lignes */
    unsigned int nbc;  /* nombre de colonnes */
    GenList *tab;      /* liste générique de lignes */
};

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
Matrix *createMatrix(unsigned int nbl, unsigned int nbc, int default_value) {
    /* Création matrice */
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL)
        exitl("matrix.c", "createMatrix", EXIT_FAILURE, "Echec malloc matrice");

    /* création liste de lignes (liste générique) */
    m->tab = createGenList(nbl);

    /* création des lignes */
    List *l;
    for (unsigned int i = 0; i < nbl; i++) {
        /* créer une ligne */
        l = createList(nbc);

        /* remplir la ligne */
        for (unsigned int j = 0; j < nbc; j++)
            listAdd(l, default_value);

        /* ajouter ligne à la matrice */
        genListAdd(m->tab, l);
    }

    m->default_value = default_value;
    m->nbl = nbl;
    m->nbc = nbc;
    return m;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void deleteMatrix(ptrMatrix *m) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "deleteMatrix", "m");
    testArgNull(*m, "matrix.c", "deleteMatrix", "*m");
#endif

    /* parcours de la liste générique */
    List *l;
    GenListIte *ite = createGenListIte((*m)->tab, FROM_BEGIN);
    while (genListIteHasNext(ite)) {
        genListIteNext(ite);
        /* suppression de la ligne */
        l = (List *)genListIteGetValue(ite);
        deleteList(&l);
    }
    deleteGenListIte(&ite);

    /* suppression liste générique */
    deleteGenList(&((*m)->tab));

    /* supression matrice */
    free(*m);
    *m = NULL;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void matrixSet(Matrix *m, unsigned int l, unsigned int c, int v) {
#ifdef DEBUG
    /* test des arguments */
    testArgNull(m, "matrix.c", "matrixSet", "m");
    if (l >= m->nbl || c >= m->nbc)
        exitl("matrix.c", "matrixSet", EXIT_FAILURE,
              "position invalide (%d,%d) dans matrice (%d,%d)", l, c, m->nbl, m->nbc);
#endif

    listSet(genListGet(m->tab, l), v, c);
}


/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
int matrixGet(Matrix *m, unsigned int l, unsigned int c) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixGet", "m");
    if (l >= m->nbl || c >= m->nbc)
        exitl("matrix.c", "matrixGet", EXIT_FAILURE,
              "position invalide (%d,%d) dans matrice (%d,%d)", l, c, m->nbl, m->nbc);
#endif

    return listGet(genListGet(m->tab, l), c);
}

/**
 * @date  14/11/2023
 * @author Ugo VALLAT
 */
unsigned int matrixNbLines(Matrix *m) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixNbLines", "m");
#endif

    return m->nbl;
}


/**
 * @date  14/11/2023
 * @author Ugo VALLAT
 */
unsigned int matrixNbColonnes(Matrix *m) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixNbColonnes", "m");
#endif

    return m->nbc;
}


/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void matrixInsertLine(Matrix *m, unsigned int l) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixInsertLine", "m");
    if (l > m->nbl)
        exitl("matrix.c", "matrixInsertLine", EXIT_FAILURE, "position %d invalide", l);
#endif

    /* création nouvelle ligne */
    List *line = createList(m->nbc);

    /* remplissage de la ligne avec default_value */
    for (unsigned int i = 0; i < m->nbc; i++)
        listAdd(line, m->default_value);

    /* ajout de la ligne dans la matrice */
    genListInsert(m->tab, line, l);
    m->nbl++;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void matrixRemoveLine(Matrix *m, unsigned int l) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixRemoveLine", "m");
    if (l >= m->nbl)
        exitl("matrix.c", "matrixRemoveLine", EXIT_FAILURE, "position %d invalide", l);
#endif

    List *line = (List *)genListGet(m->tab, l);
    deleteList(&line);
    genListRemove(m->tab, l);
    m->nbl--;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void matrixInsertColumn(Matrix *m, unsigned int c) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixInsertColumn", "m");
    if (c > m->nbc)
        exitl("matrix.c", "matrixInsertColumn", EXIT_FAILURE, "position %d invalide", c);
#endif

    /* ajout valeur par default à toutes les lignes */
    List *line;
    GenListIte *ite = createGenListIte(m->tab, FROM_BEGIN);
    while (genListIteHasNext(ite)) {
        genListIteNext(ite);
        line = genListIteGetValue(ite);
        listInsert(line, m->default_value, c);
    }
    deleteGenListIte(&ite);
    m->nbc++;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void matrixRemoveColumn(Matrix *m, unsigned int c) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixRemoveColumn", "m");
    if (c >= m->nbc)
        exitl("matrix.c", "matrixRemoveColumn", EXIT_FAILURE, "position %d invalide", c);
#endif

    /* suppression élément dans chaque ligne */
    List *line;
    GenListIte *ite = createGenListIte(m->tab, FROM_BEGIN);
    while (genListIteHasNext(ite)) {
        genListIteNext(ite);
        line = genListIteGetValue(ite);
        listRemove(line, c);
    }
    deleteGenListIte(&ite);
    m->nbc--;
}


/*------------------------------------------------------------------*/
/*                          ITERATEUR                               */
/*------------------------------------------------------------------*/

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Implémentation de la structure de l'itérateur sur la matrice
 */
struct s_matrixIte {
    Matrix *matrix;            /* matrice à parcourir */
    int cur_l;                 /* ligne actuelle */
    int cur_c;                 /* colonne actuelle */
    int line;                  /* ligne à parcourir (-1 = toutes) */
    int column;                /* colonne à parcourir (-1 = toutes) */
    fun_ite fun;               /* fonction à appliquer à chaque élément */
    void *buff;                /* buffer utilisé par la fonction fun */
    bool next;                 /* appel à fonction next */
    unsigned int shape_ref[2]; /* taille de référence [nbl, nbc] */
};

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Fonction par défault utilisée par l'itérateur si aucune fonction spécifiée, renvoie
 * la valeur le case courrante (aucune modification)
 *
 * @param v Valeur de la case courante
 * @param l Ligne courante
 * @param c Colonne courante
 * @param buff Buffer (non utilisé)
 * @return valeur de v en entrée
 */
int default_fun(int v, unsigned int l, unsigned int c, void *buff) { 
    (void)l; (void)c; (void)buff;
    return v; 
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
MatrixIte *createMatrixIte(Matrix *m, int l, int c, fun_ite fun, void *buff) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "createMatrixIte", "m");
    if (l >= (int)m->nbl || l < -1 || c >= (int)m->nbc || c < -1)
        exitl("matrix.c", "createMatrixIte", EXIT_FAILURE,
              "argument invalide (%d,%d) dans matrice (%d,%d)", l, c, m->nbl, m->nbc);
#endif

    /* initialisation de l'itérateur */
    MatrixIte *ite = malloc(sizeof(MatrixIte));
    ite->matrix = m;
    ite->next = false;
    ite->shape_ref[0] = m->nbl;
    ite->shape_ref[1] = m->nbc;
    ite->buff = buff;
    
    /* Initialisation de la fonction */
    if (fun != NULL)
        ite->fun = fun;
    else
        ite->fun = default_fun;

    /* Initialisation des informations de parcours */
    ite->line = l;
    ite->column = c;

    if (l == -1 && c == -1) { /* parcours toute la matrice */
        ite->cur_l = 0;
        ite->cur_c = -1;
    } else if (l == -1) { /* parcours une colonne */
        ite->cur_l = -1;
        ite->cur_c = c;
    } else if (c == -1) { /* parcours une ligne */
        ite->cur_l = l;
        ite->cur_c = -1;
    } else { /* Parcours une case */
        ite->cur_l = -1;
        ite->cur_c = -1;
    }

    return ite;
}

#ifdef DEBUG
/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Test si la matrice a été modifiée (taille de la liste) durant le parcours de
 * l'itérateur et exit si oui
 *
 * @param ite Itérateur à vérifier (contient la taille de référence et la matrice)
 */
void testMatrixModification(MatrixIte *ite) {
    testArgNull(ite, "matrix.c", "matrixIteHasNext", "ite");
    if (!(ite->shape_ref[0] == ite->matrix->nbl && ite->shape_ref[1] == ite->matrix->nbc))
        exitl("matrix.c", "testMatrixModification ", EXIT_FAILURE,
              "Matrice modifiée pdurant parcours");
}
#endif

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
bool matrixIteHasNext(MatrixIte *ite) {
#ifdef DEBUG
    testArgNull(ite, "matrix.c", "matrixIteHasNext", "ite");
    testMatrixModification(ite);
#endif

    int nbl = (int)ite->matrix->nbl;
    int nbc = (int)ite->matrix->nbc;

    /* matrice vide */
    if(ite->matrix->nbl == 0 || ite->matrix->nbc == 0)
        return false;
    /* Parcours une case */
    if (ite->line > -1 && ite->column > -1)
        return ite->cur_l != ite->line && ite->cur_c != ite->column;
    /* parcours une colonne */
    if (ite->line == -1 && ite->column != -1)
        return ite->cur_l < nbl - 1;
    /* parcours une ligne */
    if (ite->line != -1 && ite->column == -1)
        return ite->cur_c < nbc - 1;
    /* parcours toute la matrice */
    if (ite->line == -1 && ite->column == -1)
        return (ite->cur_c < nbc - 1 || ite->cur_l < nbl - 1);

    /* Erreur structure */
    exitl("matrix.c", "matrixIteHasNext", EXIT_FAILURE, "Erreur parametre ligne/colonne : (%d,%d)",
          ite->line, ite->column);
    return false;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
int matrixIteNext(MatrixIte *ite) {
#ifdef DEBUG
    testArgNull(ite, "matrix.c", "matrixIteNext", "ite");
    testMatrixModification(ite);
    if (!matrixIteHasNext(ite))
        exitl("matrix.c", "matrixIteNext", EXIT_FAILURE, "Fin de matrice atteint");
#endif

    /* ##### Deplacement #####*/

    /* Parcours une case */
    if (ite->line != -1 && ite->column != -1) {
        ite->cur_l = ite->line;
        ite->cur_c = ite->column;
    }
    /* parcours une colonne */
    if (ite->line == -1 && ite->column != -1) {
        ite->cur_l = ite->cur_l + 1;
    }
    /* parcours une ligne */
    if (ite->line != -1 && ite->column == -1) {
        ite->cur_c = ite->cur_c + 1;
    }
    /* parcours toute la matrice */
    if (ite->line == -1 && ite->column == -1) {
        if (ite->cur_c < ((int)ite->matrix->nbc) - 1)
            ite->cur_c++;
        else {
            ite->cur_l++;
            ite->cur_c = 0;
        }
    }

    /* ##### Lecture et Ecriture ##### */

    int before; /* valeur avant modification */
    int after;  /* valeur après modification */

    before = matrixGet(ite->matrix, ite->cur_l, ite->cur_c);
    after = ite->fun(before, ite->cur_l, ite->cur_c, ite->buff);

    matrixSet(ite->matrix, ite->cur_l, ite->cur_c, after);
    ite->next = true;
    return before;
}


/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void* deleteMatrixIte(ptrMatrixIte *ite) {
#ifdef DEBUG
    testArgNull(ite, "matrix.c", "matrixIteGetValue", "ite");
#endif

    void* buff = (*ite)->buff;
    free(*ite);
    *ite = NULL;
    return buff;
}

/*------------------------------------------------------------------*/
/*                            UTILS                                 */
/*------------------------------------------------------------------*/

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
void matrixMap(Matrix *m, int l, int c, fun_ite fun, void *buff) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixMap", "m");
    if (l >= (int)m->nbl || l < -1 || c >= (int)m->nbc || c < -1)
        exitl("matrix.c", "matrixMap", EXIT_FAILURE,
              "argument invalide (l,c)=(%d,%d) dans matrice (%d,%d)", l, c, m->nbl, m->nbc);
#endif

    MatrixIte *ite = createMatrixIte(m, l, c, fun, buff);
    while (matrixIteHasNext(ite)) {
        matrixIteNext(ite);
    }
    deleteMatrixIte(&ite);
}


/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Compare la valeur courante à celle du buffer et stock le minimum dans le buffer (avec ses
 * coordonnées)
 * @param[in] v Valeur courante à comparer
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff Pointeur vers le tableau [min, ligne_min, colone_min]
 * @return élément courant
 */
int fun_min(int v, unsigned int l, unsigned int c, void *buff) {
    GenList* lmin = (GenList*)buff;
    int* cur;
    if(v < 0) return v;
    if(genListEmpty(lmin) || ((int*)genListGet(lmin, 0))[0] <= v) {
        if(!genListEmpty(lmin) && ((int*)genListGet(lmin, 0))[0] < v)
            while(!genListEmpty(lmin))
                free(genListPop(lmin));
        cur = malloc(sizeof(int)*3);
        cur[0] = v;
        cur[1] = l;
        cur[2] = c;
        genListAdd(lmin, cur);
    }
    return v;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
GenList *matrixMin(Matrix *m, int l, int c) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixMin", "m");
    if (l >= (int)m->nbl || l < -1 || c >= (int)m->nbc || c < -1)
        exitl("matrix.c", "matrixMin", EXIT_FAILURE,
              "argument invalide (l,c)=(%d,%d) dans matrice (%d,%d)", l, c, m->nbl, m->nbc);
#endif

    /* création buffer min */
    GenList* lmin = createGenList(1);

    /* parcours de la matrice */
    matrixMap(m, l, c, fun_min, lmin);
    return lmin;
}



/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 * @brief Ajoute l'élément courant à la nouvelle matrice
 * @param[in] v Valeur courante
 * @param[in] l Ligne courante
 * @param[in] c Colonne courante
 * @param[in] buff Pointeur vers la nouvelle matrice
 * @return élément courant
 */
int fun_copy(int v, unsigned int l, unsigned int c, void *buff) {
    Matrix *m = (Matrix *)buff;
    matrixSet(m, l, c, v);
    return v;
}

/**
 * @date  1/11/2023
 * @author Ugo VALLAT
 */
Matrix *matrixCopy(Matrix *m) {
#ifdef DEBUG
    testArgNull(m, "matrix.c", "matrixCopy", "m");
#endif

    /* Création nouvelle matrice */
    Matrix *new = createMatrix(m->nbl, m->nbc, m->default_value);

    /* parcours de la matrice */
    MatrixIte *ite = createMatrixIte(m, -1, -1, fun_copy, new);
    while (matrixIteHasNext(ite))
        matrixIteNext(ite);
    deleteMatrixIte(&ite);

    return new;
}
