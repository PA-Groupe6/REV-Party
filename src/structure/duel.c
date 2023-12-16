/**
 * @author VALLAT Ugo
 * @brief Structure Duel implémentation
 */

#include "bale.h"
#include "data_struct_utils.h"
#include "genericlist.h"
#include <stdlib.h>
#include "duel.h"
#include "matrix.h"
#include <malloc.h>
#include "../logger.h"
#include <string.h>
#include "bale.h"


/*------------------------------------------------------------------*/
/*                         STRUCTURE DUEL                           */
/*------------------------------------------------------------------*/

/**
 * @date  13/11/2023
 * @brief Définition de la structure Duel
 */
struct s_duel {
    int default_value;
    GenList* labels;    /* Liste des labels */
    Matrix* matrix;     /* Matrice contenant les données */
};


/**
 * @author Ugo VALLAT
 * @date 30/11/2023
 */
Duel *createDuel(unsigned nb_candidats, GenList *labels) {
#ifdef DEBUG
    testArgNull(labels, "duel.c", "createDuel", "labels");
    if(genListSize(labels) != nb_candidats)
        exitl("duel.c", "createDuel", EXIT_FAILURE, "Nombre labels (%d) != nombre colonnes (%d)", genListSize(labels), nb_candidats);
#endif

    Duel* duel = malloc(sizeof(Duel));
    duel->default_value = DEFAULT_VALUE;
    duel->labels = copyLabels(labels);
    duel->matrix = createMatrix(nb_candidats, nb_candidats, DEFAULT_VALUE);
    return duel;
}


/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
void deleteDuel(ptrDuel *d) {
#ifdef DEBUG
    testArgNull(d, "duel.c", "deleteDuel", "d");
    testArgNull(*d, "duel.c", "deleteDuel", "*d");
#endif
    deleteMatrix(&((*d)->matrix));
    while (!genListEmpty((*d)->labels)) {
        free(genListPop((*d)->labels));
    }
    deleteGenList(&((*d)->labels));
    free(d);
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
Duel *duelSetValue(Duel *d, unsigned int l, unsigned int c, int v) {
#ifdef DEBUG
    testArgNull(d, "duel.c", "duelSetValue", "d");
    if(l > matrixNbLines(d->matrix) || c > matrixNbColonnes(d->matrix))
        exitl("duel.c", "duelSetValue", EXIT_FAILURE, "Invalide pos (%d,%d) dans duel (%d,%d)", 
            l,c, matrixNbLines(d->matrix), matrixNbColonnes(d->matrix));
    if(matrixGet(d->matrix, l, c) != d->default_value)
        exitl("duel.c", "duelSetValue", EXIT_FAILURE, "Echec set : valeur déjà set");
#endif

    matrixSet(d->matrix, l, c, v);
    return d;
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
int duelGetValue(Duel *d, unsigned int l, unsigned int c) {
#ifdef DEBUG
    testArgNull(d, "duel.c", "duelGetValue", "d");
    if(l > matrixNbLines(d->matrix) || c > matrixNbColonnes(d->matrix))
        exitl("duel.c", "duelGetValue", EXIT_FAILURE, "Invalide pos (%d,%d) dans duel (%d,%d)", 
            l,c, matrixNbLines(d->matrix), matrixNbColonnes(d->matrix));
#endif

    return matrixGet(d->matrix, l, c);
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
unsigned int duelNbCandidat(Duel *d) {
#ifdef DEBUG
    testArgNull(d, "duel.c", "duelNbCandidat", "d");
#endif

    return matrixNbColonnes(d->matrix);
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
int duelLabelToIndex(Duel *d, char *label) {
#ifdef DEBUG
    testArgNull(d, "duel.c", "duelLabelToIndex", "d");
    testArgNull(label, "duel.c", "duelLabelToIndex", "label");
#endif

    return searchLabel(d->labels, label);
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
char *duelIndexToLabel(Duel *d, unsigned int index) {
#ifdef DEBUG
    testArgNull(d, "duel.c", "duelIndexToLabel", "d");
    unsigned size = matrixNbColonnes(d->matrix);
    if(index > size)
        exitl("duel.c", "duelIndexToLabel", EXIT_FAILURE, "Invalide index (%d) dans duel (%d,%d)",
            index, size, size);
#endif

    char* label = malloc(sizeof(char)*MAX_LENGHT_LABEL);
    strncpy(label, genListGet(d->labels, index), MAX_LENGHT_LABEL);
    return label;
}


Duel* duelFromBale(Bale *b) {
#ifdef DEBUG
    testArgNull(b, "duel.c", "duelFromBale", "b");
#endif
    unsigned nbl, nbc;
    nbl = baleNbVoter(b);
    nbc = baleNbCandidat(b);

    /* récupération des labels */
    GenList *labels = createGenList(nbc);
    for(unsigned i = 0; i < nbc; i++) {
        genListAdd(labels, baleColumnToLabel(b, i));
    }

    /* création de duel */
    Duel* duel = malloc(sizeof(Duel));
    duel->default_value = DEFAULT_VALUE;
    duel->labels = labels;
    duel->matrix = createMatrix(nbc, nbc, 0);

    /* calcul des scores */
    int score_x, score_y;
    for(unsigned l = 0; l < nbl; l++) {
        for(unsigned x = 0; x < nbc-1; x++) {
            for(unsigned y = x + 1; y< nbc; y++) {
                score_x = baleGetValue(b, l, x);
                score_y = baleGetValue(b, l, y);
                if(score_x!=score_y) {
                    if(score_x == -1 || (score_y!=-1 && score_x>score_y))
                        matrixSet(duel->matrix, y, x, matrixGet(duel->matrix, y, x)+1);
                    else
                        matrixSet(duel->matrix, x, y, matrixGet(duel->matrix, x, y)+1);
                }
            }
        }
    }

    return duel;

}

