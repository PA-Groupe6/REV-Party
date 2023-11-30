/**
 * @author VALLAT Ugo
 * @brief Structure Duel header
 *
 * @brief Une matrice de duels correspond à une matrice où chaque ligne correspond au
 * score d'un candidat en duel face à un autre candidat de la colonne
 *
 * Les lignes et les colonnes son étiquetée avec le nom du candidat.
 *
 * Il y a autant de lignes que de colonnes et le label d'une ligne et d'une colonne de même
 * indice est similaire
 *
 * La matrice de duels est de taille fixe et ses éléments ne sont pas modifiables après avoir été
 * chargés dans la matrice.
 *
 * @note Taille maximum des étiquetées = @ref MAX_LENGHT_LABEL
 *
 * @remark Les dimensions d'une matrice de duels sont fixes
 *
 * @remark En cas d'erreur, toutes les fonctions de la matrice de duels exit le progamme avec un
 * message d'erreur
 */

#include "bale.h"
#include "data_struct_utils.h"
#include "genericlist.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "duel.h"
#include "matrix.h"
#include <malloc.h>
#include "../logger.h"
#include <string.h>




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
    testArgNull(labels, "duel.c", "createDuel", "labels");
    if(genListSize(labels) != nb_candidats)
        exitl("duel.c", "createDuel", EXIT_FAILURE, "Nombre labels (%d) != nombre colonnes (%d)", genListSize(labels), nb_candidats);

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
    testArgNull(d, "duel.c", "deleteDuel", "d");
    testArgNull(*d, "duel.c", "deleteDuel", "*d");
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
    testArgNull(d, "duel.c", "duelSetValue", "d");
    if(l > matrixNbLines(d->matrix) || c > matrixNbColonnes(d->matrix))
        exitl("duel.c", "duelSetValue", EXIT_FAILURE, "Invalide pos (%d,%d) dans duel (%d,%d)", 
            l,c, matrixNbLines(d->matrix), matrixNbColonnes(d->matrix));
    if(matrixGet(d->matrix, l, c) != d->default_value)
        exitl("duel.c", "duelSetValue", EXIT_FAILURE, "Echec set : valeur déjà set");
    matrixSet(d->matrix, l, c, v);
    return d;
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
int duelGetValue(Duel *d, unsigned int l, unsigned int c) {
    testArgNull(d, "duel.c", "duelGetValue", "d");
    if(l > matrixNbLines(d->matrix) || c > matrixNbColonnes(d->matrix))
        exitl("duel.c", "duelGetValue", EXIT_FAILURE, "Invalide pos (%d,%d) dans duel (%d,%d)", 
            l,c, matrixNbLines(d->matrix), matrixNbColonnes(d->matrix));
    
    return matrixGet(d->matrix, l, c);
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
unsigned int duelNbCandidat(Duel *d) {
    testArgNull(d, "duel.c", "duelNbCandidat", "d");
    return matrixNbColonnes(d->matrix);
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
int duelLabelToIndex(Duel *d, char *label) {
    testArgNull(d, "duel.c", "duelLabelToIndex", "d");
    testArgNull(label, "duel.c", "duelLabelToIndex", "label");
    return searchLabel(d->labels, label);
}

/**
 * @author Ugo VALLAT
 * @date 30/11/2023
*/
char *duelIndexToLabel(Duel *d, unsigned int index) {
    testArgNull(d, "duel.c", "duelIndexToLabel", "d");
    unsigned size = matrixNbColonnes(d->matrix);
    if(index > size)
        exitl("duel.c", "duelIndexToLabel", EXIT_FAILURE, "Invalide index (%d) dans duel (%d,%d)",
            index, size, size);
    char* label = malloc(sizeof(char)*MAX_LENGHT_LABEL);
    strncpy(label, genListGet(d->labels, index), MAX_LENGHT_LABEL);
    return label;
}

